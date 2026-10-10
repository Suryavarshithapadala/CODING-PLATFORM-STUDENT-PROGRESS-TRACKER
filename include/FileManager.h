#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>

#include "Platform.h"
#include "Problem.h"

using namespace std;

class FileManager {
public:

    // Creates data folder and required files
    static void initializeFiles();

    // Platform file operations
    static void savePlatform(const Platform& platform);
    static void loadPlatforms();

    // Problem file operations
    static void saveProblem(const Problem& problem);
    static void loadProblems();
};

#endif