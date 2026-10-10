#ifndef PROGRESS_H
#define PROGRESS_H

class Progress {
private:
    int studentId;
    int totalAssignments;
    int completedAssignments;
    int pendingAssignments;
    float progressPercentage;

public:
    Progress();
    Progress(int sid, int total, int completed);

    int getStudentId();
    int getTotalAssignments();
    int getCompletedAssignments();
    int getPendingAssignments();
    float getProgressPercentage();

    void displayProgress();
};

#endif
