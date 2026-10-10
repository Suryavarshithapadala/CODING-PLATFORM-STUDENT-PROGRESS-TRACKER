#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <ctime>

using namespace std;
// ============================================================
// UTILITY HELPERS
// ============================================================

string intToString(int value)
{
    ostringstream oss;
    oss << value;
    return oss.str();
}

string toLowerStr(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        s[i] = (char)tolower((unsigned char)s[i]);
    return s;
}

string toUpperStr(string s)
{
    for (size_t i = 0; i < s.size(); i++)
        s[i] = (char)toupper((unsigned char)s[i]);
    return s;
}

string trim(const string &s)
{
    size_t start = 0;
    while (start < s.size() && isspace((unsigned char)s[start])) start++;
    size_t end = s.size();
    while (end > start && isspace((unsigned char)s[end - 1])) end--;
    return s.substr(start, end - start);
}

bool startsWith(const string &text, const string &prefix)
{
    return text.size() >= prefix.size() && text.compare(0, prefix.size(), prefix) == 0;
}

bool containsIgnoreCase(const string &text, const string &keyword)
{
    return toLowerStr(text).find(toLowerStr(keyword)) != string::npos;
}

string shorten(const string &s, size_t maxLen)
{
    if (s.size() <= maxLen) return s;
    return s.substr(0, maxLen - 2) + "..";
}

bool isValidEmail(const string &e)
{
    size_t at = e.find('@');
    if (at == string::npos || at == 0 || at != e.rfind('@')) return false;
    if (toLowerStr(e.substr(at + 1)) != "gmail.com") return false;
    string local = e.substr(0, at);
    if (local.size() > 64 || local.front() == '.' || local.back() == '.' || local.find("..") != string::npos) return false;
    for (char ch : local)
        if (!(isalnum((unsigned char)ch) || ch == '.' || ch == '_' || ch == '+' || ch == '-')) return false;
    return true;
}

bool isValidPhone(const string &phone)
{
    if (phone.size() != 10 || phone[0] < '6' || phone[0] > '9') return false;
    for (char ch : phone) if (!isdigit((unsigned char)ch)) return false;
    return true;
}

bool isValidURL(const string &u)
{
    return (startsWith(u, "http://") || startsWith(u, "https://"))
           && u.size() > 10 && u.find(' ') == string::npos;
}

bool isValidDate(const string &d)   // format YYYY-MM-DD
{
    if (d.size() != 10 || d[4] != '-' || d[7] != '-') return false;
    for (int i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7) continue;
        if (!isdigit((unsigned char)d[i])) return false;
    }
    int month = atoi(d.substr(5, 2).c_str());
    int day = atoi(d.substr(8, 2).c_str());
    return month >= 1 && month <= 12 && day >= 1 && day <= 31;
}

string todayDate()
{
    time_t now = time(0);
    struct tm *t = localtime(&now);
    char buffer[16];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", t);
    return string(buffer);
}

bool fileHasData(const char *filename)
{
    ifstream file(filename);
    return file && file.peek() != ifstream::traits_type::eof();
}


// ============================================================
// INPUT HELPERS (validation)
// ============================================================

