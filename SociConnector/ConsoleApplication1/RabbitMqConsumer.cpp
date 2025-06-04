#include "RabbitMqConsumer.h"
#include <iostream>
#include <thread>
#include <chrono>

RabbitMqConsumer::RabbitMqConsumer(const std::string& host, int port,
    const std::string& exchange,
    const std::string& queue,
    const std::string& routingKey)
    : hostname(host), port(port), exchange_name(exchange),
    queue_name(queue), routing_key(routingKey), conn(nullptr), socket(nullptr) {
}

RabbitMqConsumer::~RabbitMqConsumer() {
    if (conn) {
        amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
    }
}

void RabbitMqConsumer::die_on_error(int x, const char* context) {
    if (x < 0) {
        std::cerr << "Error in " << context << ": " << amqp_error_string2(x) << std::endl;
        exit(1);
    }
}

void RabbitMqConsumer::die_on_amqp_error(amqp_rpc_reply_t x, const char* context) {
    switch (x.reply_type) {
    case AMQP_RESPONSE_NORMAL:
        return;
    case AMQP_RESPONSE_NONE:
        std::cerr << "Missing RPC reply type in " << context << std::endl;
        break;
    case AMQP_RESPONSE_LIBRARY_EXCEPTION:
        std::cerr << "Library error in " << context << ": " << amqp_error_string2(x.library_error) << std::endl;
        break;
    case AMQP_RESPONSE_SERVER_EXCEPTION:
        std::cerr << "Server exception in " << context << std::endl;
        break;
    }
    exit(1);
}

bool RabbitMqConsumer::connect() {
    conn = amqp_new_connection();
    die_on_error(conn == NULL ? -1 : 0, "creating AMQP connection");

    socket = amqp_tcp_socket_new(conn);
    die_on_error(socket == NULL ? -1 : 0, "creating TCP socket");

    int status = amqp_socket_open(socket, hostname.c_str(), port);
    die_on_error(status, "opening TCP socket");

    amqp_rpc_reply_t login_reply = amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    die_on_amqp_error(login_reply, "AMQP login");

    amqp_channel_open(conn, 1);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "opening channel");

    amqp_queue_declare(conn, 1, amqp_cstring_bytes(queue_name.c_str()),
        0, 1, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "declaring queue");

    amqp_queue_bind(conn, 1, amqp_cstring_bytes(queue_name.c_str()),
        amqp_cstring_bytes(exchange_name.c_str()),
        amqp_cstring_bytes(routing_key.c_str()),
        amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "binding queue");

    amqp_basic_consume(conn, 1, amqp_cstring_bytes(queue_name.c_str()),
        amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "consuming");

    return true;
}

std::string RabbitMqConsumer::consume() {
    amqp_envelope_t envelope;
    int msg_count = 0;

    
        amqp_rpc_reply_t result = amqp_consume_message(conn, &envelope, NULL, 0);

        if (AMQP_RESPONSE_NORMAL != result.reply_type) {
            if (AMQP_RESPONSE_LIBRARY_EXCEPTION == result.reply_type && AMQP_STATUS_TIMEOUT == result.library_error) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
               
            }
            die_on_amqp_error(result, "consuming message");
        }

        std::cout << "  Body: " << std::string((char*)envelope.message.body.bytes, envelope.message.body.len) << std::endl;
        std::string  RabbitQueMessage = std::string((char*)envelope.message.body.bytes, envelope.message.body.len);
        amqp_basic_ack(conn, 1, envelope.delivery_tag, 0);
        die_on_amqp_error(amqp_get_rpc_reply(conn), "acknowledging message");

        amqp_destroy_envelope(&envelope);
        std::cout << std::string((char*)envelope.message.body.bytes, envelope.message.body.len) << std::endl;
        return RabbitQueMessage;
    
}
