#include "Submission.h"
#include <fstream>
#include <iostream>

using namespace std;

Submission::Submission(int sid, int pid, const string& text)
    : studentId(sid), problemId(pid), details(text) {}

bool Submission::save() const {
    ofstream file("Submissions.txt", ios::app);
    if (!file) return false;
    file << studentId << "|" << problemId << "|" << details << endl;
    return true;
}

void Submission::display() const {
    cout << "Student ID: " << studentId << "\nProblem ID: " << problemId
         << "\nSubmission: " << details << endl;
}
