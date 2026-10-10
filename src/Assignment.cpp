#include "Assignment.h"

Assignment::Assignment() {
    assignmentId = 0;
    studentId = 0;
    problemId = 0;
    status = "Not Assigned";
}

Assignment::Assignment(int aid, int sid, int pid, string s)
    : assignmentId(aid), studentId(sid), problemId(pid), status(s) {
}

int Assignment::getAssignmentId() {
    return assignmentId;
}

int Assignment::getStudentId() {
    return studentId;
}

int Assignment::getProblemId() {
    return problemId;
}

string Assignment::getStatus() {
    return status;
}

void Assignment::setStatus(string status) {
    this->status = status;
}

void Assignment::displayAssignment() {
    cout << "Assignment ID : " << assignmentId << "\n"
         << "Student ID    : " << studentId << "\n"
         << "Problem ID    : " << problemId << "\n"
         << "Status        : " << status << endl;
}
