#include "Admin.h"
#include "Faculty.h"
#include "Student.h"
#include "Platform.h"
#include <iostream>
#include <limits>
#include <algorithm>
using namespace std;

vector<Admin::Member> Admin::facultyMembers;
vector<Admin::Member> Admin::studentMembers;
vector<Problem> Admin::managedProblems;
bool Admin::recordsInitialized = false;

namespace {
int readNumber(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << "Invalid input. Please enter a number." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readText(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin >> ws, value);
    return value;
}

string readValidGmail(const string& prompt) {
    while (true) {
        string value = readText(prompt);
        if (Person::isValidEmail(value)) return value;
        cout << "Invalid email. Please use username@gmail.com." << endl;
    }
}

string readValidPhone(const string& prompt) {
    while (true) {
        string value = readText(prompt);
        if (Person::isValidPhone(value)) return value;
        cout << "Invalid phone number. Please enter exactly 10 digits." << endl;
    }
}

template <typename T>
typename vector<T>::iterator findById(vector<T>& records, const string& id) {
    return find_if(records.begin(), records.end(),
                   [id](const T& record) { return record.id == id; });
}
}

Admin::Admin() : Person() {
    department = "General";
    role = "Administrator";
}

Admin::Admin(string idd, string n, string m, string un, string pass, string p,
             string dept, string roleName)
    : Person(idd, n, m, un, pass, p), department(dept), role(roleName) {
}

string Admin::getDepartment() {
    return department;
}

string Admin::getRole() {
    return role;
}

void Admin::setDepartment(string dept) {
    department = dept;
}

void Admin::setRole(string roleName) {
    role = roleName;
}

void Admin::displayAdminInfo() {
    cout << "\n========== ADMIN INFORMATION ==========" << endl;
    cout << "ID         : " << id << endl;
    cout << "Name       : " << name << endl;
    cout << "Email      : " << email << endl;
    cout << "Username   : " << username << endl;
    cout << "Department : " << department << endl;
    cout << "Role       : " << role << endl;
    cout << "======================================" << endl;
}

void Admin::displayDashboard() {
    cout << "\n========== ADMIN DASHBOARD ==========" << endl;
    cout << "1. Manage Faculty" << endl;
    cout << "2. Manage Students" << endl;
    cout << "3. Manage Problems" << endl;
    cout << "4. View Faculty Reports" << endl;
    cout << "5. Logout" << endl;
    cout << "====================================" << endl;
}

void Admin::initializeRecords(Faculty& faculty, Student& satya, Student& veeresh) {
    if (recordsInitialized) return;

    facultyMembers.push_back({faculty.getID(), faculty.getName(), faculty.getMailID(),
                              faculty.getUsername(), faculty.getPhone(), "CSE", "Professor"});
    studentMembers.push_back({satya.getID(), satya.getName(), satya.getMailID(),
                              satya.getUsername(), satya.getPhone(), "CSE", "2"});
    studentMembers.push_back({veeresh.getID(), veeresh.getName(), veeresh.getMailID(),
                              veeresh.getUsername(), veeresh.getPhone(), "CSE", "2"});
    recordsInitialized = true;
}

void Admin::manageFaculty() {
    while (true) {
        cout << "\n--- FACULTY MANAGEMENT ---\n"
             << "1. View faculty\n2. Add faculty\n3. Edit faculty\n4. Delete faculty\n5. Back\n";
        int choice = readNumber("Enter choice: ");
        if (choice == 5) return;

        if (choice == 1) {
            if (facultyMembers.empty()) cout << "No faculty members found." << endl;
            for (const Member& member : facultyMembers)
                cout << "ID: " << member.id << " | " << member.name << " | "
                     << member.departmentOrCourse << " | " << member.designationOrYear << endl;
        } else if (choice == 2) {
            Member member;
            member.id = readText("Faculty ID: ");
            if (findById(facultyMembers, member.id) != facultyMembers.end()) {
                cout << "A faculty member with this ID already exists." << endl;
                continue;
            }
            member.name = readText("Name: ");
            member.email = readValidGmail("Email: ");
            member.username = readText("Username: ");
            member.password = readText("Password: ");
            member.phone = readValidPhone("Phone: ");
            member.departmentOrCourse = readText("Department: ");
            member.designationOrYear = readText("Designation: ");
            facultyMembers.push_back(member);
            cout << "Faculty member added." << endl;
        } else if (choice == 3) {
            string id = readText("Faculty ID to edit: ");
            auto it = findById(facultyMembers, id);
            if (it == facultyMembers.end()) { cout << "Faculty member not found." << endl; continue; }

            while (true) {
                cout << "\n--- UPDATE FACULTY ---\n"
                     << "1. Update Name\n2. Update Email\n3. Update Phone\n"
                     << "4. Update Username\n5. Update Password\n6. Update Department\n"
                     << "7. Update Designation\n8. Back\n";
                int updateChoice = readNumber("Select field to update: ");
                if (updateChoice == 8) break;
                switch (updateChoice) {
                    case 1: it->name = readText("New name: "); break;
                    case 2: it->email = readValidGmail("New email: "); break;
                    case 3: it->phone = readValidPhone("New phone: "); break;
                    case 4: it->username = readText("New username: "); break;
                    case 5: it->password = readText("New password: "); break;
                    case 6: it->departmentOrCourse = readText("New department: "); break;
                    case 7: it->designationOrYear = readText("New designation: "); break;
                    default: cout << "Invalid choice." << endl;
                }
            }
            cout << "Faculty member updated." << endl;
        } else if (choice == 4) {
            string id = readText("Faculty ID to delete: ");
            auto it = findById(facultyMembers, id);
            if (it == facultyMembers.end()) cout << "Faculty member not found." << endl;
            else { facultyMembers.erase(it); cout << "Faculty member deleted." << endl; }
        } else cout << "Invalid choice." << endl;
    }
}

