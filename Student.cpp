#include "Student.h"
#include "Submission.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using namespace std;


// =====================================================
// DEFAULT CONSTRUCTOR
// =====================================================

Student::Student()
{
    course = "none";
    year = 0;
}


// =====================================================
// PARAMETERIZED CONSTRUCTOR
// =====================================================

Student::Student(string idd, string n, string m, string un,
                 string pass, string p, string crs, int yr)
    : Person(idd, n, m, un, pass, p)
{
    course = crs;
    year = yr;
}


// =====================================================
// GETTERS
// =====================================================

string Student::getCourse()
{
    return course;
}

int Student::getYear()
{
    return year;
}


// =====================================================
// SETTERS
// =====================================================

void Student::setCourse(string crs)
{
    if (!crs.empty())
    {
        course = crs;
    }
    else
    {
        cout << "No course selected." << endl;
    }
}

void Student::setYear(int yr)
{
    if (yr >= 1 && yr <= 4)
    {
        year = yr;
    }
    else
    {
        cout << "Invalid year. Year must be between 1 and 4."
             << endl;
    }
}


// =====================================================
// DISPLAY STUDENT
// =====================================================

void Student::displayStudent()
{
    displayPerson();

    cout << "Course : " << course << endl;
    cout << "Year   : " << year << endl;
}

bool Student::setAssignmentStatus(int selectedProblemId, const string& newStatus)
{
    ifstream readFile("Assignments.txt");
    ofstream writeFile("Assignments_temp.txt");
    if (!readFile || !writeFile) return false;

    int assignmentId, studentId, problemId;
    string status;
    bool updated = false;
    string currentStudentId = id;
    while (readFile >> assignmentId) {
        readFile.ignore();
        readFile >> studentId;
        readFile.ignore();
        readFile >> problemId;
        readFile.ignore();
        getline(readFile, status);
        if (to_string(studentId) == currentStudentId && problemId == selectedProblemId) {
            status = newStatus;
            updated = true;
        }
        writeFile << assignmentId << "|" << studentId << "|" << problemId
                  << "|" << status << endl;
    }
    readFile.close();
    writeFile.close();
    if (!updated || remove("Assignments.txt") != 0 ||
        rename("Assignments_temp.txt", "Assignments.txt") != 0) return false;
    return true;
}


// =====================================================
// VIEW ASSIGNED PROBLEMS
// =====================================================

void Student::viewAssignedProblems()
{
    ifstream assignmentFile("Assignments.txt");
    ifstream problemFile("Problems.txt");

    if (!assignmentFile)
    {
        cout << "\nAssignments.txt not found." << endl;
        return;
    }

    if (!problemFile)
    {
        cout << "\nProblems.txt not found." << endl;
        assignmentFile.close();
        return;
    }

    bool found = false;

    int assignmentId;
    int studentId;
    int problemId;
    string status;

    cout << "\n========================================" << endl;
    cout << "          ASSIGNED PROBLEMS             " << endl;
    cout << "========================================" << endl;

    while (assignmentFile >> assignmentId)
    {
        assignmentFile.ignore();

        assignmentFile >> studentId;
        assignmentFile.ignore();

        assignmentFile >> problemId;
        assignmentFile.ignore();

        getline(assignmentFile, status);

        // Only show assignments belonging to this student
        if (to_string(studentId) == id)
        {
            found = true;

            ifstream problemSearch("Problems.txt");

            int pid;
            string title;
            string difficulty;
            string platform;

            while (problemSearch >> pid)
            {
                problemSearch.ignore();

                getline(problemSearch, title, '|');
                getline(problemSearch, difficulty, '|');
                getline(problemSearch, platform);

                if (pid == problemId)
                {
                    cout << "\nAssignment ID : "
                         << assignmentId << endl;

                    cout << "Problem ID    : "
                         << pid << endl;

                    cout << "Title         : "
                         << title << endl;

                    cout << "Difficulty    : "
                         << difficulty << endl;

                    cout << "Platform      : "
                         << platform << endl;

                    cout << "Status        : "
                         << status << endl;

                    cout << "----------------------------------------"
                         << endl;

                    break;
                }
            }

            problemSearch.close();
        }
    }

    assignmentFile.close();
    problemFile.close();

    if (!found)
    {
        cout << "\nNo problems are assigned to you." << endl;
    }
}


// =====================================================
// UPDATE PROBLEM STATUS
// =====================================================

void Student::updateProblemStatus()
{
    ifstream inputFile("Assignments.txt");

    if (!inputFile)
    {
        cout << "\nAssignments.txt not found." << endl;
        return;
    }

    int selectedProblemId;

    cout << "\n========================================" << endl;
    cout << "        UPDATE PROBLEM STATUS           " << endl;
    cout << "========================================" << endl;

    cout << "Enter Problem ID: ";
    cin >> selectedProblemId;

    int assignmentId;
    int studentId;
    int problemId;
    string status;

    bool found = false;

    // First check whether this problem belongs to this student
    while (inputFile >> assignmentId)
    {
        inputFile.ignore();

        inputFile >> studentId;
        inputFile.ignore();

        inputFile >> problemId;
        inputFile.ignore();

        getline(inputFile, status);

        if (to_string(studentId) == id && problemId == selectedProblemId)
        {
            found = true;

            cout << "\nCurrent Status : "
                 << status << endl;

            break;
        }
    }

    inputFile.close();

    if (!found)
    {
        cout << "\nThis problem is not assigned to you."
             << endl;

        return;
    }

    cout << "\nSelect New Status" << endl;
    cout << "1. Pending" << endl;
    cout << "2. Completed" << endl;

    int choice;

    cout << "Enter choice: ";
    cin >> choice;

    string newStatus;

    if (choice == 1)
    {
        newStatus = "Pending";
    }
    else if (choice == 2)
    {
        newStatus = "Completed";
    }
    else
    {
        cout << "\nInvalid choice." << endl;
        return;
    }


    // Read original file
    ifstream readFile("Assignments.txt");

    // Temporary file
    ofstream writeFile("Assignments_temp.txt");

    if (!readFile || !writeFile)
    {
        cout << "\nUnable to update Assignments.txt."
             << endl;

        return;
    }

    while (readFile >> assignmentId)
    {
        readFile.ignore();

        readFile >> studentId;
        readFile.ignore();

        readFile >> problemId;
        readFile.ignore();

        getline(readFile, status);

        // Change only this student's assignment
        if (to_string(studentId) == id &&
            problemId == selectedProblemId)
        {
            status = newStatus;
        }

        // Keep exactly the same format as Faculty
        writeFile << assignmentId << "|"
                  << studentId << "|"
                  << problemId << "|"
                  << status << endl;
    }

    readFile.close();
    writeFile.close();

    // Replace original file
    remove("Assignments.txt");

    if (rename("Assignments_temp.txt",
               "Assignments.txt") != 0)
    {
        cout << "\nError replacing Assignments.txt."
             << endl;

        return;
    }

    cout << "\nProblem status updated successfully."
         << endl;

    cout << "New Status: "
         << newStatus << endl;
}


