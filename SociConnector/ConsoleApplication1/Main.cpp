
//#include <jwt-cpp/jwt.h>

#include "crow.h"
#include "UserController.h"

crow::SimpleApp app;

int main()
{
	// Define the connection string for MS SQL Server
    UserController::register_routes(app);

    app.port(18080)
        .server_name("CrowCpp")
        .multithreaded()
        .run();

    return 0;

}
