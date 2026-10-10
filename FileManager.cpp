#include "FileManager.h"

void FileManager::initializeFiles() {
    ofstream platformFile("Platforms.txt", ios::app);
    ofstream problemFile("Problems.txt", ios::app);
    ofstream assignmentFile("Assignments.txt", ios::app);
    ofstream progressFile("Progress.txt", ios::app);
    ofstream submissionFile("Submissions.txt", ios::app);

    platformFile.close();
    problemFile.close();
    assignmentFile.close();
    progressFile.close();
    submissionFile.close();
}

void FileManager::savePlatform(const Platform& platform) {

    ofstream file("Platforms.txt", ios::app);

    if (!file) {
        cout << "Error opening platforms file." << endl;
        return;
    }

    file << platform.getPlatformId() << "|"
         << platform.getPlatformName() << "|"
         << platform.getPlatformURL() << "|"
         << platform.getDescription() << endl;

    file.close();

    cout << "Platform saved successfully." << endl;
}

void FileManager::saveProblem(const Problem& problem) {

    ofstream file("Problems.txt", ios::app);

    if (!file) {
        cout << "Error opening problems file." << endl;
        return;
    }

    file << problem.getProblemId() << "|"
         << problem.getProblemName() << "|"
         << problem.getDifficulty() << "|"
         << problem.getPlatformId() << endl;

    file.close();

    cout << "Problem saved successfully." << endl;
}

void FileManager::loadPlatforms() {

    ifstream file("Platforms.txt");

    if (!file) {
        cout << "No platform data found." << endl;
        return;
    }

    string line;

    cout << "\n========= SAVED PLATFORMS =========" << endl;

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

void FileManager::loadProblems() {

    ifstream file("Problems.txt");

    if (!file) {
        cout << "No problem data found." << endl;
        return;
    }

    string line;

    cout << "\n========= SAVED PROBLEMS =========" << endl;

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}
