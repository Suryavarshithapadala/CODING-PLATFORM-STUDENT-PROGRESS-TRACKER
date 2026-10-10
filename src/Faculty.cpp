#include "Faculty.h"
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

Faculty::Faculty(string id, string name, string email,
                 string username, string password,
                 string phone, string dept, string des)
    : Person(id, name, email, username, password, phone),
      department(dept),
      designation(des),
      problemCount(0),
      assignmentCount(0),
      progressCount(0) {
    loadAllData();
}

void Faculty::displayFacultyInfo() {
    cout << "\n========== FACULTY INFORMATION ==========" << endl;
    cout << "ID           : " << id << endl;
    cout << "Name         : " << name << endl;
    cout << "Mail ID      : " << email << endl;
    cout << "Username     : " << username << endl;
    cout << "Phone        : " << phone << endl;
    cout << "Department   : " << department << endl;
    cout << "Designation  : " << designation << endl;
    cout << "=========================================" << endl;
}

void Faculty::displayDashboard() {
    int choice = 0;
    while (choice != 9) {
        cout << "\n========== FACULTY DASHBOARD ==========" << endl;
        cout << "1. Add Problem\n2. View Problems\n3. Assign Problem\n4. View Assignments" << endl;
        cout << "5. Update Assignment Status\n6. View Student Progress\n7. Generate Report" << endl;
        cout << "8. Save Data\n9. Logout" << endl;
        cout << "========================================" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        switch (choice) {
        case 1: cout << "\nAccess Denied! Only Admin can add problems." << endl; break;
        case 2: viewProblems(); break;
        case 3: assignProblem(); break;
        case 4: viewAssignments(); break;
        case 5: updateAssignmentStatus(); break;
        case 6: viewStudentProgress(); break;
        case 7: generateReport(); break;
        case 8: saveAllData(); break;
        case 9: saveAllData(); cout << "Faculty logged out." << endl; break;
        default: cout << "Invalid choice. Select 1 to 9." << endl;
        }
    }
}

void Faculty::addProblem() {
    if (problemCount >= 10) {
        cout << "\nProblem storage is full!" << endl;
        return;
    }

    string title, difficulty;
    int platformId;
    int problemId = 100;
    for (int i = 0; i < problemCount; i++) {
        if (problems[i].getProblemId() > problemId) {
            problemId = problems[i].getProblemId();
        }
    }
    ++problemId;

    cout << "\n========== ADD PROBLEM ==========" << endl;
    cout << "Generated Problem ID: " << problemId << endl;

    cout << "Enter Problem Title: ";
    cin.ignore();
    getline(cin, title);

    cout << "Enter Difficulty: ";
    getline(cin, difficulty);

    cout << "Enter Platform ID: ";
    cin >> platformId;

    problems[problemCount] = Problem(problemId, title, difficulty, platformId);
    problemCount++;

    cout << "\nProblem Added Successfully!" << endl;
}

void Faculty::viewProblems() {
    cout << "\n========== VIEW PROBLEMS ==========" << endl;
    if (problemCount == 0) {
        cout << "No problems available." << endl;
        return;
    }

    for (int i = 0; i < problemCount; i++) {
        cout << "\nProblem " << i + 1 << endl;
        cout << "-------------------------" << endl;
        problems[i].displayProblem();
    }
}

bool Faculty::problemExists(int problemId) {
    for (int i = 0; i < problemCount; i++) {
        if (problems[i].getProblemId() == problemId) {
            return true;
        }
    }
    return false;
}

void Faculty::assignProblem() {
    if (assignmentCount >= 10) {
        cout << "\nAssignment storage is full!" << endl;
        return;
    }

    int studentId, problemId;
    int assignmentId = 1001 + assignmentCount;

    cout << "\n========== ASSIGN PROBLEM ==========" << endl;
    cout << "Generated Assignment ID: " << assignmentId << endl;

    cout << "Enter Student ID: ";
    cin >> studentId;

    cout << "Enter Problem ID: ";
    cin >> problemId;

    if (!problemExists(problemId)) {
        cout << "\nProblem ID not found!" << endl;
        return;
    }

    assignments[assignmentCount] = Assignment(assignmentId, studentId, problemId, "Assigned");
    assignmentCount++;

    cout << "\nProblem Assigned Successfully!" << endl;
    assignments[assignmentCount - 1].displayAssignment();
}

void Faculty::viewAssignments() {
    cout << "\n========== VIEW ASSIGNMENTS ==========" << endl;
    if (assignmentCount == 0) {
        cout << "No assignments available." << endl;
        return;
    }

    for (int i = 0; i < assignmentCount; i++) {
        cout << "\nAssignment " << i + 1 << endl;
        cout << "-------------------------" << endl;
        assignments[i].displayAssignment();
    }
}

void Faculty::updateAssignmentStatus() {
    if (assignmentCount == 0) {
        cout << "\nNo assignments available!" << endl;
        return;
    }

    int studentId, problemId;
    string status;

    cout << "\n========== UPDATE ASSIGNMENT STATUS ==========" << endl;
    cout << "Enter Student ID: ";
    cin >> studentId;

    cout << "Enter Problem ID: ";
    cin >> problemId;

    cout << "Enter Status (Completed/Pending): ";
    cin >> status;

    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].getStudentId() == studentId && assignments[i].getProblemId() == problemId) {
            if (status == "Completed" || status == "completed") {
                assignments[i].setStatus("Completed");
                cout << "\nAssignment marked as Completed!" << endl;
            }
            else if (status == "Pending" || status == "pending") {
                assignments[i].setStatus("Pending");
                cout << "\nAssignment marked as Pending!" << endl;
            }
            else {
                cout << "\nInvalid status! Use Completed or Pending." << endl;
                return;
            }
            return;
        }
    }

    cout << "\nAssignment not found!" << endl;
}

