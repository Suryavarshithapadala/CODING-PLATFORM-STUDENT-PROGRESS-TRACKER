#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

#include <iostream>
#include <string>
using namespace std;

class Assignment {
private:
    int assignmentId;
    int studentId;
    int problemId;
    string status;

public:
    Assignment();
    Assignment(int aid, int sid, int pid, string s);

    int getAssignmentId();
    int getStudentId();
    int getProblemId();
    string getStatus();

    void setStatus(string status);
    void displayAssignment();
};

#endif
