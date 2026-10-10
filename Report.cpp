#include "Report.h"
#include <iostream>

using namespace std;

void Report::displayStudentReport(Progress& progress) const {
    cout << "\n========== STUDENT PERFORMANCE REPORT ==========" << endl;
    progress.displayProgress();
    cout << "================================================" << endl;
}
