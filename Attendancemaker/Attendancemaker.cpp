#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

class Student {
private:
    string matricNo;
    string name;
    bool present;

public:
    Student(const string& matricNo, const string& name) : matricNo(matricNo), name(name), present(false) {} // default constructor

    // getters and setters
    const string& getMatricNo() const {
        return matricNo;
    }

    // setters are not needed for matricNo as it is a constant value
    const string& getName() const {
        return name;
    }

    bool isPresent() const {
        return present;
    }

    void markPresent() {
        present = true;
    }

    void markAbsent() {
        present = false;
    }
};

class AttendanceManager {
private:
    vector<Student> students;

public:
    void addStudent(const string& matricNo, const string& name) {
        students.push_back(Student(matricNo, name));
    }

    void loadFromCSV(const string& filename) {
        ifstream inputFile(filename);
        if (!inputFile.is_open()) {
            cout << "Failed to open file: " << filename << endl;
            return;
        }

        string line;
        while (getline(inputFile, line)) {
            stringstream ss(line);
            string matricNo, name, status;
            getline(ss, matricNo, ',');
            getline(ss, name, ',');
            getline(ss, status, ',');

            if (status == "Present") {
                students.push_back(Student(matricNo, name));
                students.back().markPresent();
            }
            else {
                students.push_back(Student(matricNo, name));
                students.back().markAbsent();
            }
        }

        inputFile.close();
    }

    void saveToCSV(const string& filename) {
        ofstream outputFile(filename);
        if (!outputFile.is_open()) {
            cout << "Failed to open file: " << filename << endl;
            return;
        }

        for (const Student& student : students) {
            outputFile << student.getMatricNo() << "," << student.getName() << "," << (student.isPresent() ? "Present" : "Absent") << endl;
        }

        outputFile.close();
    }

    void searchByMatricNo(const string& matricNo) {
        auto it = find_if(students.begin(), students.end(), [&](const Student& s) {
            return s.getMatricNo() == matricNo;
            });

        if (it != students.end()) {
            cout << "Student found: " << it->getName() << " (Matric No: " << it->getMatricNo() << ")" << endl;
            cout << "Attendance status: " << (it->isPresent() ? "Present" : "Absent") << endl;
        }
        else {
            cout << "Student with matriculation number " << matricNo << " not found." << endl;
        }
    }

    void markPresent(const string& matricNo) {
        auto it = find_if(students.begin(), students.end(), [&](const Student& s) {
            return s.getMatricNo() == matricNo;
            });

        if (it != students.end()) {
            it->markPresent();
            cout << "Attendance marked present for: " << it->getName() << endl;
        }
        else {
            cout << "Student with matriculation number " << matricNo << " not found." << endl;
        }
    }

    void markAbsent(const string& matricNo) {
        auto it = find_if(students.begin(), students.end(), [&](const Student& s) {
            return s.getMatricNo() == matricNo;
            });

        if (it != students.end()) {
            it->markAbsent();
            cout << "Attendance marked absent for: " << it->getName() << endl;
        }
        else {
            cout << "Student with matriculation number " << matricNo << " not found." << endl;
        }
    }
};

int main() {
    AttendanceManager attendanceManager;

    // existing data from CSV file
    attendanceManager.loadFromCSV("attendance.csv");

    int choice;
    string searchMatricNo;

    cout << "Menu:" << endl;
    cout << "1. Search for student by matriculation number" << endl;
    cout << "2. Mark student present" << endl;
    cout << "3. Mark student absent" << endl;
    cout << "4. Exit" << endl;
    while (true) {
        cout << "\nEnter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter matriculation number to search: ";
            cin >> searchMatricNo;
            attendanceManager.searchByMatricNo(searchMatricNo);
            break;
        case 2:
            cout << "Enter matriculation number to mark present: ";
            cin >> searchMatricNo;
            attendanceManager.markPresent(searchMatricNo);
            attendanceManager.saveToCSV("attendance.csv");
            break;
        case 3:
            cout << "Enter matriculation number to mark absent: ";
            cin >> searchMatricNo;
            attendanceManager.markAbsent(searchMatricNo);
            attendanceManager.saveToCSV("attendance.csv");
            break;
        case 4:
            cout << "Exiting program." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
