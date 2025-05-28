#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <soci/soci.h>
#include <soci/odbc/soci-odbc.h>

int main()
{
    const std::string connectionString =
        "DRIVER={ODBC Driver 17 for SQL Server};"
        "SERVER=PR-BACHLITZANA;"
        "DATABASE=database;"//your database 
        "UID=admin;"//your username
        "PWD=admin;";//your password

    try
    {
        std::cout << "Connecting with connection string:\n" << connectionString << std::endl;
        soci::session sql(soci::odbc, connectionString);
        std::cout << "Successfully connected to MS SQL Server database!" << std::endl;

        soci::rowset<soci::row> rs = (sql.prepare << "SELECT * FROM Table"); // your table
        for (const auto& r : rs)
        {
            for (std::size_t i = 0; i != r.size(); ++i)
            {
                std::cout << r.get_properties(i).get_name() << ": ";
                if (r.get_indicator(i) == soci::i_null)
                    std::cout << "NULL";
                else
                    switch (r.get_properties(i).get_data_type())
                    {
                    case soci::dt_string:
                        std::cout << r.get<std::string>(i);
                        break;
                    case soci::dt_integer:
                        std::cout << r.get<int>(i);
                        break;
                    case soci::dt_double:
                        std::cout << r.get<double>(i);
                        break;
                    case soci::dt_long_long:
                        std::cout << r.get<long long>(i);
                        break;
                    case soci::dt_unsigned_long_long:
                        std::cout << r.get<unsigned long long>(i);
                        break;
                    case soci::dt_date:
                        std::cout << r.get<std::tm>(i).tm_year + 1900 << "-"
                            << r.get<std::tm>(i).tm_mon + 1 << "-"
                            << r.get<std::tm>(i).tm_mday;
                        break;
                    default:
                        std::cout << "[unsupported type]";
                    }
                std::cout << "  ";
            }
            std::cout << std::endl;
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
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Standard Exception: An unexpected error occurred." << std::endl;
        std::cerr << "  Details: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