// =====================================================
// RECORD SUBMISSION
// =====================================================

void Student::recordSubmission()
{
    ifstream assignmentFile("Assignments.txt");

    if (!assignmentFile)
    {
        cout << "\nAssignments.txt not found." << endl;
        return;
    }

    int selectedProblemId;

    cout << "\n========================================" << endl;
    cout << "           RECORD SUBMISSION            " << endl;
    cout << "========================================" << endl;

    cout << "Enter Problem ID: ";
    cin >> selectedProblemId;

    int assignmentId;
    int studentId;
    int problemId;
    string status;

    bool found = false;

    while (assignmentFile >> assignmentId)
    {
        assignmentFile.ignore();

        assignmentFile >> studentId;
        assignmentFile.ignore();

        assignmentFile >> problemId;
        assignmentFile.ignore();

        getline(assignmentFile, status);

        if (to_string(studentId) == id &&
            problemId == selectedProblemId)
        {
            found = true;
            break;
        }
    }

    assignmentFile.close();

    if (!found)
    {
        cout << "\nThis problem is not assigned to you."
             << endl;

        return;
    }

    string submission;

    cin.ignore();

    cout << "Enter submission details: ";
    getline(cin, submission);

    if (submission.empty())
    {
        cout << "\nSubmission cannot be empty."
             << endl;

        return;
    }

    Submission newSubmission(stoi(id), selectedProblemId, submission);
    if (!newSubmission.save()) {
        cout << "\nUnable to save submission." << endl;
        return;
    }
    cout << "\nSubmission recorded successfully." << endl;
    if (setAssignmentStatus(selectedProblemId, "Completed")) {
        cout << "Assignment marked Completed; progress will update from this submission." << endl;
    }
}


// =====================================================
// VIEW PROGRESS
// =====================================================

void Student::viewProgress()
{
    ifstream assignmentFile("Assignments.txt");

    if (!assignmentFile)
    {
        cout << "\nAssignments.txt not found."
             << endl;

        return;
    }

    int assignmentId;
    int studentId;
    int problemId;
    string status;

    int totalAssignments = 0;
    int completedAssignments = 0;
    int pendingAssignments = 0;

    while (assignmentFile >> assignmentId)
    {
        assignmentFile.ignore();

        assignmentFile >> studentId;
        assignmentFile.ignore();

        assignmentFile >> problemId;
        assignmentFile.ignore();

        getline(assignmentFile, status);

        if (to_string(studentId) == id)
        {
            totalAssignments++;

            if (status == "Completed")
            {
                completedAssignments++;
            }
            else
            {
                pendingAssignments++;
            }
        }
    }

    assignmentFile.close();

    cout << "\n========================================" << endl;
    cout << "             MY PROGRESS                " << endl;
    cout << "========================================" << endl;

    if (totalAssignments == 0)
    {
        cout << "No assignments found." << endl;
        return;
    }

    float percentage =
        (completedAssignments * 100.0f)
        / totalAssignments;

    cout << "Student ID            : "
         << id << endl;

    cout << "Total Assignments     : "
         << totalAssignments << endl;

    cout << "Completed Assignments : "
         << completedAssignments << endl;

    cout << "Pending Assignments   : "
         << pendingAssignments << endl;

    cout << "Progress Percentage   : "
         << percentage << "%" << endl;

    cout << "========================================"
         << endl;
}


// =====================================================
// STUDENT DASHBOARD
// =====================================================

void Student::displayDashboard()
{
    int choice;

    do
    {
        cout << "\n========================================" << endl;
        cout << "          STUDENT DASHBOARD             " << endl;
        cout << "========================================" << endl;

        cout << "1. View Assigned Problems" << endl;
        cout << "2. Update Problem Status" << endl;
        cout << "3. Record Submission" << endl;
        cout << "4. View Progress" << endl;
        cout << "5. View Student Profile" << endl;
        cout << "6. Logout" << endl;

        cout << "========================================" << endl;

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                viewAssignedProblems();
                break;

            case 2:
                updateProblemStatus();
                break;

            case 3:
                recordSubmission();
                break;

            case 4:
                viewProgress();
                break;

            case 5:
                cout << "\n========== STUDENT PROFILE =========="
                     << endl;

                displayStudent();
                break;

            case 6:
                cout << "\nLogging out..."
                     << endl;

                break;

            default:
                cout << "\nInvalid choice."
                     << endl;
        }

    } while (choice != 6);
}