void Faculty::viewStudentProgress() {
    if (assignmentCount == 0) {
        cout << "\nNo assignments available!" << endl;
        return;
    }

    int studentId;
    cout << "\n========== VIEW STUDENT PROGRESS ==========" << endl;
    cout << "Enter Student ID: ";
    cin >> studentId;

    int total = 0;
    int completed = 0;

    for (int i = 0; i < assignmentCount; i++) {
        if (assignments[i].getStudentId() == studentId) {
            total++;
            if (assignments[i].getStatus() == "Completed") {
                completed++;
            }
        }
    }

    if (total == 0) {
        cout << "\nNo assignments found for Student ID " << studentId << "." << endl;
        return;
    }

    Progress newProgress(studentId, total, completed);

    for (int i = 0; i < progressCount; i++) {
        if (progressRecords[i].getStudentId() == studentId) {
            progressRecords[i] = newProgress;
            cout << "\nStudent Progress Updated Successfully!" << endl;
            progressRecords[i].displayProgress();
            return;
        }
    }

    if (progressCount >= 10) {
        cout << "\nProgress storage is full!" << endl;
        return;
    }

    progressRecords[progressCount] = newProgress;
    progressCount++;

    cout << "\nStudent Progress Calculated Successfully!" << endl;
    progressRecords[progressCount - 1].displayProgress();
}

void Faculty::generateReport() {
    if (progressCount == 0) {
        cout << "\nNo student progress records available!" << endl;
        return;
    }

    Report report;
    for (int i = 0; i < progressCount; i++) {
        report.displayStudentReport(progressRecords[i]);
    }
}

void Faculty::saveProblems() {
    ofstream file("Problems.txt");
    if (!file) {
        cout << "\nError opening Problems.txt!" << endl;
        return;
    }

    for (int i = 0; i < problemCount; i++) {
        file << problems[i].getProblemId() << "|"
             << problems[i].getProblemName() << "|"
             << problems[i].getDifficulty() << "|"
             << problems[i].getPlatformId() << endl;
    }
    file.close();
    cout << "\nProblems saved successfully!" << endl;
}

void Faculty::saveAssignments() {
    ofstream file("Assignments.txt");
    if (!file) {
        cout << "\nError opening Assignments.txt!" << endl;
        return;
    }

    for (int i = 0; i < assignmentCount; i++) {
        file << assignments[i].getAssignmentId() << "|"
             << assignments[i].getStudentId() << "|"
             << assignments[i].getProblemId() << "|"
             << assignments[i].getStatus() << endl;
    }
    file.close();
    cout << "\nAssignments saved successfully!" << endl;
}

void Faculty::saveProgress() {
    ofstream file("Progress.txt");
    if (!file) {
        cout << "\nError opening Progress.txt!" << endl;
        return;
    }

    for (int i = 0; i < progressCount; i++) {
        file << progressRecords[i].getStudentId() << "|"
             << progressRecords[i].getTotalAssignments() << "|"
             << progressRecords[i].getCompletedAssignments() << "|"
             << progressRecords[i].getPendingAssignments() << "|"
             << progressRecords[i].getProgressPercentage() << endl;
    }
    file.close();
    cout << "\nProgress saved successfully!" << endl;
}

void Faculty::loadProblems() {
    ifstream file("Problems.txt");
    if (!file) return;

    string line;
    while (getline(file, line) && problemCount < 10) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, name, diff, platformIdStr;
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, diff, '|');
        getline(ss, platformIdStr, '|');

        problems[problemCount] = Problem(stoi(idStr), name, diff, stoi(platformIdStr));
        problemCount++;
    }
    file.close();
}

void Faculty::loadAssignments() {
    ifstream file("Assignments.txt");
    if (!file) return;

    string line;
    while (getline(file, line) && assignmentCount < 10) {
        if (line.empty()) continue;
        stringstream ss(line);
        string aIdStr, sIdStr, pIdStr, status;
        getline(ss, aIdStr, '|');
        getline(ss, sIdStr, '|');
        getline(ss, pIdStr, '|');
        getline(ss, status, '|');

        assignments[assignmentCount] = Assignment(stoi(aIdStr), stoi(sIdStr), stoi(pIdStr), status);
        assignmentCount++;
    }
    file.close();
}

void Faculty::loadProgress() {
    ifstream file("Progress.txt");
    if (!file) return;

    string line;
    while (getline(file, line) && progressCount < 10) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sIdStr, totalStr, completedStr, pendingStr, percentStr;
        getline(ss, sIdStr, '|');
        getline(ss, totalStr, '|');
        getline(ss, completedStr, '|');
        getline(ss, pendingStr, '|');
        getline(ss, percentStr, '|');

        Progress p(stoi(sIdStr), stoi(totalStr), stoi(completedStr));
        progressRecords[progressCount] = p;
        progressCount++;
    }
    file.close();
}

void Faculty::saveAllData() {
    saveProblems();
    saveAssignments();
    saveProgress();
    cout << "\n====================================" << endl;
    cout << "All data saved successfully!" << endl;
    cout << "====================================" << endl;
}

void Faculty::loadAllData() {
    loadProblems();
    loadAssignments();
    loadProgress();
}

void Faculty::refreshData() {
    problemCount = 0;
    assignmentCount = 0;
    progressCount = 0;
    loadAllData();
}
