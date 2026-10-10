#include "Platform.h"

Platform::Platform() {
    platformId = 1;
    platformName = "LeetCode";
    platformURL = "https://leetcode.com";
    description = "Coding and DSA practice platform";
}

Platform::Platform(int id, string name, string url, string desc)
    : platformId(id),
      platformName(name),
      platformURL(url),
      description(desc) {
}

void Platform::displayPlatform() const {
    cout << "Platform ID   : " << platformId << endl;
    cout << "Platform Name : " << platformName << endl;
    cout << "Platform URL  : " << platformURL << endl;
    cout << "Description   : " << description << endl;
}

int Platform::getPlatformId() const {
    return platformId;
}

string Platform::getPlatformName() const {
    return platformName;
}

string Platform::getPlatformURL() const {
    return platformURL;
}

string Platform::getDescription() const {
    return description;
}

void Platform::setPlatformId(int id) {
    platformId = id;
}

void Platform::setPlatformName(string name) {
    platformName = name;
}

void Platform::setPlatformURL(string url) {
    platformURL = url;
}

void Platform::setDescription(string desc) {
    description = desc;
}