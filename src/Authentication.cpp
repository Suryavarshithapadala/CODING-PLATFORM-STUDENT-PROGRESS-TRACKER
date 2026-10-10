#include "Authentication.h"
#include <iostream>
#include <fstream>

using namespace std;

UserRole Authentication::login() {
    string username, password;
    cout << "\n========== LOGIN ==========" << endl;
    cout << "Username: "; cin >> username;
    cout << "Password: "; cin >> password;
    if (username == "tripuraA" && password == "5678") {
        loggedInUsername = username;
        return UserRole::Admin;
    }
    if (username == "varshithaF" && password == "1234") {
        loggedInUsername = username;
        return UserRole::Faculty;
    }
    if ((username == "satyaS" && password == "0987") ||
        (username == "veereshS" && password == "1122")) {
        loggedInUsername = username;
        return UserRole::Student;
    }
    cout << "Invalid username or password." << endl;
    return UserRole::None;
}

const string& Authentication::getLoggedInUsername() const {
    return loggedInUsername;
}

void Authentication::saveAccounts() {
    ofstream file("Users.txt");
    if (!file) return;
    file << "Admin|201|Tripura|tripuraA|5678|Computer Science|System Administrator\n";
    file << "Faculty|501|Varshitha|varshithaF|1234|CSE|Professor\n";
    file << "Student|301|Satya|satyaS|0987|CSE|2\n";
    file << "Student|302|Veeresh|veereshS|1122|CSE|2\n";
}

string Authentication::roleName(UserRole role) {
    if (role == UserRole::Admin) return "Admin";
    if (role == UserRole::Faculty) return "Faculty";
    if (role == UserRole::Student) return "Student";
    return "Unknown";
}