void Admin::manageStudents() {
    while (true) {
        cout << "\n--- STUDENT MANAGEMENT ---\n"
             << "1. View students\n2. Add student\n3. Edit student\n4. Delete student\n5. Back\n";
        int choice = readNumber("Enter choice: ");
        if (choice == 5) return;

        if (choice == 1) {
            if (studentMembers.empty()) cout << "No students found." << endl;
            for (const Member& member : studentMembers)
                cout << "ID: " << member.id << " | " << member.name << " | "
                     << member.departmentOrCourse << " | Year " << member.designationOrYear << endl;
        } else if (choice == 2) {
            Member member;
            vector<string> existingStudentIds;
            for (const auto& existing : studentMembers) existingStudentIds.push_back(existing.id);
            member.id = Person::generateNextStudentId(existingStudentIds);
            if (findById(studentMembers, member.id) != studentMembers.end()) {
                cout << "A student with this ID already exists." << endl;
                continue;
            }
            member.name = readText("Name: ");
            member.email = readValidGmail("Email: ");
            member.username = readText("Username: ");
            member.password = readText("Password: ");
            member.phone = readValidPhone("Phone: ");
            member.departmentOrCourse = readText("Course: ");
            member.designationOrYear = readText("Year: ");
            studentMembers.push_back(member);
            cout << "Student added with ID: " << member.id << endl;
        } else if (choice == 3) {
            string id = readText("Student ID to edit: ");
            auto it = findById(studentMembers, id);
            if (it == studentMembers.end()) { cout << "Student not found." << endl; continue; }

            while (true) {
                cout << "\n--- UPDATE STUDENT ---\n"
                     << "1. Update Name\n2. Update Email\n3. Update Phone\n"
                     << "4. Update Username\n5. Update Password\n6. Update Course\n"
                     << "7. Update Year\n8. Back\n";
                int updateChoice = readNumber("Select field to update: ");
                if (updateChoice == 8) break;
                switch (updateChoice) {
                    case 1: it->name = readText("New name: "); break;
                    case 2: it->email = readValidGmail("New email: "); break;
                    case 3: it->phone = readValidPhone("New phone: "); break;
                    case 4: it->username = readText("New username: "); break;
                    case 5: it->password = readText("New password: "); break;
                    case 6: it->departmentOrCourse = readText("New course: "); break;
                    case 7: it->designationOrYear = readText("New year: "); break;
                    default: cout << "Invalid choice." << endl;
                }
            }
            cout << "Student updated." << endl;
        } else if (choice == 4) {
            string id = readText("Student ID to delete: ");
            auto it = findById(studentMembers, id);
            if (it == studentMembers.end()) cout << "Student not found." << endl;
            else { studentMembers.erase(it); cout << "Student deleted." << endl; }
        } else cout << "Invalid choice." << endl;
    }
}

void Admin::manageProblems(const Platform& platform) {
    while (true) {
        cout << "\n--- PROBLEM MANAGEMENT ---\n"
             << "1. View platform and problems\n2. Add problem\n3. Edit problem\n4. Delete problem\n5. Back\n";
        int choice = readNumber("Enter choice: ");
        if (choice == 5) return;

        if (choice == 1) {
            platform.displayPlatform();
            if (managedProblems.empty()) cout << "No admin-managed problems found." << endl;
            for (const Problem& problem : managedProblems) problem.displayProblem();
        } else if (choice == 2) {
            int id = readNumber("Problem ID: ");
            auto exists = find_if(managedProblems.begin(), managedProblems.end(),
                                  [id](const Problem& p) { return p.getProblemId() == id; });
            if (exists != managedProblems.end()) { cout << "A problem with this ID already exists." << endl; continue; }
            string name = readText("Problem name: ");
            string difficulty = readText("Difficulty: ");
            managedProblems.emplace_back(id, name, difficulty, platform.getPlatformId());
            cout << "Problem added." << endl;
        } else if (choice == 3) {
            int id = readNumber("Problem ID to edit: ");
            auto it = find_if(managedProblems.begin(), managedProblems.end(),
                              [id](const Problem& p) { return p.getProblemId() == id; });
            if (it == managedProblems.end()) { cout << "Problem not found." << endl; continue; }
            it->setProblemName(readText("New problem name: "));
            it->setDifficulty(readText("New difficulty: "));
            cout << "Problem updated." << endl;
        } else if (choice == 4) {
            int id = readNumber("Problem ID to delete: ");
            auto it = find_if(managedProblems.begin(), managedProblems.end(),
                              [id](const Problem& p) { return p.getProblemId() == id; });
            if (it == managedProblems.end()) cout << "Problem not found." << endl;
            else { managedProblems.erase(it); cout << "Problem deleted." << endl; }
        } else cout << "Invalid choice." << endl;
    }
}

void Admin::runDashboard(Faculty& faculty, Student& satya, Student& veeresh,
                         const Platform& platform) {
    initializeRecords(faculty, satya, veeresh);
    int choice = 0;

    while (choice != 5) {
        displayDashboard();
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            manageFaculty();
            break;
        case 2:
            manageStudents();
            break;
        case 3:
            manageProblems(platform);
            break;
        case 4:
            faculty.generateReport();
            break;
        case 5:
            cout << "Admin logged out." << endl;
            break;
        default:
            cout << "Invalid choice. Select 1 to 5." << endl;
        }
    }
}
