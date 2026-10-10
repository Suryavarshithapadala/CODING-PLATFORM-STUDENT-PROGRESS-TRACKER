#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include "Problem.h"
#include <string>
#include <vector>
using namespace std;

class Faculty;
class Student;
class Platform;

class Admin : public Person {
private:
    struct Member {
        string id;
        string name;
        string email;
        string username;
        string password;
        string phone;
        string departmentOrCourse;
        string designationOrYear;
    };

    string department;
    string role;
    static vector<Member> facultyMembers;
    static vector<Member> studentMembers;
    static vector<Problem> managedProblems;
    static bool recordsInitialized;

    void initializeRecords(Faculty& faculty, Student& satya, Student& veeresh);
    void manageFaculty();
    void manageStudents();
    void manageProblems(const Platform& platform);

public:
    Admin();
    Admin(string idd, string n, string m, string un, string pass, string p,
          string dept, string roleName);

    string getDepartment();
    string getRole();

    void setDepartment(string dept);
    void setRole(string roleName);

    void displayAdminInfo();
    void displayDashboard() override;
    void runDashboard(Faculty& faculty, Student& satya, Student& veeresh,
                      const Platform& platform);
};

#endif
