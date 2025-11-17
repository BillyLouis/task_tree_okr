#include<iostream>
#include<iomanip>
#include<string>
#include "TreeN.h"
#include <climits>

using namespace std;

// Prompt the user with `prompt` and return a validated non-negative integer.
// Accepts only digits (0-9). Keeps prompting until a valid value is entered.
static int promptForNonNegativeInt(const string &prompt) {
    string line;
    while (true) {
        cout << prompt;
        if (!std::getline(cin, line)) {
            // EOF or error - return 0 as a safe default
            return 0;
        }

        // trim whitespace
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            cout << "Please enter a number (digits only).\n";
            continue;
        }
        size_t end = line.find_last_not_of(" \t\r\n");
        string s = line.substr(start, end - start + 1);

        bool ok = !s.empty();
        for (char c : s) {
            if (!isdigit(static_cast<unsigned char>(c))) { ok = false; break; }
        }
        if (!ok) {
            cout << "Invalid input — please type digits only (no signs, letters or spaces).\n";
            continue;
        }

        try {
            long val = stol(s);
            if (val < 0) { cout << "Please enter a non-negative integer.\n"; continue; }
            if (val > INT_MAX) { cout << "Number too large, try a smaller value.\n"; continue; }
            return static_cast<int>(val);
        } catch (...) {
            cout << "Invalid numeric value, try again.\n";
        }
    }
}

int main()
{
  cout <<" This Program Uses a Tree to Simulate Whether or not a Project" <<endl;
  cout <<"Meet  or Violate the Deadline in Which the Project is to be Done" << endl;
  cout << " " << endl;

  Tree theJobs;
  TreeItr Top;
  Top = theJobs.TreeRoot();
  theJobs.setupTree(theJobs);
  //calcCompletion(Top);
  //determineCritical(Top);
  cout << "PROJECT SCHEDULE" << endl;
  theJobs.preorderPrint(Top);


  Tree project;
  TreeItr root = project.setupTree(project);


  //-- May Prompt a File here with Jobs to be added
  Task newTask("Write Docs", 5, 0, false);
  TreeItr added = project.addTask(root, newTask);

  // Ask the user how many days have passed and update statuses (validate digits-only)
  int daysPassed = promptForNonNegativeInt("\nEnter number of days passed since project start (integer): ");
  project.updateStatusByElapsedDays(daysPassed);

  cout << "\nPROJECT DIAGRAM (statuses after " << daysPassed << " days):\n";
  project.printDiagram(root);

    double okr = project.calculateOKRPercentage();
    cout << "\nProject OKR: " << okr << "% (" << fixed << setprecision(1) << okr << "% )\n";

  return 0;
}

