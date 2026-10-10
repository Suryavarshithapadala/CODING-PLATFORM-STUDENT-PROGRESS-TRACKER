#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include <iostream>
#include <string>

using namespace std;

class Student : public Person
{
private:
    string course;
    int year;
    bool setAssignmentStatus(int problemId, const string& newStatus);

public:

    // Default constructor
    Student();

    // Parameterized constructor
    Student(string idd, string n, string m, string un,
            string pass, string p, string crs, int yr);

    // Getters
    string getCourse();
    int getYear();

    // Setters
    void setCourse(string crs);
    void setYear(int yr);

    // Display student details
    void displayStudent();

    // Student functionalities
    void viewAssignedProblems();
    void updateProblemStatus();
    void recordSubmission();
    void viewProgress();

    // Dashboard
    void displayDashboard() override;
};

#endif
