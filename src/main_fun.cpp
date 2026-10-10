#include <iostream>
#include <limits>
#include "Admin.h"
#include "Faculty.h"
#include "Student.h"
#include "FileManager.h"
#include "Platform.h"
#include "Authentication.h"

using namespace std;

namespace {
int readChoice(const string& prompt) {
    int choice;
    while (true) {
        cout << prompt;
        if (cin >> choice) return choice;
        cout << "Invalid input. Please enter a number." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

}

int main() {
    FileManager::initializeFiles();
    Authentication::saveAccounts();
    Platform platform(1, "LeetCode", "https://leetcode.com",
                      "Coding and DSA practice platform");
    Faculty faculty("F001", "Varshitha", "varshitha@gmail.com", "varshithaF", "1234",
                   "9876543210", "CSE", "Professor");
    Student satya("S001", "Satya", "satya@gmail.com", "satyaS", "0987",
                 "9876543211", "CSE", 2);
    Student veeresh("S002", "Veeresh", "veeresh@gmail.com", "veereshS", "1122",
                   "9876543212", "CSE", 2);
    cout << "========================================\n";
    cout << "   CODING PLATFORM PERFORMANCE SYSTEM\n";
    cout << "========================================\n";

    bool running = true;
    while (running) {
        cout << "\n1. Login\n2. Exit\n";
        int menuChoice = readChoice("Enter choice: ");
        if (menuChoice == 2) {
            running = false;
            continue;
        }
        if (menuChoice != 1) {
            cout << "Invalid choice. Select 1 or 2." << endl;
            continue;
        }

        Authentication authentication;
        UserRole roleChoice = authentication.login();
        if (roleChoice == UserRole::None) continue;
        cout << "Login successful. Welcome "
             << Authentication::roleName(roleChoice) << "!" << endl;

        switch (roleChoice) {
        case UserRole::Admin: {
            faculty.refreshData();
            Admin admin("A001", "Tripura", "tripura@gmail.com", "tripuraA", "5678",
                        "9876543213", "Computer Science", "System Administrator");
            admin.displayAdminInfo();
            admin.runDashboard(faculty, satya, veeresh, platform);
            break;
        }
        case UserRole::Faculty:
            faculty.refreshData();
            faculty.displayFacultyInfo();
            faculty.displayDashboard();
            break;
        case UserRole::Student:
            if (authentication.getLoggedInUsername() == "satyaS") {
                satya.displayStudent();
                satya.displayDashboard();
            } else {
                veeresh.displayStudent();
                veeresh.displayDashboard();
            }
            break;
        default: break;
        }
    }

    cout << "\nProgram ended successfully.\nPress Enter to close..." << endl;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
    return 0;
}
