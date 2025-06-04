#include "User.h"
#include <cstdio>

User::User(
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
)
    : user_code(code), user_fname(fname), user_mname(mname), user_lname(lname),
    user_email(email), user_status(status), lang_code(lang), user_password(password),
    user_valperno(valperno), user_valpertype(valpertype), user_grctype(grctype),
    user_grcperno(grcperno), user_grcpertype(grcpertype), user_triesleft(triesleft),
    seco_code(seco)
{
    user_pswdupddate = parse_datetime(pswdupddate_str);
    user_insdate = parse_datetime(insdate_str);
}

User::User()
    : user_code(), user_fname(), user_mname(), user_lname(), user_email(),
    user_status(0), lang_code(), user_password(), user_pswdupddate({}),
    user_valperno(0), user_valpertype(), user_grctype(), user_grcperno(0),
    user_grcpertype(), user_triesleft(0), user_insdate({}), seco_code()
{
    // Optionally initialize std::tm members to zero
    std::memset(&user_pswdupddate, 0, sizeof(std::tm));
    std::memset(&user_insdate, 0, sizeof(std::tm));
}

std::tm User::parse_datetime(const std::string& dt) {
    std::tm tm = {};
    sscanf_s(dt.c_str(), "%d-%d-%d %d:%d:%d",
        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
        &tm.tm_hour, &tm.tm_min, &tm.tm_sec);
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    return tm;
}
