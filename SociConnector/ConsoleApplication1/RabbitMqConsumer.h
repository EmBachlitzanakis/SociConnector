#pragma once
#include <string>
#include <amqp.h>
#include <amqp_tcp_socket.h>
#include <amqp_framing.h>

class RabbitMqConsumer {
public:
    RabbitMqConsumer(const std::string& host, int port,
        const std::string& exchange,
        const std::string& queue,
        const std::string& routingKey);
    ~RabbitMqConsumer();

    bool connect();
    std::string consume();

private:
    void die_on_error(int x, const char* context);
    void die_on_amqp_error(amqp_rpc_reply_t x, const char* context);

    std::string hostname;
    int port;
    std::string exchange_name;
    std::string queue_name;
    std::string routing_key;

    amqp_connection_state_t conn;
    amqp_socket_t* socket;
};
