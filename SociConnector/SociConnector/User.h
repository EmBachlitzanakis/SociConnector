#pragma once
#include <string>
#include <ctime>

class User {
public:
    std::string user_code;
    std::string user_fname;
    std::string user_mname;
    std::string user_lname;
    std::string user_email;
    int user_status;
    std::string lang_code;
    std::string user_password;
    std::tm user_pswdupddate;
    int user_valperno;
    std::string user_valpertype;
    std::string user_grctype;
    int user_grcperno;
    std::string user_grcpertype;
    int user_triesleft;
    std::tm user_insdate;
    std::string seco_code;

    User(
        const std::string& code,
        const std::string& fname,
        const std::string& mname,
        const std::string& lname,
        const std::string& email,
        int status,
        const std::string& lang,
        const std::string& password,
        const std::string& pswdupddate_str,
        int valperno,
        const std::string& valpertype,
        const std::string& grctype,
        int grcperno,
        const std::string& grcpertype,
        int triesleft,
        const std::string& insdate_str,
        const std::string& seco
    );

    User(); // Default constructor

    static std::tm parse_datetime(const std::string& dt);
};

