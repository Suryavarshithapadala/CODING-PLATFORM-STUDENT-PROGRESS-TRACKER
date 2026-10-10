#ifndef PROBLEM_H
#define PROBLEM_H

#include <iostream>
#include <string>
using namespace std;

class Problem {
private:
    int problemId;
    string problemName;
    string difficulty;
    int platformId;

public:
    Problem();

    Problem(int id, string name, string diff, int pId);

    void displayProblem() const;

    int getProblemId() const;
    string getProblemName() const;
    string getDifficulty() const;
    int getPlatformId() const;

    void setProblemId(int id);
    void setProblemName(string name);
    void setDifficulty(string diff);
    void setPlatformId(int pId);
};

#endif