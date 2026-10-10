#include "Progress.h"
#include <iostream>
using namespace std;

Progress::Progress() {
    studentId = 0;
    totalAssignments = 0;
    completedAssignments = 0;
    pendingAssignments = 0;
    progressPercentage = 0.0f;
}

Progress::Progress(int sid, int total, int completed)
    : studentId(sid),
      totalAssignments(total),
      completedAssignments(completed) {
    pendingAssignments = totalAssignments - completedAssignments;

    if (totalAssignments > 0) {
        progressPercentage = (completedAssignments * 100.0f) / totalAssignments;
    }
    else {
        progressPercentage = 0.0f;
    }
}

int Progress::getStudentId() {
    return studentId;
}

int Progress::getTotalAssignments() {
    return totalAssignments;
}

int Progress::getCompletedAssignments() {
    return completedAssignments;
}

int Progress::getPendingAssignments() {
    return pendingAssignments;
}

float Progress::getProgressPercentage() {
    return progressPercentage;
}

void Progress::displayProgress() {
    cout << "\n========== STUDENT PROGRESS ==========" << endl;
    cout << "Student ID: " << studentId << endl;
    cout << "Total Assignments      : " << totalAssignments << endl;
    cout << "Completed Assignments  : " << completedAssignments << endl;
    cout << "Pending Assignments    : " << pendingAssignments << endl;
    cout << "Progress Percentage    : " << progressPercentage << "%" << endl;
    cout << "======================================" << endl;
}
