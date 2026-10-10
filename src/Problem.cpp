#include "Problem.h"

Problem::Problem() {
    problemId = 101;
    problemName = "Two Sum";
    difficulty = "Easy";
    platformId = 1;
}

Problem::Problem(int id, string name, string diff, int pId)
    : problemId(id),
      problemName(name),
      difficulty(diff),
      platformId(pId) {
}

void Problem::displayProblem() const {
    cout << "Problem ID   : " << problemId << endl;
    cout << "Problem Name : " << problemName << endl;
    cout << "Difficulty   : " << difficulty << endl;
    cout << "Platform ID  : " << platformId << endl;
}

int Problem::getProblemId() const {
    return problemId;
}

string Problem::getProblemName() const {
    return problemName;
}

string Problem::getDifficulty() const {
    return difficulty;
}

int Problem::getPlatformId() const {
    return platformId;
}

void Problem::setProblemId(int id) {
    problemId = id;
}

void Problem::setProblemName(string name) {
    problemName = name;
}

void Problem::setDifficulty(string diff) {
    difficulty = diff;
}

void Problem::setPlatformId(int pId) {
    platformId = pId;
}