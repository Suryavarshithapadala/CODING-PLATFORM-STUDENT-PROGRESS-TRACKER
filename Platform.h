#ifndef PLATFORM_H
#define PLATFORM_H

#include <iostream>
#include <string>
using namespace std;

class Platform {
private:
    int platformId;
    string platformName;
    string platformURL;
    string description;

public:
    Platform();

    Platform(int id, string name, string url, string desc);

    void displayPlatform() const;

    int getPlatformId() const;
    string getPlatformName() const;
    string getPlatformURL() const;
    string getDescription() const;

    void setPlatformId(int id);
    void setPlatformName(string name);
    void setPlatformURL(string url);
    void setDescription(string desc);
};

#endif