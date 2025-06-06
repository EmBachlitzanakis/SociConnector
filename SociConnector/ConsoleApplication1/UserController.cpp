
#include "UserController.h"
#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>
#include "User.h"
#include <iostream>
#include <sstream>
#include "RabbitMqConsumer.h"

extern const std::string connectionString;

// Connection string for your DB
const std::string connectionString =
"DRIVER={ODBC Driver 17 for SQL Server};"
"SERVER=PR-BACHLITZANA;"
"DATABASE=FIDUCIAM_PROD;"
"UID=db2admin;"
"PWD=db2admin1;";

void UserController::register_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/insert")
        ([](crow::response& res) {
        std::ostringstream os;

        try
        {
            std::cout << "Connecting with connection string:\n" << connectionString << std::endl;
            soci::session sql(soci::odbc, connectionString);
            std::cout << "Successfully connected to MS SQL Server database!" << std::endl;


            User user(
                "usercode1", "John", "A.", "Doe", "john.doe@email.com", 0, "EN", "password123", "2025-05-28 10:00:00", 0, "D", "0",
                0, "D", 3, "2025-05-28 10:00:00", "ebachlitzanakis@profilesw.com-2"
            );

            sql << "INSERT INTO [dbo].[USER] ("
                "[USER_CODE], [USER_FNAME], [USER_MNAME], [USER_LNAME], [USER_EMAIL], [USER_STATUS], "
                "[LANG_CODE], [USER_PASSWORD], [USER_PSWDUPDDATE], [USER_VALPERNO], [USER_VALPERTYPE], "
                "[USER_GRCTYPE], [USER_GRCPERNO], [USER_GRCPERTYPE], [USER_TRIESLEFT], [USER_INSDATE], [SECO_CODE]) "
                "VALUES (:user_code, :user_fname, :user_mname, :user_lname, :user_email, :user_status, "
                ":lang_code, :user_password, :user_pswdupddate, :user_valperno, :user_valpertype, "
                ":user_grctype, :user_grcperno, :user_grcpertype, :user_triesleft, :user_insdate, :seco_code)",
                soci::use(user.user_code, "user_code"),
                soci::use(user.user_fname, "user_fname"),
                soci::use(user.user_mname, "user_mname"),
                soci::use(user.user_lname, "user_lname"),
                soci::use(user.user_email, "user_email"),
                soci::use(user.user_status, "user_status"),
                soci::use(user.lang_code, "lang_code"),
                soci::use(user.user_password, "user_password"),
                soci::use(user.user_pswdupddate, "user_pswdupddate"),
                soci::use(user.user_valperno, "user_valperno"),
                soci::use(user.user_valpertype, "user_valpertype"),
                soci::use(user.user_grctype, "user_grctype"),
                soci::use(user.user_grcperno, "user_grcperno"),
                soci::use(user.user_grcpertype, "user_grcpertype"),
                soci::use(user.user_triesleft, "user_triesleft"),
                soci::use(user.user_insdate, "user_insdate"),
                soci::use(user.seco_code, "seco_code");
            //std::cout << "User inserted successfully!" << std::endl;
            std::cout << "User inserted successfully!" << std::endl;




            RabbitMqConsumer consumer("localhost", 5672, "my_direct_exchange", "my_queue", "my_routing_key");
            if (consumer.connect()) {
                if (consumer.consume() != "Success") {
                    sql << "DELETE FROM [dbo].[USER] WHERE [USER_CODE] = :user_code",
                        soci::use(user.user_code, "user_code");

                    std::cout << "User deleted successfully!" << std::endl;
                    os << "User deleted successfully!" << std::endl;

                }
                else {
                    // Example of a query to retrieve users
                    soci::rowset<soci::row> rs = (sql.prepare <<
                        "SELECT [USER_CODE], [USER_FNAME], [USER_MNAME], [USER_LNAME], "
                        "[USER_EMAIL], [USER_STATUS], [LANG_CODE], [USER_PASSWORD], [USER_PSWDUPDDATE], "
                        "[USER_VALPERNO], [USER_VALPERTYPE], [USER_GRCTYPE], [USER_GRCPERNO], "
                        "[USER_GRCPERTYPE], [USER_TRIESLEFT], [USER_INSDATE], [SECO_CODE] FROM [dbo].[USER] "
                        "WHERE [USER_CODE] = :user_code",
                        soci::use(user.user_code, "user_code"));
                    for (const auto& r : rs)
                    {
                        std::cout << "User: " << r.get<std::string>(0) << ", " << r.get<std::string>(1) << " " << r.get<std::string>(3) << std::endl;
                        os << "{";
                        os << "\"user_code\": \"" << r.get<std::string>(0) << "\", ";
                        os << "\"user_fname\": \"" << r.get<std::string>(1) << "\", ";
                        os << "\"user_lname\": \"" << r.get<std::string>(3) << "\"";
                        os << "}";

                    }
                }
            }





        }
        catch (const soci::soci_error& e)
        {
            std::cerr << "SOCI Error: Failed to connect to MS SQL Server." << std::endl;
            std::cerr << "  Details: " << e.what() << std::endl;

            const soci::odbc_soci_error* odbc_err = dynamic_cast<const soci::odbc_soci_error*>(&e);
            if (odbc_err)
            {
                std::cerr << "  ODBC Error Code: " << odbc_err->odbc_error_code() << std::endl;
                std::cerr << "  Native Error Code: " << odbc_err->native_error_code() << std::endl;
                std::cerr << "  ODBC Error Message: " << odbc_err->odbc_error_message() << std::endl;
            }
            // return 1;
        }
        catch (const std::exception& e)
        {
            std::cerr << "Standard Exception: An unexpected error occurred." << std::endl;
            std::cerr << "  Details: " << e.what() << std::endl;
            //  return 1;
        }
        res.set_header("Content-Type", "application/json");
        res.write(os.str());

        // Modify the response object as needed
        res.code = 200;
        //  res.body = "Hello, world!";
        res.end();
            });

    CROW_ROUTE(app, "/delete")
        ([](crow::response& res) {
        std::ostringstream os;

        try
        {
            std::cout << "Connecting with connection string:\n" << connectionString << std::endl;
            soci::session sql(soci::odbc, connectionString);
            std::cout << "Successfully connected to MS SQL Server database!" << std::endl;

            User user(
                "usercode1", "John", "A.", "Doe", "john.doe@email.com", 0, "EN", "password123", "2025-05-28 10:00:00", 0, "D", "0",
                0, "D", 3, "2025-05-28 10:00:00", "ebachlitzanakis@profilesw.com-2"
            );

            RabbitMqConsumer consumer("localhost", 5672, "my_direct_exchange", "my_queue", "my_routing_key");
            if (consumer.connect()) {
                if (consumer.consume() == "Success") {
                    sql << "DELETE FROM [dbo].[USER] WHERE [USER_CODE] = :user_code",
                        soci::use(user.user_code, "user_code");

                    std::cout << "User deleted successfully!" << std::endl;
                    os << "User deleted successfully!" << std::endl;

                }

            }

        }
        catch (const soci::soci_error& e)
        {

        }


        res.set_header("Content-Type", "application/json");
        res.write(os.str());
        res.code = 200;
        res.end();

            });
}
