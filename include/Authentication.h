#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include <string>

enum class UserRole { None, Admin, Faculty, Student };

class Authentication {
    std::string loggedInUsername;
public:
    UserRole login();
    const std::string& getLoggedInUsername() const;
    static void saveAccounts();
    static std::string roleName(UserRole role);
};

#endif
