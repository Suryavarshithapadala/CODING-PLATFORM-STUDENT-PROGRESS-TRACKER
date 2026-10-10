#ifndef SUBMISSION_H
#define SUBMISSION_H

#include <string>

class Submission {
    int studentId, problemId;
    std::string details;
public:
    Submission(int studentId, int problemId, const std::string& details);
    bool save() const;
    void display() const;
};

#endif
