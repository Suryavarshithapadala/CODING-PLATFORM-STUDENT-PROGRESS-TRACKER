#ifndef FACULTY_H
#define FACULTY_H

#include "Person.h"
#include "Problem.h"
#include "Assignment.h"
#include "Progress.h"
#include "Report.h"
#include <iostream>
#include <string>

using namespace std;

// ================= FACULTY CLASS =================

class Faculty : public Person {

private:
    string department;
    string designation;

    Problem problems[10];
    int problemCount;

    Assignment assignments[10];
    int assignmentCount;

    Progress progressRecords[10];
    int progressCount;

public:

    Faculty(string id, string name, string email,
            string username, string password,
            string phone, string dept, string des);

    void displayFacultyInfo();

    void displayDashboard() override;

    void addProblem();

    void viewProblems();

    bool problemExists(int problemId);

    void assignProblem();

    void viewAssignments();

    void updateAssignmentStatus();

    void viewStudentProgress();

    void generateReport();

    void saveProblems();

    void saveAssignments();

    void saveProgress();

    void loadProblems();

    void loadAssignments();

    void loadProgress();

    void saveAllData();

    void loadAllData();
    void refreshData();
};

#endif