int getInteger(string message)
{
    int value;
    while (true)
    {
        cout << message;
        if (cin >> value)
        {
            cin.ignore(1000, '\n');
            return value;
        }
        if (cin.eof()) { cout << "\nInput closed. Exiting." << endl; exit(0); }
        cout << "Invalid input. Enter a number." << endl;
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

int getIntegerInRange(string message, int lo, int hi)
{
    while (true)
    {
        int value = getInteger(message);
        if (value >= lo && value <= hi) return value;
        cout << "Please enter a value between " << lo << " and " << hi << "." << endl;
    }
}

double getDoubleInRange(string message, double lo, double hi)
{
    double value;
    while (true)
    {
        cout << message;
        if (cin >> value)
        {
            cin.ignore(1000, '\n');
            if (value >= lo && value <= hi) return value;
            cout << "Please enter a value between " << lo << " and " << hi << "." << endl;
        }
        else
        {
            if (cin.eof()) { cout << "\nInput closed. Exiting." << endl; exit(0); }
            cout << "Invalid input. Enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
        }
    }
}

// Reads a line, trims it and replaces '|' (our file separator) with '/'.
string getString(string message)
{
    string value;
    cout << message;
    if (!getline(cin, value))
    {
        cout << "\nInput closed. Exiting." << endl;
        exit(0);
    }
    value = trim(value);
    for (size_t i = 0; i < value.size(); i++)
        if (value[i] == '|') value[i] = '/';
    return value;
}

string getNonEmptyString(string message)
{
    while (true)
    {
        string value = getString(message);
        if (!value.empty()) return value;
        cout << "This field cannot be empty." << endl;
    }
}

string getValidEmail(string message)
{
    while (true)
    {
        string value = getString(message);
        if (isValidEmail(value)) return value;
        cout << "Invalid email format (example: name@gmail.com)." << endl;
    }
}

string getValidPhone(string message)
{
    while (true)
    {
        string value = getString(message);
        if (isValidPhone(value)) return value;
        cout << "Invalid phone. Enter a 10-digit Indian mobile number starting with 6-9." << endl;
    }
}

string getValidDate(string message, bool notInPast)
{
    while (true)
    {
        string value = getString(message);
        if (!isValidDate(value))
        {
            cout << "Invalid date. Use the format YYYY-MM-DD." << endl;
            continue;
        }
        if (notInPast && value < todayDate())
        {
            cout << "Date cannot be in the past (today is " << todayDate() << ")." << endl;
            continue;
        }
        return value;
    }
}

// Returns "" if the user types 'cancel'.
string getValidURL(string message, const string &requiredPrefix)
{
    while (true)
    {
        string url = getString(message);
        if (toLowerStr(url) == "cancel") return "";
        if (!isValidURL(url))
        {
            cout << "Invalid link. It must start with http:// or https:// (or type 'cancel')." << endl;
            continue;
        }
        if (!requiredPrefix.empty() && !startsWith(url, requiredPrefix))
        {
            cout << "Link must belong to " << requiredPrefix << " (or type 'cancel')." << endl;
            continue;
        }
        return url;
    }
}

bool confirmAction(string message)
{
    string answer = toLowerStr(getString(message + " (y/n): "));
    return answer == "y" || answer == "yes";
}


// ============================================================
// PERSON - ABSTRACT BASE CLASS
// ============================================================

class Person
{
protected:
    string name;
    string email;
    string phone;

public:
    Person(string n = "", string e = "", string p = "")
    {
        name = n;
        email = e;
        phone = p;
    }

    virtual void display() const = 0;

    string getName() const { return name; }
    string getEmail() const { return email; }
    string getPhone() const { return phone; }
    void setEmail(string e) { email = e; }

    virtual ~Person() {}
};


// ============================================================
// TIMETABLE  (the university runs 9 timetables, numbered 1 - 9)
// ============================================================

class Timetable
{
public:
    enum { COUNT = 9 };

    static bool isValid(int no) { return no >= 1 && no <= COUNT; }

    static string timings(int no)
    {
        static const char *slots[COUNT] = {
            "8:30 AM - 3:20 PM",  "8:30 AM - 3:20 PM",  "9:00 AM - 4:00 PM",
            "9:00 AM - 4:00 PM",  "9:30 AM - 4:20 PM",  "9:30 AM - 4:20 PM",
            "10:00 AM - 5:00 PM", "10:00 AM - 5:00 PM", "11:00 AM - 6:00 PM"
        };
        if (!isValid(no)) return "Not assigned";
        return slots[no - 1];
    }

    static string labDay(int no)
    {
        static const char *days[COUNT] = {
            "Monday", "Tuesday", "Wednesday", "Thursday", "Friday",
            "Monday", "Tuesday", "Wednesday", "Thursday"
        };
        if (!isValid(no)) return "-";
        return days[no - 1];
    }

    static string describe(int no)
    {
        if (!isValid(no)) return "Not assigned";
        return "Timetable " + intToString(no) + " (" + timings(no) +
               ", Lab: " + labDay(no) + ")";
    }

    static void printAll()
    {
        cout << "\n--- University Timetables ---" << endl;
        cout << setw(6) << "No." << setw(22) << "Timings" << "Lab Day" << endl;
        cout << string(42, '-') << endl;
        for (int i = 1; i <= COUNT; i++)
            cout << setw(6) << i << setw(22) << timings(i) << labDay(i) << endl;
    }
};


// ============================================================
// STUDENT CLASS
// ============================================================

class Student : public Person
{
protected:
    string studentID;
    int year;
    string department;
    int timetableNo;     // 1 - 9
    double cgpa;         // 0.00 - 10.00

public:
    Student(string n = "", string id = "", string e = "",
            int y = 1, string dpt = "", int tt = 1, double g = 0.0, string p = "")
        : Person(n, e, p)
    {
        studentID = id;
        year = y;
        department = dpt;
        timetableNo = tt;
        cgpa = g;
    }

    virtual void display() const
    {
        cout << "\n--- Student Details ---" << endl;
        cout << "Name       : " << name << endl;
        cout << "Roll No : " << studentID << endl;
        cout << "Email      : " << email << endl;
        cout << "Phone      : " << (phone.empty() ? "Not provided" : phone) << endl;
        cout << "Year       : " << year << endl;
        cout << "Department : " << department << endl;
        cout << "CGPA       : " << cgpa << endl;
        cout << "Timetable  : " << Timetable::describe(timetableNo) << endl;
    }

    void updateYear(int newYear)
    {
        if (newYear < 1 || newYear > 4)
            throw invalid_argument("Year must be between 1 and 4.");
        year = newYear;
    }

    void updateEmail(string newEmail)
    {
        if (!isValidEmail(newEmail))
            throw invalid_argument("Invalid email.");
        email = newEmail;
    }

    void updateTimetable(int newNo)
    {
        if (!Timetable::isValid(newNo))
            throw invalid_argument("Timetable number must be between 1 and 9.");
        timetableNo = newNo;
    }

    void updateCGPA(double newCGPA)
    {
        if (newCGPA < 0.0 || newCGPA > 10.0)
            throw invalid_argument("CGPA must be between 0.00 and 10.00.");
        cgpa = newCGPA;
    }

    string getStudentID() const { return studentID; }
    int getYear() const { return year; }
    string getDepartment() const { return department; }
    int getTimetableNo() const { return timetableNo; }
    double getCGPA() const { return cgpa; }

    friend void studentSummary(const Student &s);
};

void studentSummary(const Student &s)
{
    cout << "\nStudent Summary";
    cout << "\nName       : " << s.name;
    cout << "\nRoll No : " << s.studentID;
    cout << "\nDepartment : " << s.department;
    cout << "\nYear       : " << s.year;
    cout << "\nCGPA       : " << s.cgpa;
    cout << "\nTimetable  : " << s.timetableNo << endl;
}


// ============================================================
// FACULTY CLASS
// ============================================================

class Faculty : public Person
{
protected:
    string facultyID;
    string department;
    string subject;
    string courseCode;

public:
    Faculty(string n = "", string id = "", string e = "",
            string dpt = "", string sub = "", string code = "", string p = "")
        : Person(n, e, p)
    {
        facultyID = id;
        department = dpt;
        subject = sub;
        courseCode = code;
    }

    virtual void display() const
    {
        cout << "\n--- Faculty Details ---" << endl;
        cout << "Name        : " << name << endl;
        cout << "Faculty ID  : " << facultyID << endl;
        cout << "Email       : " << email << endl;
        cout << "Phone       : " << (phone.empty() ? "Not provided" : phone) << endl;
        cout << "Department  : " << department << endl;
        cout << "Subject     : " << subject << endl;
        cout << "Course Code : " << courseCode << endl;
    }

    string getFacultyID() const { return facultyID; }
    string getDepartment() const { return department; }
    string getSubject() const { return subject; }
    string getCourseCode() const { return courseCode; }
};


// ============================================================
// PLATFORM  (with website, so problem links can be validated)
// ============================================================

class Platform
{
private:
    int platformID;
    string platformName;
    string website;

public:
    Platform(int id = 0, string name = "", string site = "")
    {
        platformID = id;
        platformName = name;
        website = site;
    }

    void display() const
    {
        cout << setw(5) << platformID << setw(16) << platformName << website << endl;
    }

    int getID() const { return platformID; }
    string getName() const { return platformName; }
    string getWebsite() const { return website; }
};

int findPlatformIndex(const vector<Platform> &platforms, int id)
{
    for (int i = 0; i < (int)platforms.size(); i++)
        if (platforms[i].getID() == id) return i;
    return -1;
}

string getPlatformName(const vector<Platform> &platforms, int id)
{
    int idx = findPlatformIndex(platforms, id);
    if (idx == -1) return "Unknown";
    return platforms[idx].getName();
}

string getPlatformWebsite(const vector<Platform> &platforms, int id)
{
    int idx = findPlatformIndex(platforms, id);
    if (idx == -1) return "";
    return platforms[idx].getWebsite();
}

int nextPlatformID(const vector<Platform> &platforms)
{
    int maxID = 0;
    for (int i = 0; i < (int)platforms.size(); i++)
        if (platforms[i].getID() > maxID) maxID = platforms[i].getID();
    return maxID + 1;
}

bool platformNameExists(const vector<Platform> &platforms, const string &name)
{
    for (int i = 0; i < (int)platforms.size(); i++)
        if (toLowerStr(platforms[i].getName()) == toLowerStr(name)) return true;
    return false;
}


// ============================================================
// PROBLEM CLASS  (each problem carries its own direct link)
// ============================================================

class Problem
{
private:
    int problemID;
    string title;
    string difficulty;
    int platformID;
    string topic;
    string url;
    string description;

public:
    Problem(int id = 0, string t = "", string d = "",
            int pid = 0, string top = "", string u = "", string desc = "")
    {
        problemID = id;
        title = t;
        difficulty = d;
        platformID = pid;
        topic = top;
        url = u;
        description = desc.empty()
            ? "Practice " + (topic.empty() ? string("this problem") : topic)
                + ". Open the problem link for the full statement, constraints, examples, and submission requirements."
            : desc;
    }

    void display(const vector<Platform> &platforms) const
    {
        cout << "\nProblem ID : " << problemID;
        cout << "\nTitle      : " << title;
        cout << "\nDifficulty : " << difficulty;
        cout << "\nPlatform   : " << getPlatformName(platforms, platformID);
        cout << "\nTopic      : " << topic;
        cout << "\nLink       : " << url << endl;
        cout << "Description: " << (description.empty() ? "Not provided" : description) << endl;
    }

    int getID() const { return problemID; }
    string getTitle() const { return title; }
    string getDifficulty() const { return difficulty; }
    int getPlatformID() const { return platformID; }
    string getTopic() const { return topic; }
    string getURL() const { return url; }
    string getDescription() const { return description; }
};

int findProblemIndex(const vector<Problem> &problems, int id)
{
    for (int i = 0; i < (int)problems.size(); i++)
        if (problems[i].getID() == id) return i;
    return -1;
}

int nextProblemID(const vector<Problem> &problems)
{
    int maxID = 100;   // problem IDs start at 101
    for (int i = 0; i < (int)problems.size(); i++)
        if (problems[i].getID() > maxID) maxID = problems[i].getID();
    return maxID + 1;
}

bool problemURLExists(const vector<Problem> &problems, const string &url)
{
    for (int i = 0; i < (int)problems.size(); i++)
        if (problems[i].getURL() == url) return true;
    return false;
}


// ============================================================
// STUDENT PLATFORM (relationship: one student -> many platforms)
// ============================================================

class StudentPlatform
{
private:
    string studentID;
    int platformID;
    string profileURL;

public:
    StudentPlatform(string sid = "", int pid = 0, string url = "")
    {
        studentID = sid;
        platformID = pid;
        profileURL = url;
    }

    void display(const vector<Platform> &platforms) const
    {
        cout << "\nRoll No  : " << studentID;
        cout << "\nPlatform    : " << getPlatformName(platforms, platformID);
        cout << "\nProfile URL : " << profileURL << endl;
    }

    string getStudentID() const { return studentID; }
    int getPlatformID() const { return platformID; }
    string getProfileURL() const { return profileURL; }
};

bool isStudentPlatformUnique(const vector<StudentPlatform> &links,
                              const string &studentID, int platformID)
{
    for (int i = 0; i < (int)links.size(); i++)
    {
        if (links[i].getStudentID() == studentID &&
            links[i].getPlatformID() == platformID)
            return false;
    }
    return true;
}


// ============================================================
// ASSIGNMENT CLASS  (Student + Problem + Faculty + Deadline)
// ============================================================

class Assignment
{
private:
    int assignmentID;
    string studentID;
    int problemID;
    string facultyID;
    string deadline;

public:
    Assignment(int aid = 0, string sid = "", int pid = 0,
               string fid = "", string dl = "")
    {
        assignmentID = aid;
        studentID = sid;
        problemID = pid;
        facultyID = fid;
        deadline = dl;
    }

    int getAssignmentID() const { return assignmentID; }
    string getStudentID() const { return studentID; }
    int getProblemID() const { return problemID; }
    string getFacultyID() const { return facultyID; }
    string getDeadline() const { return deadline; }
};

int nextAssignmentID(const vector<Assignment> &assignments)
{
    int maxID = 0;
    for (int i = 0; i < (int)assignments.size(); i++)
        if (assignments[i].getAssignmentID() > maxID) maxID = assignments[i].getAssignmentID();
    return maxID + 1;
}

bool hasAssignment(const vector<Assignment> &assignments,
                    const string &studentID, int problemID)
{
    for (int i = 0; i < (int)assignments.size(); i++)
        if (assignments[i].getStudentID() == studentID &&
            assignments[i].getProblemID() == problemID)
            return true;
    return false;
}

bool facultyAssignedProblem(const vector<Assignment> &assignments, const string &facultyID,
                             const string &studentID, int problemID)
{
    for (int i = 0; i < (int)assignments.size(); i++)
        if (assignments[i].getFacultyID() == facultyID &&
            assignments[i].getStudentID() == studentID &&
            assignments[i].getProblemID() == problemID)
            return true;
    return false;
}


// ============================================================
// SUBMISSION CLASS  (the evidence of a student's work)
// ============================================================

class Submission
{
private:
    int submissionID;
    string studentID;
    int problemID;
    int platformID;
    string submissionURL;
    string submissionDate;
    string status; // Pending, Accepted, Rejected, In Progress

public:
    Submission(int sub = 0, string sid = "", int pid = 0, int plid = 0,
               string url = "", string date = "", string st = "Pending")
    {
        submissionID = sub;
        studentID = sid;
        problemID = pid;
        platformID = plid;
        submissionURL = url;
        submissionDate = date;
        status = st;
    }

    void display(const vector<Platform> &platforms) const
    {
        cout << "\nApp Submission ID : " << submissionID;
        cout << "\nRoll No    : " << studentID;
        cout << "\nProblem ID    : " << problemID;
        cout << "\nPlatform      : " << getPlatformName(platforms, platformID);
        cout << "\nSubmission URL: " << submissionURL;
        cout << "\nDate          : " << submissionDate;
        cout << "\nStatus        : " << status << endl;
    }

    int getSubmissionID() const { return submissionID; }
    string getStudentID() const { return studentID; }
    int getProblemID() const { return problemID; }
    int getPlatformID() const { return platformID; }
    string getSubmissionURL() const { return submissionURL; }
    string getDate() const { return submissionDate; }
    string getStatus() const { return status; }

    void updateStatus(string newStatus)
    {
        if (newStatus != "Pending" && newStatus != "Accepted" &&
            newStatus != "Rejected" && newStatus != "In Progress")
            throw invalid_argument("Invalid submission status.");
        status = newStatus;
    }
};

int findSubmissionIndex(const vector<Submission> &submissions, int id)
{
    for (int i = 0; i < (int)submissions.size(); i++)
        if (submissions[i].getSubmissionID() == id) return i;
    return -1;
}

int nextSubmissionID(const vector<Submission> &submissions)
{
    int maxID = 0;
    for (int i = 0; i < (int)submissions.size(); i++)
        if (submissions[i].getSubmissionID() > maxID) maxID = submissions[i].getSubmissionID();
    return maxID + 1;
}

bool hasAcceptedSubmission(const vector<Submission> &submissions,
                            const string &studentID, int problemID)
{
    for (int i = 0; i < (int)submissions.size(); i++)
    {
        if (submissions[i].getStudentID() == studentID &&
            submissions[i].getProblemID() == problemID &&
            submissions[i].getStatus() == "Accepted")
            return true;
    }
    return false;
}

bool hasUnderReviewSubmission(const vector<Submission> &submissions,
                               const string &studentID, int problemID)
{
    for (int i = 0; i < (int)submissions.size(); i++)
    {
        if (submissions[i].getStudentID() == studentID &&
            submissions[i].getProblemID() == problemID &&
            (submissions[i].getStatus() == "Pending" ||
             submissions[i].getStatus() == "In Progress"))
            return true;
    }
    return false;
}


// ============================================================
// ASSIGNMENT STATE + PROGRESS CALCULATION
// ============================================================

// Completed / Under Review / OVERDUE / Not Completed
string assignmentState(const vector<Submission> &submissions, const Assignment &a)
{
    if (hasAcceptedSubmission(submissions, a.getStudentID(), a.getProblemID()))
        return "Completed";
    if (hasUnderReviewSubmission(submissions, a.getStudentID(), a.getProblemID()))
        return "Under Review";
    if (a.getDeadline() < todayDate())
        return "OVERDUE";
    return "Not Completed";
}

struct ProgressStats
{
    int total;
    int completed;
    int underReview;
    int overdue;
};

ProgressStats calculateProgress(const vector<Assignment> &assignments,
                                 const vector<Submission> &submissions,
                                 const string &studentID)
{
    ProgressStats s;
    s.total = 0;
    s.completed = 0;
    s.underReview = 0;
    s.overdue = 0;

    for (int i = 0; i < (int)assignments.size(); i++)
    {
        if (assignments[i].getStudentID() != studentID) continue;

        s.total++;
        string state = assignmentState(submissions, assignments[i]);
        if (state == "Completed") s.completed++;
        else if (state == "Under Review") s.underReview++;
        else if (state == "OVERDUE") s.overdue++;
    }
    return s;
}


// ============================================================
// PLATFORM OPERATIONS
// ============================================================

void displayPlatforms(const vector<Platform> &platforms)
{
    if (platforms.empty()) { cout << "\nNo platforms available." << endl; return; }
    cout << "\n--- Platforms ---" << endl;
    cout << setw(5) << "ID" << setw(16) << "Name" << "Website" << endl;
    cout << string(50, '-') << endl;
    for (int i = 0; i < (int)platforms.size(); i++) platforms[i].display();
}

void addPlatform(vector<Platform> &platforms)
{
    string name = getNonEmptyString("Enter platform name: ");
    if (platformNameExists(platforms, name))
    {
        cout << "\nError: Platform '" << name << "' already exists." << endl;
        return;
    }

    string website = getValidURL("Enter platform website (e.g. https://leetcode.com): ", "");
    if (website.empty()) { cout << "\nCancelled." << endl; return; }
    while (!website.empty() && website[website.size() - 1] == '/')
        website.erase(website.size() - 1);

    int id = nextPlatformID(platforms);
    platforms.push_back(Platform(id, name, website));
    cout << "\nPlatform added successfully with ID " << id << "." << endl;
}

void deletePlatform(vector<Platform> &platforms, const vector<Problem> &problems,
                     const vector<StudentPlatform> &links)
{
    int id = getInteger("Enter platform ID: ");
    int idx = findPlatformIndex(platforms, id);
    if (idx == -1) { cout << "\nPlatform not found." << endl; return; }

    for (int i = 0; i < (int)problems.size(); i++)
    {
        if (problems[i].getPlatformID() == id)
        {
            cout << "\nCannot delete: problems still belong to this platform." << endl;
            return;
        }
    }
    for (int i = 0; i < (int)links.size(); i++)
    {
        if (links[i].getPlatformID() == id)
        {
            cout << "\nCannot delete: students have linked accounts on this platform." << endl;
            return;
        }
    }

    platforms.erase(platforms.begin() + idx);
    cout << "\nPlatform deleted successfully." << endl;
}


// ============================================================
// STUDENT OPERATIONS  (CGPA ranking, timetable, CRUD)
// ============================================================

int findStudentIndex(const vector<Student> &students, const string &id)
{
    for (int i = 0; i < (int)students.size(); i++)
        if (students[i].getStudentID() == id) return i;
    return -1;
}

bool isStudentIDUnique(const vector<Student> &students, const string &id)
{
    return findStudentIndex(students, id) == -1;
}

template <typename Records, typename IdGetter>
string nextMemberID(const Records &records, char prefix, IdGetter getID)
{
    int maxNumber = 0;
    for (const auto &record : records)
    {
        string id = getID(record);
        if (id.size() < 2 || toupper((unsigned char)id[0]) != prefix) continue;
        bool digitsOnly = true;
        for (size_t i = 1; i < id.size(); ++i)
            if (!isdigit((unsigned char)id[i])) { digitsOnly = false; break; }
        if (digitsOnly) maxNumber = max(maxNumber, atoi(id.substr(1).c_str()));
    }
    ostringstream out;
    out << prefix << setw(3) << setfill('0') << (maxNumber + 1);
    return out.str();
}

string departmentRollCode(const string &department)
{
    if (department == "AIML") return "AI";
    if (department == "CSE") return "CS";
    if (department == "ECE") return "EC";
    if (department == "IT") return "IT";
    if (department == "ME") return "ME";
    return "";
}

string nextStudentRollNo(const vector<Student> &students, const string &department,
                         bool lateralEntry)
{
    const string cohort = lateralEntry ? "26B21" : "25B11";
    const string stem = cohort + departmentRollCode(department);
    int highest = 0;
    for (const Student &student : students)
    {
        const string id = student.getStudentID();
        if (id.size() != stem.size() + 3 || id.compare(0, stem.size(), stem) != 0) continue;
        const string suffix = id.substr(stem.size());
        if (all_of(suffix.begin(), suffix.end(), [](unsigned char ch) { return isdigit(ch); }))
            highest = max(highest, atoi(suffix.c_str()));
    }
    ostringstream out;
    out << stem << setw(3) << setfill('0') << (highest + 1);
    return out.str();
}

// Highest CGPA first; equal CGPA -> alphabetical by name.
bool compareStudentByCGPA(const Student &a, const Student &b)
{
    if (fabs(a.getCGPA() - b.getCGPA()) > 0.0001) return a.getCGPA() > b.getCGPA();
    return a.getName() < b.getName();
}

bool compareStudentByName(const Student &a, const Student &b)
{
    return a.getName() < b.getName();
}

bool compareStudentByDepartment(const Student &a, const Student &b)
{
    if (a.getDepartment() != b.getDepartment()) return a.getDepartment() < b.getDepartment();
    return compareStudentByCGPA(a, b);
}

// Students with the same CGPA share the same rank.
int getStudentRank(const vector<Student> &students, const string &id)
{
    int idx = findStudentIndex(students, id);
    if (idx == -1) return -1;

    int rank = 1;
    for (int i = 0; i < (int)students.size(); i++)
        if (students[i].getCGPA() > students[idx].getCGPA() + 0.0001) rank++;
    return rank;
}

void printStudentTable(const vector<Student> &list, bool showRank)
{
    if (list.empty()) { cout << "\nNo students to show." << endl; return; }

    cout << "\n";
    if (showRank) cout << setw(6) << "Rank";
    cout << setw(13) << "Roll No" << setw(20) << "Name" << setw(7) << "Dept"
         << setw(6) << "Year" << setw(7) << "CGPA" << "Timetable" << endl;
    cout << string(72, '-') << endl;

    int rank = 1;
    for (int i = 0; i < (int)list.size(); i++)
    {
        if (showRank)
        {
            if (i > 0 && fabs(list[i].getCGPA() - list[i - 1].getCGPA()) > 0.0001)
                rank = i + 1;
            cout << setw(6) << rank;
        }
        cout << setw(13) << list[i].getStudentID()
             << setw(20) << shorten(list[i].getName(), 18)
             << setw(7) << list[i].getDepartment()
             << setw(6) << list[i].getYear()
             << setw(7) << list[i].getCGPA()
             << list[i].getTimetableNo() << endl;
    }
}

void addStudent(vector<Student> &students)
{
    string name = getNonEmptyString("Enter student name: ");
    string email = getValidEmail("Enter email: ");
    string phone = getValidPhone("Enter phone: ");
    int year = getIntegerInRange("Enter year (1-4): ", 1, 4);
    string department;
    while (true)
    {
        department = toUpperStr(getNonEmptyString("Enter department (AIML, CSE, ECE, IT, ME): "));
        if (!departmentRollCode(department).empty()) break;
        cout << "Choose one of the listed departments so a matching roll number can be generated." << endl;
    }
    bool lateralEntry = false;
    if (year == 2)
        lateralEntry = getIntegerInRange("Admission type: 1. Regular  2. Second-year lateral entry: ", 1, 2) == 2;
    string id = nextStudentRollNo(students, department, lateralEntry);
    cout << "Generated roll number: " << id << endl;

    Timetable::printAll();
    int timetableNo = getIntegerInRange("Enter timetable number (1-9): ", 1, Timetable::COUNT);
    double cgpa = getDoubleInRange("Enter CGPA (0.00 - 10.00): ", 0.0, 10.0);

    students.push_back(Student(name, id, email, year, department, timetableNo, cgpa, phone));
    sort(students.begin(), students.end(), compareStudentByCGPA);
    cout << "\nStudent added successfully with ID " << id
         << ". Login using this ID and the default password student123." << endl;
}

// Always shows students ranked by CGPA (highest first).
void displayStudents(const vector<Student> &students)
{
    if (students.empty()) { cout << "\nNo students available." << endl; return; }

    vector<Student> ranked = students;
    sort(ranked.begin(), ranked.end(), compareStudentByCGPA);

    cout << "\n===== CAMPUS ROSTER RANKING (by CGPA) =====";
    printStudentTable(ranked, true);
}

void displayStudentsByDepartment(const vector<Student> &students)
{
    string dept = toUpperStr(getNonEmptyString("Enter department: "));

    vector<Student> filtered;
    for (int i = 0; i < (int)students.size(); i++)
        if (students[i].getDepartment() == dept) filtered.push_back(students[i]);

    if (filtered.empty()) { cout << "\nNo students found in " << dept << "." << endl; return; }

    sort(filtered.begin(), filtered.end(), compareStudentByCGPA);
    cout << "\n===== " << dept << " STUDENTS (by CGPA) =====";
    printStudentTable(filtered, true);
}

void displayStudentsByTimetable(const vector<Student> &students)
{
    Timetable::printAll();
    int no = getIntegerInRange("Enter timetable number (1-9): ", 1, Timetable::COUNT);

    vector<Student> filtered;
    for (int i = 0; i < (int)students.size(); i++)
        if (students[i].getTimetableNo() == no) filtered.push_back(students[i]);

    if (filtered.empty()) { cout << "\nNo students follow timetable " << no << "." << endl; return; }

    sort(filtered.begin(), filtered.end(), compareStudentByCGPA);
    cout << "\n===== " << Timetable::describe(no) << " =====";
    printStudentTable(filtered, true);
}

void updateStudent(vector<Student> &students)
{
    string id = toUpperStr(getNonEmptyString("Enter roll number: "));
    int idx = findStudentIndex(students, id);
    if (idx == -1) { cout << "\nStudent not found." << endl; return; }

    cout << "\n1. Update Year";
    cout << "\n2. Update Email";
    cout << "\n3. Update Timetable";
    cout << "\n4. Update CGPA";
    int choice = getInteger("\nEnter choice: ");

    try
    {
        if (choice == 1)
            students[idx].updateYear(getIntegerInRange("Enter new year (1-4): ", 1, 4));
        else if (choice == 2)
            students[idx].updateEmail(getValidEmail("Enter new email: "));
        else if (choice == 3)
        {
            Timetable::printAll();
            students[idx].updateTimetable(getIntegerInRange("Enter new timetable number (1-9): ", 1, Timetable::COUNT));
        }
        else if (choice == 4)
            students[idx].updateCGPA(getDoubleInRange("Enter new CGPA (0.00 - 10.00): ", 0.0, 10.0));
        else { cout << "Invalid choice." << endl; return; }

        sort(students.begin(), students.end(), compareStudentByCGPA);
        cout << "\nStudent updated successfully." << endl;
    }
    catch (exception &e)
    {
        cout << "\nError: " << e.what() << endl;
    }
}

// Deleting a student also removes everything that belonged to them.
void deleteStudent(vector<Student> &students, vector<Assignment> &assignments,
                    vector<Submission> &submissions, vector<StudentPlatform> &links)
{
    string id = toUpperStr(getNonEmptyString("Enter roll number: "));
    int idx = findStudentIndex(students, id);
    if (idx == -1) { cout << "\nStudent not found." << endl; return; }

    if (!confirmAction("Delete " + students[idx].getName() + " and all their assignments/submissions?"))
    {
        cout << "\nCancelled." << endl;
        return;
    }

    for (int i = (int)assignments.size() - 1; i >= 0; i--)
        if (assignments[i].getStudentID() == id) assignments.erase(assignments.begin() + i);
    for (int i = (int)submissions.size() - 1; i >= 0; i--)
        if (submissions[i].getStudentID() == id) submissions.erase(submissions.begin() + i);
    for (int i = (int)links.size() - 1; i >= 0; i--)
        if (links[i].getStudentID() == id) links.erase(links.begin() + i);

    students.erase(students.begin() + idx);
    cout << "\nStudent deleted successfully." << endl;
}

void searchStudent(const vector<Student> &students)
{
    string id = toUpperStr(getNonEmptyString("Enter roll number: "));
    int idx = findStudentIndex(students, id);
    if (idx == -1) { cout << "\nStudent not found." << endl; return; }
    students[idx].display();
    cout << "CGPA Rank  : " << getStudentRank(students, id) << " of " << students.size() << endl;
}

void sortStudents(vector<Student> &students)
{
    if (students.empty()) { cout << "\nNo students available." << endl; return; }

    cout << "\n1. Sort by CGPA (highest first)";
    cout << "\n2. Sort by Name (A-Z)";
    cout << "\n3. Sort by Department (then CGPA)";
    int choice = getIntegerInRange("\nEnter choice: ", 1, 3);

    if (choice == 1)
    {
        sort(students.begin(), students.end(), compareStudentByCGPA);
        cout << "\nStudents sorted by CGPA.";
        printStudentTable(students, true);
    }
    else if (choice == 2)
    {
        sort(students.begin(), students.end(), compareStudentByName);
        cout << "\nStudents sorted by name.";
        printStudentTable(students, false);
    }
    else
    {
        sort(students.begin(), students.end(), compareStudentByDepartment);
        cout << "\nStudents sorted by department.";
        printStudentTable(students, false);
    }
}


// ============================================================
// FACULTY OPERATIONS
// ============================================================

int findFacultyIndex(const vector<Faculty> &faculty, const string &id)
{
    for (int i = 0; i < (int)faculty.size(); i++)
        if (faculty[i].getFacultyID() == id) return i;
    return -1;
}

bool isFacultyIDUnique(const vector<Faculty> &faculty, const string &id)
{
    return findFacultyIndex(faculty, id) == -1;
}

void addFaculty(vector<Faculty> &faculty)
{
    string id = nextMemberID(faculty, 'F', [](const Faculty &f) { return f.getFacultyID(); });
    cout << "Generated faculty ID: " << id << endl;

    string name = getNonEmptyString("Enter faculty name: ");
    string email = getValidEmail("Enter email: ");
    string phone = getValidPhone("Enter phone: ");
    string department = toUpperStr(getNonEmptyString("Enter department: "));
    string subject = getNonEmptyString("Enter subject taught: ");
    string courseCode = toUpperStr(getNonEmptyString("Enter course code (e.g. CS201): "));

    faculty.push_back(Faculty(name, id, email, department, subject, courseCode, phone));
    cout << "\nFaculty added successfully with ID " << id
         << ". Login using this ID and the default password faculty123." << endl;
}

void displayFaculty(const vector<Faculty> &faculty)
{
    if (faculty.empty()) { cout << "\nNo faculty available." << endl; return; }

    cout << "\n===== FACULTY DIRECTORY =====\n" << endl;
    cout << setw(8) << "ID" << setw(28) << "Name" << setw(7) << "Dept"
         << setw(36) << "Subject" << "Course" << endl;
    cout << string(87, '-') << endl;
    for (int i = 0; i < (int)faculty.size(); i++)
    {
        cout << setw(8) << faculty[i].getFacultyID()
             << setw(28) << shorten(faculty[i].getName(), 26)
             << setw(7) << faculty[i].getDepartment()
             << setw(36) << shorten(faculty[i].getSubject(), 34)
             << faculty[i].getCourseCode() << endl;
    }
}

void deleteFaculty(vector<Faculty> &faculty, const vector<Assignment> &assignments)
{
    string id = toUpperStr(getNonEmptyString("Enter faculty ID: "));
    int idx = findFacultyIndex(faculty, id);
    if (idx == -1) { cout << "\nFaculty not found." << endl; return; }

    for (int i = 0; i < (int)assignments.size(); i++)
    {
        if (assignments[i].getFacultyID() == id)
        {
            cout << "\nCannot delete: this faculty member still has assignments on record." << endl;
            return;
        }
    }

    faculty.erase(faculty.begin() + idx);
    cout << "\nFaculty deleted successfully." << endl;
}


// ============================================================
// PROBLEM OPERATIONS
// ============================================================

void printProblemRow(const Problem &p, const vector<Platform> &platforms)
{
    cout << setw(6) << p.getID()
         << setw(40) << shorten(p.getTitle(), 38)
         << setw(9) << p.getDifficulty()
         << setw(15) << getPlatformName(platforms, p.getPlatformID())
         << p.getTopic() << endl;
    cout << "      Link: " << p.getURL() << endl;
    cout << "      Description: " << p.getDescription() << endl;
}

void printProblemHeader()
{
    cout << "\n" << setw(6) << "ID" << setw(40) << "Title" << setw(9) << "Level"
         << setw(15) << "Platform" << "Topic" << endl;
    cout << string(90, '-') << endl;
}

void addProblem(vector<Problem> &problems, const vector<Platform> &platforms)
{
    if (platforms.empty()) { cout << "\nAdd a platform first." << endl; return; }

    string title = getNonEmptyString("Enter problem title: ");

    cout << "\n1. Easy  2. Medium  3. Hard";
    int level = getIntegerInRange("\nEnter difficulty: ", 1, 3);
    string difficulty = (level == 1) ? "Easy" : (level == 2) ? "Medium" : "Hard";

    displayPlatforms(platforms);
    int platformID = getInteger("Enter platform ID from the list above: ");
    if (findPlatformIndex(platforms, platformID) == -1)
    {
        cout << "\nError: Platform ID " << platformID << " does not exist." << endl;
        return;
    }

    string topic = getNonEmptyString("Enter topic: ");
    string description = getNonEmptyString("Enter problem description: ");

    string site = getPlatformWebsite(platforms, platformID);
    cout << "\nPaste the direct link to this problem";
    if (!site.empty()) cout << " (must start with " << site << ")";
    cout << ".";
    string url = getValidURL("\nProblem link (or 'cancel'): ", site);
    if (url.empty()) { cout << "\nCancelled." << endl; return; }

    if (problemURLExists(problems, url))
    {
        cout << "\nError: a problem with this link already exists." << endl;
        return;
    }

    int id = nextProblemID(problems);
    problems.push_back(Problem(id, title, difficulty, platformID, topic, url, description));
    cout << "\nProblem added successfully with ID " << id << "." << endl;
}

void displayProblems(const vector<Problem> &problems, const vector<Platform> &platforms)
{
    if (problems.empty()) { cout << "\nNo problems available." << endl; return; }
    printProblemHeader();
    for (int i = 0; i < (int)problems.size(); i++) printProblemRow(problems[i], platforms);
    cout << "\nTotal problems: " << problems.size() << endl;
}

void displayProblemsByPlatform(const vector<Problem> &problems, const vector<Platform> &platforms)
{
    displayPlatforms(platforms);
    int platformID = getInteger("Enter platform ID: ");
    if (findPlatformIndex(platforms, platformID) == -1)
    {
        cout << "\nPlatform not found." << endl;
        return;
    }

    bool found = false;
    for (int i = 0; i < (int)problems.size(); i++)
    {
        if (problems[i].getPlatformID() == platformID)
        {
            if (!found) printProblemHeader();
            printProblemRow(problems[i], platforms);
            found = true;
        }
    }
    if (!found) cout << "\nNo problems on this platform yet." << endl;
}

void deleteProblem(vector<Problem> &problems, const vector<Assignment> &assignments)
{
    int id = getInteger("Enter problem ID: ");
    int idx = findProblemIndex(problems, id);
    if (idx == -1) { cout << "\nProblem not found." << endl; return; }

    for (int i = 0; i < (int)assignments.size(); i++)
    {
        if (assignments[i].getProblemID() == id)
        {
            cout << "\nCannot delete: this problem is already assigned to students." << endl;
            return;
        }
    }

    problems.erase(problems.begin() + idx);
    cout << "\nProblem deleted successfully." << endl;
}


// ============================================================
// FUNCTION OVERLOADING  (search by keyword vs by ID)
// ============================================================

void searchProblem(const vector<Problem> &problems,
                    const vector<Platform> &platforms, string keyword)
{
    bool found = false;
    for (int i = 0; i < (int)problems.size(); i++)
    {
        if (containsIgnoreCase(problems[i].getTitle(), keyword) ||
            containsIgnoreCase(problems[i].getTopic(), keyword))
        {
            if (!found) printProblemHeader();
            printProblemRow(problems[i], platforms);
            found = true;
        }
    }
    if (!found) cout << "\nNo problem matches '" << keyword << "'." << endl;
}

void searchProblem(const vector<Problem> &problems,
                    const vector<Platform> &platforms, int problemID)
{
    int idx = findProblemIndex(problems, problemID);
    if (idx == -1) { cout << "\nProblem not found." << endl; return; }
    problems[idx].display(platforms);
}

void searchProblemMenu(const vector<Problem> &problems, const vector<Platform> &platforms)
{
    cout << "\n1. Search by Problem ID";
    cout << "\n2. Search by Title / Topic keyword";
    int choice = getIntegerInRange("\nEnter choice: ", 1, 2);

    if (choice == 1)
    {
        int id = getInteger("Enter problem ID: ");
        searchProblem(problems, platforms, id);       // int overload
    }
    else
    {
        string keyword = getNonEmptyString("Enter keyword: ");
        searchProblem(problems, platforms, keyword);  // string overload
    }
}


// ============================================================
// STUDENT PLATFORM OPERATIONS
// ============================================================

void addStudentPlatform(vector<StudentPlatform> &links, const string &studentID,
                         const vector<Platform> &platforms)
{
    displayPlatforms(platforms);
    int platformID = getInteger("Enter platform ID from the list above: ");
    if (findPlatformIndex(platforms, platformID) == -1)
    {
        cout << "\nPlatform not found." << endl;
        return;
    }

    if (!isStudentPlatformUnique(links, studentID, platformID))
    {
        cout << "\nYou already have an account linked for this platform." << endl;
        return;
    }

    string site = getPlatformWebsite(platforms, platformID);
    string url = getValidURL("Enter your profile URL on this platform (or 'cancel'): ", site);
    if (url.empty()) { cout << "\nCancelled." << endl; return; }

    links.push_back(StudentPlatform(studentID, platformID, url));
    cout << "\nPlatform account linked successfully." << endl;
}

void displayStudentPlatforms(const vector<StudentPlatform> &links,
                              const vector<Platform> &platforms,
                              const string &studentID)
{
    bool found = false;
    for (int i = 0; i < (int)links.size(); i++)
    {
        if (links[i].getStudentID() == studentID)
        {
            links[i].display(platforms);
            found = true;
        }
    }
    if (!found) cout << "\nNo linked platform accounts found." << endl;
}


// ============================================================
// ASSIGNMENT OPERATIONS  (Faculty assigns Problem to Student)
// ============================================================

void printAssignment(const Assignment &a, const vector<Problem> &problems,
                      const vector<Platform> &platforms,
                      const vector<Submission> &submissions)
{
    int pIdx = findProblemIndex(problems, a.getProblemID());

    cout << "\nAssignment ID : " << a.getAssignmentID();
    cout << "\nRoll No    : " << a.getStudentID();
    cout << "\nAssigned By   : " << a.getFacultyID();
    if (pIdx != -1)
    {
        const Problem &p = problems[pIdx];
        cout << "\nProblem       : " << p.getID() << " - " << p.getTitle()
             << " [" << p.getDifficulty() << ", " << p.getTopic() << "]";
        cout << "\nPlatform      : " << getPlatformName(platforms, p.getPlatformID());
        cout << "\nProblem Link  : " << p.getURL();
    }
    else
    {
        cout << "\nProblem       : " << a.getProblemID() << " (no longer available)";
    }
    cout << "\nDeadline      : " << a.getDeadline();
    cout << "\nState         : " << assignmentState(submissions, a) << endl;
}

// Returns true if a new assignment was created (false if it already existed).
bool createAssignment(vector<Assignment> &assignments, const string &studentID,
                       int problemID, const string &facultyID, const string &deadline)
{
    if (hasAssignment(assignments, studentID, problemID)) return false;
    assignments.push_back(Assignment(nextAssignmentID(assignments), studentID,
                                      problemID, facultyID, deadline));
    return true;
}

void assignProblem(vector<Assignment> &assignments, const vector<Student> &students,
                    const vector<Problem> &problems, const string &facultyID)
{
    string studentID = toUpperStr(getNonEmptyString("Enter roll number: "));
    if (findStudentIndex(students, studentID) == -1)
    {
        cout << "\nStudent not found." << endl;
        return;
    }

    int problemID = getInteger("Enter problem ID: ");
    if (findProblemIndex(problems, problemID) == -1)
    {
        cout << "\nProblem not found." << endl;
        return;
    }

    if (hasAssignment(assignments, studentID, problemID))
    {
        cout << "\nThis problem is already assigned to this student." << endl;
        return;
    }

    string deadline = getValidDate("Enter deadline (YYYY-MM-DD): ", true);
    createAssignment(assignments, studentID, problemID, facultyID, deadline);
    cout << "\nProblem assigned successfully." << endl;
}

void assignProblemToDepartment(vector<Assignment> &assignments, const vector<Student> &students,
                                const vector<Problem> &problems, const string &facultyID)
{
    int problemID = getInteger("Enter problem ID: ");
    if (findProblemIndex(problems, problemID) == -1)
    {
        cout << "\nProblem not found." << endl;
        return;
    }

    string dept = toUpperStr(getNonEmptyString("Enter department (e.g. CSE): "));
    string deadline = getValidDate("Enter deadline (YYYY-MM-DD): ", true);

    int added = 0;
    int skipped = 0;
    for (int i = 0; i < (int)students.size(); i++)
    {
        if (students[i].getDepartment() != dept) continue;
        if (createAssignment(assignments, students[i].getStudentID(), problemID, facultyID, deadline))
            added++;
        else
            skipped++;
    }

    if (added == 0 && skipped == 0)
        cout << "\nNo students found in department " << dept << "." << endl;
    else
        cout << "\nAssigned to " << added << " student(s); " << skipped
             << " already had this problem." << endl;
}

void displayAssignmentsForStudent(const vector<Assignment> &assignments,
                                   const vector<Problem> &problems,
                                   const vector<Platform> &platforms,
                                   const vector<Submission> &submissions,
                                   const string &studentID)
{
    bool found = false;
    for (int i = 0; i < (int)assignments.size(); i++)
    {
        if (assignments[i].getStudentID() == studentID)
        {
            printAssignment(assignments[i], problems, platforms, submissions);
            found = true;
        }
    }
    if (!found) cout << "\nNo assignments found for this student." << endl;
}

void displayAssignmentsForFaculty(const vector<Assignment> &assignments,
                                   const vector<Problem> &problems,
                                   const vector<Platform> &platforms,
                                   const vector<Submission> &submissions,
                                   const string &facultyID)
{
    bool found = false;
    for (int i = 0; i < (int)assignments.size(); i++)
    {
        if (assignments[i].getFacultyID() == facultyID)
        {
            printAssignment(assignments[i], problems, platforms, submissions);
            found = true;
        }
    }
    if (!found) cout << "\nYou have not assigned any problems yet." << endl;
}


// ============================================================
// SUBMISSION OPERATIONS
// ============================================================

void recordSubmission(vector<Submission> &submissions,
                       const vector<Assignment> &assignments,
                       const vector<Problem> &problems,
                       const vector<Platform> &platforms,
                       const string &studentID)
{
    int problemID = getInteger("Enter the problem ID you solved: ");

    // A submission must correspond to a real assignment.
    if (!hasAssignment(assignments, studentID, problemID))
    {
        cout << "\nThis problem was not assigned to you. Cannot record submission." << endl;
        return;
    }

    int pIdx = findProblemIndex(problems, problemID);
    if (pIdx == -1) { cout << "\nProblem not found." << endl; return; }

    if (hasAcceptedSubmission(submissions, studentID, problemID))
    {
        cout << "\nThis problem is already marked Accepted. Nothing more to submit." << endl;
        return;
    }

    int platformID = problems[pIdx].getPlatformID();
    string site = getPlatformWebsite(platforms, platformID);
    cout << "\nProblem  : " << problems[pIdx].getTitle();
    cout << "\nPlatform : " << getPlatformName(platforms, platformID) << endl;

    string url = getValidURL("Enter your submission URL (or 'cancel'): ", site);
    if (url.empty()) { cout << "\nCancelled." << endl; return; }

    // Students cannot mark their own work as Accepted - it starts as Pending
    // and only the faculty who assigned the problem can review it.
    int id = nextSubmissionID(submissions);
    submissions.push_back(Submission(id, studentID, problemID, platformID,
                                      url, todayDate(), "Pending"));
    cout << "\nSubmission recorded. App Submission ID: " << id
         << ". Give this number to faculty for review; the platform's solution number stays in the URL."
         << " Status: Pending faculty review." << endl;
}

void displaySubmissionsForStudent(const vector<Submission> &submissions,
                                   const vector<Platform> &platforms,
                                   const string &studentID)
{
    bool found = false;
    for (int i = 0; i < (int)submissions.size(); i++)
    {
        if (submissions[i].getStudentID() == studentID)
        {
            submissions[i].display(platforms);
            found = true;
        }
    }
    if (!found) cout << "\nNo submissions found for this student." << endl;
}

// Shows submissions for problems this faculty assigned.
void displaySubmissionsForFaculty(const vector<Submission> &submissions,
                                   const vector<Assignment> &assignments,
                                   const vector<Platform> &platforms,
                                   const string &facultyID, bool onlyPending)
{
    bool found = false;
    for (int i = 0; i < (int)submissions.size(); i++)
    {
        if (!facultyAssignedProblem(assignments, facultyID,
                                     submissions[i].getStudentID(), submissions[i].getProblemID()))
            continue;

        if (onlyPending && submissions[i].getStatus() != "Pending" &&
            submissions[i].getStatus() != "In Progress")
            continue;

        submissions[i].display(platforms);
        found = true;
    }
    if (!found)
        cout << (onlyPending ? "\nNo submissions are waiting for your review."
                             : "\nNo submissions found for your assignments.") << endl;
}

void reviewSubmission(vector<Submission> &submissions,
                       const vector<Assignment> &assignments,
                       const vector<Platform> &platforms,
                       const string &facultyID)
{
    displaySubmissionsForFaculty(submissions, assignments, platforms, facultyID, true);
    int id = getInteger("Enter the app Submission ID shown above (not the platform solution number): ");
    int idx = findSubmissionIndex(submissions, id);
    if (idx == -1)
    {
        cout << "\nSubmission not found. Use the app's Submission ID, not the CodeChef number or student roll number." << endl;
        return;
    }

    if (!facultyAssignedProblem(assignments, facultyID,
                                 submissions[idx].getStudentID(), submissions[idx].getProblemID()))
    {
        cout << "\nYou did not assign this problem to this student, so you cannot review it." << endl;
        return;
    }

    submissions[idx].display(platforms);

    cout << "\n1. Pending  2. In Progress  3. Accepted  4. Rejected";
    int choice = getInteger("\nEnter new status: ");

    try
    {
        if (choice == 1) submissions[idx].updateStatus("Pending");
        else if (choice == 2) submissions[idx].updateStatus("In Progress");
        else if (choice == 3) submissions[idx].updateStatus("Accepted");
        else if (choice == 4) submissions[idx].updateStatus("Rejected");
        else { cout << "Invalid choice." << endl; return; }

        cout << "\nSubmission status updated." << endl;
    }
    catch (exception &e)
    {
        cout << "\nError: " << e.what() << endl;
    }
}


// ============================================================
// PROGRESS + REPORT
// ============================================================

void studentProgress(const vector<Assignment> &assignments,
                      const vector<Submission> &submissions,
                      const string &studentID)
{
    ProgressStats s = calculateProgress(assignments, submissions, studentID);

    if (s.total == 0)
    {
        cout << "\nNo assignments found for this student." << endl;
        return;
    }

    double percentage = (double)s.completed / s.total * 100;

    cout << "\n===== CODING JOURNEY PROGRESS =====" << endl;
    cout << "Roll No   : " << studentID << endl;
    cout << "Assigned     : " << s.total << endl;
    cout << "Accepted     : " << s.completed << endl;
    cout << "Under Review : " << s.underReview << endl;
    cout << "Overdue      : " << s.overdue << endl;
    cout << "Progress     : " << percentage << "%" << endl;
}

void generateReport(const vector<Student> &students,
                     const vector<Assignment> &assignments,
                     const vector<Submission> &submissions)
{
    ofstream file("campus_codetrack_report.txt");
    if (!file) { cout << "\nUnable to create report." << endl; return; }

    vector<Student> ranked = students;
    sort(ranked.begin(), ranked.end(), compareStudentByCGPA);

    file << fixed << setprecision(2);
    file << "CAMPUS CODETRACK - STUDENT PROGRESS REVIEW (ranked by CGPA)\n";
    file << "Generated on: " << todayDate() << "\n";
    file << "==================================================\n\n";

    for (int i = 0; i < (int)ranked.size(); i++)
    {
        ProgressStats s = calculateProgress(assignments, submissions, ranked[i].getStudentID());

        file << "Rank        : " << (getStudentRank(ranked, ranked[i].getStudentID())) << "\n";
        file << "Student     : " << ranked[i].getName() << "\n";
        file << "ID          : " << ranked[i].getStudentID() << "\n";
        file << "Department  : " << ranked[i].getDepartment()
             << " (Year " << ranked[i].getYear() << ")\n";
        file << "CGPA        : " << ranked[i].getCGPA() << "\n";
        file << "Timetable   : " << Timetable::describe(ranked[i].getTimetableNo()) << "\n";
        file << "Assigned    : " << s.total << "\n";
        file << "Accepted    : " << s.completed << "\n";
        file << "Under Review: " << s.underReview << "\n";
        file << "Overdue     : " << s.overdue << "\n";

        if (s.total > 0)
            file << "Progress    : " << ((double)s.completed / s.total * 100) << "%\n\n";
        else
            file << "Progress    : N/A (nothing assigned)\n\n";
    }

    file.close();
    cout << "\nReport generated as campus_codetrack_report.txt" << endl;
}


// ============================================================
// FILE HANDLING  (persistence for every entity)
// ============================================================

vector<string> splitLine(const string &line, char delimiter)
{
    vector<string> parts;
    string current;
    for (size_t i = 0; i < line.size(); i++)
    {
        if (line[i] == delimiter) { parts.push_back(current); current = ""; }
        else if (line[i] != '\r') current += line[i];
    }
    parts.push_back(current);
    return parts;
}

void saveStudents(const vector<Student> &students)
{
    ofstream file("tracker_students.txt");
    file << fixed << setprecision(2);
    for (int i = 0; i < (int)students.size(); i++)
    {
        file << students[i].getStudentID() << "|" << students[i].getName() << "|"
             << students[i].getEmail() << "|" << students[i].getYear() << "|"
             << students[i].getDepartment() << "|" << students[i].getTimetableNo() << "|"
             << students[i].getCGPA() << "|" << students[i].getPhone() << endl;
    }
}

void loadStudents(vector<Student> &students)
{
    ifstream file("tracker_students.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 7) continue;      // skips old-format / damaged lines

        int year = atoi(f[3].c_str());
        int timetableNo = atoi(f[5].c_str());
        double cgpa = atof(f[6].c_str());
        if (year < 1 || year > 4) year = 1;
        if (!Timetable::isValid(timetableNo)) timetableNo = 1;
        if (cgpa < 0.0 || cgpa > 10.0) cgpa = 0.0;

        string phone = f.size() >= 8 ? f[7] : "";
        students.push_back(Student(f[1], f[0], f[2], year, f[4], timetableNo, cgpa, phone));
    }
}

void saveFaculty(const vector<Faculty> &faculty)
{
    ofstream file("tracker_faculty.txt");
    for (int i = 0; i < (int)faculty.size(); i++)
    {
        file << faculty[i].getFacultyID() << "|" << faculty[i].getName() << "|"
             << faculty[i].getEmail() << "|" << faculty[i].getDepartment() << "|"
             << faculty[i].getSubject() << "|" << faculty[i].getCourseCode() << "|"
             << faculty[i].getPhone() << endl;
    }
}

void loadFaculty(vector<Faculty> &faculty)
{
    ifstream file("tracker_faculty.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 6) continue;
        string phone = f.size() >= 7 ? f[6] : "";
        faculty.push_back(Faculty(f[1], f[0], f[2], f[3], f[4], f[5], phone));
    }
}

void savePlatforms(const vector<Platform> &platforms)
{
    ofstream file("tracker_platforms.txt");
    for (int i = 0; i < (int)platforms.size(); i++)
        file << platforms[i].getID() << "|" << platforms[i].getName() << "|"
             << platforms[i].getWebsite() << endl;
}

void loadPlatforms(vector<Platform> &platforms)
{
    ifstream file("tracker_platforms.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 3) continue;
        platforms.push_back(Platform(atoi(f[0].c_str()), f[1], f[2]));
    }
}

void saveProblems(const vector<Problem> &problems)
{
    ofstream file("tracker_problems.txt");
    for (int i = 0; i < (int)problems.size(); i++)
    {
        file << problems[i].getID() << "|" << problems[i].getTitle() << "|"
             << problems[i].getDifficulty() << "|" << problems[i].getPlatformID() << "|"
             << problems[i].getTopic() << "|" << problems[i].getURL() << "|"
             << problems[i].getDescription() << endl;
    }
}

void loadProblems(vector<Problem> &problems)
{
    ifstream file("tracker_problems.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 6) continue;
        string description = f.size() >= 7 ? f[6] : "";
        problems.push_back(Problem(atoi(f[0].c_str()), f[1], f[2],
                                    atoi(f[3].c_str()), f[4], f[5], description));
    }
}

void saveStudentPlatforms(const vector<StudentPlatform> &links)
{
    ofstream file("tracker_studentplatforms.txt");
    for (int i = 0; i < (int)links.size(); i++)
    {
        file << links[i].getStudentID() << "|" << links[i].getPlatformID() << "|"
             << links[i].getProfileURL() << endl;
    }
}

void loadStudentPlatforms(vector<StudentPlatform> &links)
{
    ifstream file("tracker_studentplatforms.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 3) continue;
        links.push_back(StudentPlatform(f[0], atoi(f[1].c_str()), f[2]));
    }
}

void saveAssignments(const vector<Assignment> &assignments)
{
    ofstream file("tracker_assignments.txt");
    for (int i = 0; i < (int)assignments.size(); i++)
    {
        file << assignments[i].getAssignmentID() << "|" << assignments[i].getStudentID() << "|"
             << assignments[i].getProblemID() << "|" << assignments[i].getFacultyID() << "|"
             << assignments[i].getDeadline() << endl;
    }
}

void loadAssignments(vector<Assignment> &assignments)
{
    ifstream file("tracker_assignments.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 5) continue;
        assignments.push_back(Assignment(atoi(f[0].c_str()), f[1],
                                          atoi(f[2].c_str()), f[3], f[4]));
    }
}

void saveSubmissions(const vector<Submission> &submissions)
{
    ofstream file("tracker_submissions.txt");
    for (int i = 0; i < (int)submissions.size(); i++)
    {
        file << submissions[i].getSubmissionID() << "|" << submissions[i].getStudentID() << "|"
             << submissions[i].getProblemID() << "|" << submissions[i].getPlatformID() << "|"
             << submissions[i].getSubmissionURL() << "|" << submissions[i].getDate() << "|"
             << submissions[i].getStatus() << endl;
    }
}

void loadSubmissions(vector<Submission> &submissions)
{
    ifstream file("tracker_submissions.txt");
    if (!file) return;

    string line;
    while (getline(file, line))
    {
        vector<string> f = splitLine(line, '|');
        if (f.size() < 7) continue;
        submissions.push_back(Submission(atoi(f[0].c_str()), f[1],
                                          atoi(f[2].c_str()), atoi(f[3].c_str()),
                                          f[4], f[5], f[6]));
    }
}

void saveAllData(const vector<Student> &students, const vector<Faculty> &faculty,
                  const vector<Platform> &platforms, const vector<Problem> &problems,
                  const vector<Assignment> &assignments, const vector<Submission> &submissions,
                  const vector<StudentPlatform> &links)
{
    saveStudents(students);
    saveFaculty(faculty);
    savePlatforms(platforms);
    saveProblems(problems);
    saveAssignments(assignments);
    saveSubmissions(submissions);
    saveStudentPlatforms(links);
}


// ============================================================
// DEFAULT (SEED) DATA - used on the very first run
// ============================================================

void seedPlatforms(vector<Platform> &platforms)
{
    platforms.push_back(Platform(1, "LeetCode",      "https://leetcode.com"));
    platforms.push_back(Platform(2, "CodeChef",      "https://www.codechef.com"));
    platforms.push_back(Platform(3, "HackerRank",    "https://www.hackerrank.com"));
    platforms.push_back(Platform(4, "GeeksforGeeks", "https://www.geeksforgeeks.org"));
    platforms.push_back(Platform(5, "Codeforces",    "https://codeforces.com"));
}

void seedStudents(vector<Student> &s)
{
    // Local sample roster: roll numbers are grouped by programme.
    s.push_back(Student("Rama",       "25B11AI586", "rama@gmail.com",       2, "AIML", 5, 9.12));
    s.push_back(Student("Sita",       "25B11CS501", "sita@gmail.com",       2, "CSE",  6, 8.45));
    s.push_back(Student("Lakshman",   "25B11CS502", "lakshman@gmail.com",   2, "CSE",  5, 9.35));
    s.push_back(Student("Hanuman",    "25B11CS503", "hanuman@gmail.com",    3, "CSE",  7, 7.80));
    s.push_back(Student("Bharath",    "25B11AI587", "bharath@gmail.com",    2, "AIML", 5, 8.90));
    s.push_back(Student("Arjun",      "25B11AI588", "arjun@gmail.com",      1, "AIML", 1, 8.15));
    s.push_back(Student("Krishna",    "25B11EC601", "krishna@gmail.com",    2, "ECE",  3, 8.72));
    s.push_back(Student("Bhim",       "25B11EC602", "bhim@gmail.com",       3, "ECE",  8, 7.35));
    s.push_back(Student("Yudi",       "25B11IT701", "yudi@gmail.com",       2, "IT",   6, 9.05));
    s.push_back(Student("Draupadi",   "25B11IT702", "draupadi@gmail.com",   1, "IT",   2, 7.60));
    s.push_back(Student("Karna",      "25B11ME801", "karna@gmail.com",      4, "ME",   9, 8.30));
    s.push_back(Student("Duryan",     "25B11ME802", "duryan@gmail.com",     3, "ME",   7, 6.95));
    s.push_back(Student("Jatayu",     "25B11CS504", "jatayu@gmail.com",     1, "CSE",  1, 9.60));
    s.push_back(Student("Shatrughna", "25B11CS505", "shatrughna@gmail.com", 4, "CSE",  9, 8.05));
    s.push_back(Student("Nakul",      "25B11AI589", "nakul@gmail.com",      3, "AIML", 8, 8.60));
    s.push_back(Student("Sahadev",    "25B11EC603", "sahadev@gmail.com",    1, "ECE",  2, 7.10));

    sort(s.begin(), s.end(), compareStudentByCGPA);
}

void seedFaculty(vector<Faculty> &f)
{
    //                  name                       ID      email                       dept    subject                          course
    f.push_back(Faculty("Abhilash",                "F101", "abhilash@gmail.com",       "AIML", "Data Structures",               "CS201"));
    f.push_back(Faculty("Dr. Lakshmi Narayan",     "F102", "lakshmi.narayan@gmail.com","CSE",  "Design & Analysis of Algorithms","CS301"));
    f.push_back(Faculty("Prof. Ramesh Kulkarni",   "F103", "ramesh.kulkarni@gmail.com","CSE",  "Database Management Systems",   "CS305"));
    f.push_back(Faculty("Dr. Sunita Menon",        "F104", "sunita.menon@gmail.com",   "AIML", "Machine Learning",              "AI302"));
    f.push_back(Faculty("Prof. Harish Patel",      "F105", "harish.patel@gmail.com",   "IT",   "Web Technologies",              "IT210"));
    f.push_back(Faculty("Dr. Meenakshi Sundaram",  "F106", "meenakshi.s@gmail.com",    "IT",   "Computer Networks",             "IT305"));
    f.push_back(Faculty("Prof. Anil Deshmukh",     "F107", "anil.deshmukh@gmail.com",  "ECE",  "Digital Signal Processing",     "EC303"));
    f.push_back(Faculty("Dr. Farah Khan",          "F108", "farah.khan@gmail.com",     "ECE",  "Embedded Systems",              "EC401"));
    f.push_back(Faculty("Prof. Venkat Subramanian","F109", "venkat.s@gmail.com",       "ME",   "Numerical Methods",             "ME205"));
    f.push_back(Faculty("Dr. Rekha Balan",         "F110", "rekha.balan@gmail.com",    "ME",   "Engineering Mathematics",       "MA201"));
    f.push_back(Faculty("Prof. Sandeep Joshi",     "F111", "sandeep.joshi@gmail.com",  "CSE",  "Operating Systems",             "CS303"));
    f.push_back(Faculty("Dr. Pallavi Rao",         "F112", "pallavi.rao@gmail.com",    "AIML", "Deep Learning",                 "AI401"));
}

void seedProblems(vector<Problem> &p)
{
    const string LC = "https://leetcode.com/problems/";
    const string GG = "https://www.geeksforgeeks.org/problems/";
    const string HR = "https://www.hackerrank.com/challenges/";
    const string CC = "https://www.codechef.com/problems/";
    const string CF = "https://codeforces.com/problemset/problem/";

    //                  ID   title                                           level     plat topic                        link
    p.push_back(Problem(101, "Two Sum",                                      "Easy",   1, "Arrays",                     LC + "two-sum/"));
    p.push_back(Problem(102, "Binary Search",                                "Medium", 4, "Searching",                  GG + "binary-search-1587115620/1"));
    p.push_back(Problem(103, "Valid Parentheses",                            "Easy",   1, "Stacks",                     LC + "valid-parentheses/"));
    p.push_back(Problem(104, "Reverse a Linked List",                        "Easy",   4, "Linked Lists",               GG + "reverse-a-linked-list/1"));
    p.push_back(Problem(105, "Maximum Depth of Binary Tree",                 "Easy",   1, "Binary Trees",               LC + "maximum-depth-of-binary-tree/"));
    p.push_back(Problem(106, "Climbing Stairs",                              "Easy",   1, "Dynamic Programming",        LC + "climbing-stairs/"));
    p.push_back(Problem(107, "Number of Islands",                            "Medium", 1, "Graphs",                     LC + "number-of-islands/"));
    p.push_back(Problem(108, "Rat in a Maze Problem - I",                    "Medium", 4, "Backtracking",               GG + "rat-in-a-maze-problem/1"));
    p.push_back(Problem(109, "Longest Substring Without Repeating Characters","Medium",1, "Sliding Window",            LC + "longest-substring-without-repeating-characters/"));
    p.push_back(Problem(110, "Solve Me First",                               "Easy",   3, "Input/Output Basics",        HR + "solve-me-first/problem"));
    p.push_back(Problem(111, "Diagonal Difference",                          "Easy",   3, "Matrix Traversal",           HR + "diagonal-difference/problem"));
    p.push_back(Problem(112, "Container With Most Water",                    "Medium", 1, "Two Pointers",               LC + "container-with-most-water/"));
    p.push_back(Problem(113, "Kth Largest Element in an Array",              "Medium", 1, "Heaps",                      LC + "kth-largest-element-in-an-array/"));
    p.push_back(Problem(114, "Implement Trie (Prefix Tree)",                 "Medium", 1, "Tries",                      LC + "implement-trie-prefix-tree/"));
    p.push_back(Problem(115, "Implement Queue using Stacks",                 "Easy",   1, "Queues",                     LC + "implement-queue-using-stacks/"));
    p.push_back(Problem(116, "Detect Loop in Linked List",                   "Medium", 4, "Floyd's Cycle Detection",    GG + "detect-loop-in-linked-list/1"));
    p.push_back(Problem(117, "Kadane's Algorithm",                           "Medium", 4, "Subarray Optimization",      GG + "kadanes-algorithm-1587115620/1"));
    p.push_back(Problem(118, "Lonely Integer",                               "Easy",   3, "Bit Manipulation",           HR + "lonely-integer/problem"));
    p.push_back(Problem(119, "Sparse Arrays",                                "Medium", 3, "Hashing",                    HR + "sparse-arrays/problem"));
    p.push_back(Problem(120, "Mark and Toys",                                "Easy",   3, "Greedy",                     HR + "mark-and-toys/problem"));
    p.push_back(Problem(121, "Add Two Numbers",                              "Easy",   2, "Basic Math",                 CC + "FLOW001"));
    p.push_back(Problem(122, "Prime Generator",                              "Medium", 2, "Number Theory",              CC + "PRIME1"));
    p.push_back(Problem(123, "Turbo Sort",                                   "Medium", 2, "Sorting",                    CC + "TSORT"));
    p.push_back(Problem(124, "Reverse The Number",                           "Easy",   2, "Digit Manipulation",         CC + "FLOW007"));
    p.push_back(Problem(125, "GCD and LCM",                                  "Easy",   2, "Euclidean Algorithm",        CC + "FLOW016"));
    p.push_back(Problem(126, "ATM",                                          "Easy",   2, "Conditional Logic",          CC + "HS08TEST"));
    p.push_back(Problem(127, "Watermelon",                                   "Easy",   5, "Math & Parity",              CF + "4/A"));
    p.push_back(Problem(128, "Way Too Long Words",                           "Easy",   5, "String Manipulation",        CF + "71/A"));
    p.push_back(Problem(129, "Bit++",                                        "Easy",   5, "Implementation",             CF + "282/A"));
    p.push_back(Problem(130, "Domino piling",                                "Easy",   5, "Tiling & Constructive Logic",CF + "50/A"));
    p.push_back(Problem(131, "Beautiful Matrix",                             "Easy",   5, "Grid Distance",              CF + "263/A"));
}

void seedAssignments(vector<Assignment> &a)
{
    //                     id  student        problem faculty  deadline
    a.push_back(Assignment(1,  "25B11AI586", 101, "F101", "2026-12-31"));
    a.push_back(Assignment(2,  "25B11AI586", 103, "F101", "2026-12-31"));
    a.push_back(Assignment(3,  "25B11AI587", 101, "F101", "2026-12-31"));
    a.push_back(Assignment(4,  "25B11AI588", 104, "F101", "2026-11-30"));
    a.push_back(Assignment(5,  "25B11CS501", 106, "F102", "2026-11-30"));
    a.push_back(Assignment(6,  "25B11CS502", 107, "F102", "2026-09-10"));
    a.push_back(Assignment(7,  "25B11CS503", 109, "F102", "2026-12-15"));
    a.push_back(Assignment(8,  "25B11CS504", 114, "F102", "2026-12-15"));
    a.push_back(Assignment(9,  "25B11AI589", 119, "F104", "2026-12-10"));
    a.push_back(Assignment(10, "25B11IT701", 110, "F105", "2026-10-31"));
    a.push_back(Assignment(11, "25B11IT702", 111, "F106", "2026-11-05"));
    a.push_back(Assignment(12, "25B11EC601", 121, "F107", "2026-10-31"));
    a.push_back(Assignment(13, "25B11EC602", 127, "F108", "2026-11-20"));
    a.push_back(Assignment(14, "25B11ME801", 122, "F109", "2026-11-15"));
    a.push_back(Assignment(15, "25B11ME802", 125, "F110", "2026-11-15"));
}

void seedSubmissions(vector<Submission> &s)
{
    // Sample data only - the submission links are placeholders.
    s.push_back(Submission(1, "25B11AI586", 101, 1, "https://leetcode.com/submissions/detail/1001/", "2026-09-12", "Accepted"));
    s.push_back(Submission(2, "25B11AI586", 103, 1, "https://leetcode.com/submissions/detail/1002/", "2026-09-18", "Pending"));
    s.push_back(Submission(3, "25B11AI587", 101, 1, "https://leetcode.com/submissions/detail/1003/", "2026-09-14", "Accepted"));
    s.push_back(Submission(4, "25B11CS501", 106, 1, "https://leetcode.com/submissions/detail/1004/", "2026-09-16", "Accepted"));
    s.push_back(Submission(5, "25B11CS502", 107, 1, "https://leetcode.com/submissions/detail/1005/", "2026-09-08", "Rejected"));
    s.push_back(Submission(6, "25B11IT701", 110, 3, "https://www.hackerrank.com/challenges/solve-me-first/submissions/code/1006", "2026-09-15", "Accepted"));
    s.push_back(Submission(7, "25B11EC601", 121, 2, "https://www.codechef.com/viewsolution/1007", "2026-09-17", "In Progress"));
}

void seedStudentPlatforms(vector<StudentPlatform> &l)
{
    l.push_back(StudentPlatform("25B11AI586", 1, "https://leetcode.com/u/jahnavi/"));
    l.push_back(StudentPlatform("25B11AI586", 5, "https://codeforces.com/profile/jahnavi"));
    l.push_back(StudentPlatform("25B11CS502", 1, "https://leetcode.com/u/lakshman/"));
}

void seedDefaultData(vector<Student> &students, vector<Faculty> &faculty,
                      vector<Platform> &platforms, vector<Problem> &problems,
                      vector<Assignment> &assignments, vector<Submission> &submissions,
                      vector<StudentPlatform> &links)
{
    students.clear();
    faculty.clear();
    platforms.clear();
    problems.clear();
    assignments.clear();
    submissions.clear();
    links.clear();

    seedPlatforms(platforms);
    seedStudents(students);
    seedFaculty(faculty);
    seedProblems(problems);
    seedAssignments(assignments);
    seedSubmissions(submissions);
    seedStudentPlatforms(links);
}


// ============================================================
// STUDENT MENU
// ============================================================

void studentMenu(vector<Student> &students, const vector<Assignment> &assignments,
                  const vector<Problem> &problems, vector<Submission> &submissions,
                  vector<StudentPlatform> &links, const vector<Platform> &platforms,
                  const string &studentID)
{
    int idx = findStudentIndex(students, studentID);
    cout << "\nWelcome, " << students[idx].getName() << "!" << endl;

    while (true)
    {
        cout << "\n\n===== LEARNER WORKSPACE =====";
        cout << "\n1. View My Profile";
        cout << "\n2. View My Linked Platforms";
        cout << "\n3. Link a New Platform Account";
        cout << "\n4. View My Assigned Problems";
        cout << "\n5. Search Problems";
        cout << "\n6. Record a Submission";
        cout << "\n7. View My Submissions";
        cout << "\n8. View My Progress";
        cout << "\n9. Logout";

        int choice = getInteger("\nEnter choice: ");

        if (choice == 1)
        {
            idx = findStudentIndex(students, studentID);
            students[idx].display();
            cout << "CGPA Rank  : " << getStudentRank(students, studentID)
                 << " of " << students.size() << endl;
        }
        else if (choice == 2) displayStudentPlatforms(links, platforms, studentID);
        else if (choice == 3) addStudentPlatform(links, studentID, platforms);
        else if (choice == 4)
            displayAssignmentsForStudent(assignments, problems, platforms, submissions, studentID);
        else if (choice == 5) searchProblemMenu(problems, platforms);
        else if (choice == 6)
        {
            displayAssignmentsForStudent(assignments, problems, platforms, submissions, studentID);
            recordSubmission(submissions, assignments, problems, platforms, studentID);
        }
        else if (choice == 7) displaySubmissionsForStudent(submissions, platforms, studentID);
        else if (choice == 8)
        {
            idx = findStudentIndex(students, studentID);
            studentSummary(students[idx]);
            studentProgress(assignments, submissions, studentID);
        }
        else if (choice == 9) { cout << "\nLogged out." << endl; break; }
        else cout << "\nInvalid choice." << endl;
    }
}


// ============================================================
// FACULTY MENU
// ============================================================

void facultyMenu(const vector<Student> &students, const vector<Faculty> &faculty,
                  vector<Problem> &problems, vector<Assignment> &assignments,
                  vector<Submission> &submissions, vector<Platform> &platforms,
                  const string &facultyID)
{
    int fIdx = findFacultyIndex(faculty, facultyID);
    cout << "\nWelcome, " << faculty[fIdx].getName() << " ("
         << faculty[fIdx].getSubject() << ", " << faculty[fIdx].getDepartment() << ")" << endl;

    while (true)
    {
        cout << "\n\n===== FACULTY WORKSPACE =====";
        cout << "\n1.  Add Coding Problem (with link)";
        cout << "\n2.  View All Problems";
        cout << "\n3.  View Problems by Platform";
        cout << "\n4.  Search Problems";
        cout << "\n5.  Delete Problem";
        cout << "\n6.  View Platforms";
        cout << "\n7.  Add Platform";
        cout << "\n8.  Assign Problem to a Student";
        cout << "\n9.  Assign Problem to a Whole Department";
        cout << "\n10. View My Assignments";
        cout << "\n11. View Submissions Waiting for Review";
        cout << "\n12. View All Submissions for My Assignments";
        cout << "\n13. Review a Submission (Accept / Reject)";
        cout << "\n14. View Student Progress";
        cout << "\n15. Generate Report";
        cout << "\n16. Logout";

        int choice = getInteger("\nEnter choice: ");

        if (choice == 1) addProblem(problems, platforms);
        else if (choice == 2) displayProblems(problems, platforms);
        else if (choice == 3) displayProblemsByPlatform(problems, platforms);
        else if (choice == 4) searchProblemMenu(problems, platforms);
        else if (choice == 5) deleteProblem(problems, assignments);
        else if (choice == 6) displayPlatforms(platforms);
        else if (choice == 7) addPlatform(platforms);
        else if (choice == 8) assignProblem(assignments, students, problems, facultyID);
        else if (choice == 9) assignProblemToDepartment(assignments, students, problems, facultyID);
        else if (choice == 10)
            displayAssignmentsForFaculty(assignments, problems, platforms, submissions, facultyID);
        else if (choice == 11)
            displaySubmissionsForFaculty(submissions, assignments, platforms, facultyID, true);
        else if (choice == 12)
            displaySubmissionsForFaculty(submissions, assignments, platforms, facultyID, false);
        else if (choice == 13) reviewSubmission(submissions, assignments, platforms, facultyID);
        else if (choice == 14)
        {
            string id = toUpperStr(getNonEmptyString("Enter roll number: "));
            if (findStudentIndex(students, id) == -1) cout << "\nStudent not found." << endl;
            else studentProgress(assignments, submissions, id);
        }
        else if (choice == 15) generateReport(students, assignments, submissions);
        else if (choice == 16) { cout << "\nLogged out." << endl; break; }
        else cout << "\nInvalid choice." << endl;
    }
}


// ============================================================
// ADMIN MENU
// ============================================================

void adminMenu(vector<Student> &students, vector<Faculty> &faculty,
               vector<Problem> &problems, vector<Platform> &platforms,
               vector<Assignment> &assignments, vector<Submission> &submissions,
               vector<StudentPlatform> &links)
{
    while (true)
    {
        cout << "\n\n===== CAMPUS ADMIN CONSOLE =====";
        cout << "\n1.  Add Student";
        cout << "\n2.  View Students (ranked by CGPA)";
        cout << "\n3.  View Students by Department";
        cout << "\n4.  View Students by Timetable";
        cout << "\n5.  Update Student";
        cout << "\n6.  Delete Student";
        cout << "\n7.  Search Student";
        cout << "\n8.  Sort Students (CGPA / Name / Department)";
        cout << "\n9.  Add Faculty";
        cout << "\n10. View Faculty";
        cout << "\n11. Delete Faculty";
        cout << "\n12. View Platforms";
        cout << "\n13. Delete Platform";
        cout << "\n14. View Problems (oversight)";
        cout << "\n15. View University Timetables";
        cout << "\n16. Logout";

        int choice = getInteger("\nEnter choice: ");

        if (choice == 1) addStudent(students);
        else if (choice == 2) displayStudents(students);
        else if (choice == 3) displayStudentsByDepartment(students);
        else if (choice == 4) displayStudentsByTimetable(students);
        else if (choice == 5) updateStudent(students);
        else if (choice == 6) deleteStudent(students, assignments, submissions, links);
        else if (choice == 7) searchStudent(students);
        else if (choice == 8) sortStudents(students);
        else if (choice == 9) addFaculty(faculty);
        else if (choice == 10) displayFaculty(faculty);
        else if (choice == 11) deleteFaculty(faculty, assignments);
        else if (choice == 12) displayPlatforms(platforms);
        else if (choice == 13) deletePlatform(platforms, problems, links);
        else if (choice == 14) displayProblems(problems, platforms);
        else if (choice == 15) Timetable::printAll();
        else if (choice == 16) { cout << "\nLogged out." << endl; break; }
        else cout << "\nInvalid choice." << endl;
    }
}


// ============================================================
// MAIN
// ============================================================

int main()
{
    cout << fixed << setprecision(2) << left;

    vector<Student> students;
    vector<Faculty> faculty;
    vector<Problem> problems;
    vector<Assignment> assignments;
    vector<Submission> submissions;
    vector<StudentPlatform> links;
    vector<Platform> platforms;

    loadPlatforms(platforms);
    loadStudents(students);
    loadFaculty(faculty);
    loadProblems(problems);
    loadAssignments(assignments);
    loadSubmissions(submissions);
    loadStudentPlatforms(links);

    // First-ever run (or only old-format data files found): load the sample data.
    if (students.empty() && faculty.empty() && problems.empty() && platforms.empty())
    {
        seedDefaultData(students, faculty, platforms, problems,
                         assignments, submissions, links);
    }
    else
    {
        sort(students.begin(), students.end(), compareStudentByCGPA);
    }

    while (true)
    {
        cout << "\n\n==============================================";
        cout << "\n CODING PLATFORMS & STUDENT PROGRESS TRACER";
        cout << "\n==============================================";
        cout << "\n1. ADMIN LOGIN";
        cout << "\n2. FACULTY LOGIN";
        cout << "\n3. STUDENT LOGIN";
        cout << "\n4. EXIT";

        int choice = getInteger("\nEnter choice: ");

        if (choice == 1)
        {
            string username = getString("Username: ");
            string password = getString("Password: ");
            if (username == "admin" && password == "admin123")
            {
                adminMenu(students, faculty, problems, platforms, assignments, submissions, links);
                saveAllData(students, faculty, platforms, problems, assignments, submissions, links);
            }
            else
                cout << "\nInvalid admin login." << endl;
        }
        else if (choice == 2)
        {
            string id = toUpperStr(getString("Faculty ID: "));
            string password = getString("Password: ");
            if (findFacultyIndex(faculty, id) != -1 && password == "faculty123")
            {
                facultyMenu(students, faculty, problems, assignments, submissions, platforms, id);
                saveAllData(students, faculty, platforms, problems, assignments, submissions, links);
            }
            else
                cout << "\nInvalid faculty login." << endl;
        }
        else if (choice == 3)
        {
            string id = toUpperStr(getString("Roll No: "));
            string password = getString("Password: ");
            if (findStudentIndex(students, id) != -1 && password == "student123")
            {
                studentMenu(students, assignments, problems, submissions, links, platforms, id);
                saveAllData(students, faculty, platforms, problems, assignments, submissions, links);
            }
            else
                cout << "\nInvalid student login." << endl;
        }
        else if (choice == 4)
        {
            saveAllData(students, faculty, platforms, problems, assignments, submissions, links);

            cout << "\nData saved successfully.";
            cout << "\nThank you for using the system!" << endl;
            break;
        }
        else
        {
            cout << "\nInvalid choice." << endl;
        }
    }

    return 0;
}

