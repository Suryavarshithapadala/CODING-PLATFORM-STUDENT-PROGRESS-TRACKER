//Person class file as Person.cpp

#include "Person.h"
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>

Person::Person() {
    id = "A001";
    phone = "9876543210";
    name = "Bunny";
    username = "BunnyTheCoder";
    password = "Bunny0192";
    email = "bunny@gmail.com";
}

Person::Person(string idd, string n, string m, string un, string pass, string p)
    : id(idd), phone(p), name(n), username(un), email(m), password(pass) {
}

bool Person::isValidEmail(const string& email) {
    if (email.empty()) return false;
    size_t atPos = email.find('@');
    size_t dotPos = email.find_last_of('.');
    if (atPos == string::npos || atPos == 0 || atPos + 1 >= email.size()) return false;
    if (dotPos == string::npos || dotPos <= atPos + 1 || dotPos == email.size() - 1) return false;
    string domain = email.substr(atPos + 1);
    if (domain != "gmail.com") return false;
    for (char ch : email) {
        if (ch == '@' || ch == '.') continue;
        if (!(isalnum(static_cast<unsigned char>(ch)) || ch == '_' || ch == '-')) {
            return false;
        }
    }
    return true;
}

bool Person::isValidPhone(const string& phone) {
    if (phone.size() != 10) return false;
    for (char ch : phone) {
        if (!isdigit(static_cast<unsigned char>(ch))) return false;
    }
    return true;
}

string Person::generateNextStudentId(const vector<string>& existingIds) {
    int maxId = 0;
    for (const string& existingId : existingIds) {
        if (existingId.size() >= 2 && existingId[0] == 'S') {
            string numberPart = existingId.substr(1);
            if (!numberPart.empty()) {
                int val = 0;
                try {
                    val = stoi(numberPart);
                } catch (...) {
                    val = 0;
                }
                maxId = max(maxId, val);
            }
        }
    }
    ostringstream oss;
    oss << "S" << setw(3) << setfill('0') << (maxId + 1);
    return oss.str();
}

void Person::displayPerson() {
    cout << "ID         : " << id << endl
         << "Name       : " << name << endl
         << "Mail ID    : " << email << endl
         << "Username   : " << username << endl
         << "Phone      : " << phone << endl;
}

string Person::getID() {
    return id;
}

string Person::getName() {
    return name;
}

string Person::getMailID() {
    return email;
}

string Person::getUsername() {
    return username;
}

string Person::getPassword() {
    return password;
}

string Person::getPhone() {
    return phone;
}

void Person::setID(string id) {
    this->id = id;
}

void Person::setName(string name) {
    this->name = name;
}

void Person::setMailID(string email) {
    if (!isValidEmail(email)) {
        cout << "Invalid email! Use username@gmail.com format." << endl;
        return;
    }
    this->email = email;
}

void Person::setUsername(string username) {
    this->username = username;
}

void Person::setPassword(string password) {
    this->password = password;
}

void Person::setPhone(string phone) {
    if (!isValidPhone(phone)) {
        cout << "Invalid phone number! Use exactly 10 digits." << endl;
        return;
    }
    this->phone = phone;
}

Person::~Person() {
    cout << "------------Destructor---------------" << endl;
}

void Person::displayDashboard() {
    cout << "Displaying Base Person Dashboard..." << endl;
}
