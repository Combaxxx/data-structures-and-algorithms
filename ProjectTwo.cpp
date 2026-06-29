#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

// Course structure stores one course
struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

// Trim extra spaces from text
string trim(string text) {
    size_t first = text.find_first_not_of(" \t\r\n");
    if (first == string::npos) {
        return "";
    }

    size_t last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

// Convert input to uppercase
string toUpperCase(string text) {
    transform(text.begin(), text.end(), text.begin(), ::toupper);
    return text;
}

// Find course by course number
Course* findCourse(vector<Course>& courses, string courseNumber) {
    courseNumber = toUpperCase(trim(courseNumber));

    for (Course& course : courses) {
        if (toUpperCase(course.courseNumber) == courseNumber) {
            return &course;
        }
    }

    return nullptr;
}

// Load course data from file
bool loadCourses(string fileName, vector<Course>& courses) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error: Could not open file." << endl;
        return false;
    }

    courses.clear();

    string line;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string item;
        vector<string> tokens;

        while (getline(ss, item, ',')) {
            tokens.push_back(trim(item));
        }

        if (tokens.size() < 2) {
            cout << "Error: Invalid line format skipped: " << line << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); ++i) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        courses.push_back(course);
    }

    file.close();

    sort(courses.begin(), courses.end(), [](Course a, Course b) {
        return a.courseNumber < b.courseNumber;
    });

    cout << "Course data loaded successfully." << endl;
    return true;
}

// Print all courses in alphanumeric order
void printCourseList(vector<Course>& courses) {
    cout << endl;
    cout << "Here is a sample schedule:" << endl;
    cout << endl;

    for (Course course : courses) {
        cout << course.courseNumber << ", " << course.courseTitle << endl;
    }
}

// Print one course and its prerequisites
void printCourseInformation(vector<Course>& courses) {
    string courseNumber;

    cout << "What course do you want to know about? ";
    cin >> courseNumber;

    Course* course = findCourse(courses, courseNumber);

    if (course == nullptr) {
        cout << "Error: Course not found." << endl;
        return;
    }

    cout << endl;
    cout << course->courseNumber << ", " << course->courseTitle << endl;

    cout << "Prerequisites: ";

    if (course->prerequisites.empty()) {
        cout << "None" << endl;
    } else {
        for (size_t i = 0; i < course->prerequisites.size(); ++i) {
            Course* prereq = findCourse(courses, course->prerequisites[i]);

            if (prereq != nullptr) {
                cout << prereq->courseNumber << " - " << prereq->courseTitle;
            } else {
                cout << course->prerequisites[i];
            }

            if (i < course->prerequisites.size() - 1) {
                cout << ", ";
            }
        }

        cout << endl;
    }
}

// Display menu
void displayMenu() {
    cout << endl;
    cout << "Menu:" << endl;
    cout << "1. Load Data Structure" << endl;
    cout << "2. Print Course List" << endl;
    cout << "3. Print Course" << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

int main() {
    vector<Course> courses;
    bool dataLoaded = false;
    int choice = 0;

    cout << "Welcome to the course planner." << endl;

    while (choice != 9) {
        displayMenu();
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Invalid input. Please enter a number." << endl;
            continue;
        }

        if (choice == 1) {
            string fileName;

            cout << "Enter the file name: ";
            getline(cin >> ws, fileName);

            dataLoaded = loadCourses(fileName, courses);
        }
        else if (choice == 2) {
            if (!dataLoaded) {
                cout << "Error: Please load the course data first." << endl;
            } else {
                printCourseList(courses);
            }
        }
        else if (choice == 3) {
            if (!dataLoaded) {
                cout << "Error: Please load the course data first." << endl;
            } else {
                printCourseInformation(courses);
            }
        }
        else if (choice == 9) {
            cout << "Thank you for using the course planner!" << endl;
        }
        else {
            cout << choice << " is not a valid option." << endl;
        }
    }

    return 0;
}
