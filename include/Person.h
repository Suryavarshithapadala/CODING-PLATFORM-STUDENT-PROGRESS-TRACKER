//Person Class file as Person.h

#ifndef PERSON_H
#define PERSON_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Person {
protected:
    string id;
    string phone;
    string name, username, email, password;

public:
    Person();

    Person(string idd, string n, string m, string un, string pass, string p);

    void displayPerson();

    string getID();
    string getName();
    string getMailID();
    string getUsername();
    string getPassword();
    string getPhone();

    void setID(string id);
    void setName(string name);
    void setMailID(string email);
    void setUsername(string username);
    void setPassword(string password);
    void setPhone(string phone);

    static bool isValidEmail(const string& email);
    static bool isValidPhone(const string& phone);
    static string generateNextStudentId(const vector<string>& existingIds);

    virtual ~Person();

    virtual void displayDashboard();
};

#endif
