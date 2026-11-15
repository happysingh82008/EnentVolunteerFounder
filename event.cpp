#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <exception>
#include <stdexcept>

using namespace std;

const string DB_FILE_PATH = "C:\\Users\\happy\\OneDrive\\CLion\\Desktop\\pycharm.py\\projects\\EnentVolunteerFounder\\database_students.txt";

class DatabaseException : public exception {
private:
    string msg;
public:
    DatabaseException(string m) : msg(m) {}
    const char* what() const noexcept override {
        return msg.c_str();
    }
};

class DuplicateRegistrationException : public exception {
public:
    const char* what() const noexcept override {
        return "[Error] Student ID or Email is already registered!";
    }
};

class DashboardInterface {
public:
    virtual void renderDashboard() const = 0; 
    virtual ~DashboardInterface() {}
};

class User : public DashboardInterface {
protected:
    string name;
    string email;
    long long password;

public:
    User() {
        name = "Default";
        email = "none@college.edu";
        password = 0;
    }
    User(string n, string em, long long pass) {
        name = n;
        email = em;
        password = pass;
    }

    virtual void displayBasicInfo() const {
        cout << "Name: " << name << " | Email: " << email << endl;
    }

    void renderDashboard() const override {
        cout << "[Generic User Dashboard] Welcome, " << name << endl;
    }

    friend istream& operator>>(istream& in, User& u) {
        cout << "Enter Full Name: ";
        in >> ws;
        getline(in, u.name);
        cout << "Enter College Email: ";
        in >> u.email;
        cout << "Enter Numeric Password: ";
        in >> u.password;
        return in;
    }
};

class Student : public User {
private:
    string skill;
    string appliedEvent;
    int studentId;
    string achievements;
    int experienceScore;
    static int globalStudentCount; 

public:
    Student() : User(), skill("General"), appliedEvent("None"), studentId(0), achievements("None"), experienceScore(0) {}

    Student(int id, string n, string sk, string em, long long pass, string ev, string ach, int score) 
        : User(n, em, pass), skill(sk), appliedEvent(ev), studentId(id), achievements(ach), experienceScore(score) {
        globalStudentCount++;
    }

    void setStudentDetails(int id, string eventName) {
        studentId = id;
        cout << "Enter Student Full Name: ";
        cin >> ws;
        getline(cin, name); 
        cout << "Enter Your Primary Skill (e.g., Coding, Management, Design): ";
        cin >> skill;
        cout << "Enter College Email: ";
        cin >> email;
        cout << "Enter Security Password (numeric): ";
        if (!(cin >> password)) {
            cin.clear();
            cin.ignore(10000, '\n');
            throw invalid_argument("[Error] Password must be a valid number!");
        }
        cout << "Enter Past Achievements / Experience Summary: ";
        cin >> ws;
        getline(cin, achievements);
        cout << "Enter Experience Score (1 to 10 for priority preference): ";
        if (!(cin >> experienceScore)) {
            cin.clear();
            cin.ignore(10000, '\n');
            experienceScore = 1;
        }

        appliedEvent = eventName;
        globalStudentCount++;
        cout << "-> Registration Successful for Event: " << appliedEvent << " (Score: " << experienceScore << ")\n";
    }

    friend ostream& operator<<(ostream& out, const Student& s) {
        out << "ID: " << s.studentId
            << " | Name: " << s.name
            << " | Skill: " << s.skill
            << " | Event: " << s.appliedEvent
            << " | Score: " << s.experienceScore
            << " | Achievements: " << s.achievements;
        return out;
    }

    bool operator==(const Student& other) const {
        return (this->studentId == other.studentId || this->email == other.email);
    }

    Student& operator++() {
        this->studentId++;
        return *this;
    }

    void renderDashboard() const override {
        cout << "\n==============================\n";
        cout << "      STUDENT DASHBOARD       \n";
        cout << "==============================\n";
        cout << "Student ID   : " << studentId << endl;
        cout << "Name         : " << name << endl;
        cout << "Email        : " << email << endl;
        cout << "Primary Skill: " << skill << endl;
        cout << "Applied Event: " << appliedEvent << endl;
        cout << "Score        : " << experienceScore << endl;
        cout << "Achievements : " << achievements << endl;
        cout << "==============================\n";
    }

    static int getTotalGlobalStudents() {
        return globalStudentCount;
    }

    int getId() const { return studentId; }
    string getName() const { return name; }
    string getSkill() const { return skill; }
    string getEmail() const { return email; }
    long long getPassword() const { return password; }
    string getAppliedEvent() const { return appliedEvent; }
    string getAchievements() const { return achievements; }
    int getExperienceScore() const { return experienceScore; }

    friend class Admin;
};

int Student::globalStudentCount = 0;

template <typename T>
struct Node {
    T data;
    Node* next;
};

template <typename T>
class PriorityVolunteerQueue {
private:
    Node<T>* frontNode;
    Node<T>* rearNode;
public:
    PriorityVolunteerQueue() : frontNode(nullptr), rearNode(nullptr) {}

    bool isEmpty() const { return frontNode == nullptr; }

    void enqueue(const T& val) {
        Node<T>* newNode = new Node<T>{val, nullptr};
        if (isEmpty() || val.getExperienceScore() > frontNode->data.getExperienceScore()) {
            newNode->next = frontNode;
            frontNode = newNode;
            if (!rearNode) rearNode = newNode;
        } else {
            Node<T>* curr = frontNode;
            while (curr->next && curr->next->data.getExperienceScore() >= val.getExperienceScore()) {
                curr = curr->next;
            }
            newNode->next = curr->next;
            curr->next = newNode;
            if (!newNode->next) rearNode = newNode;
        }
    }

    T dequeue() {
        if (isEmpty()) {
            throw underflow_error("[Error] Queue is empty!");
        }
        Node<T>* temp = frontNode;
        T val = temp->data;
        frontNode = frontNode->next;
        if (frontNode == nullptr) rearNode = nullptr;
        delete temp;
        return val;
    }
};

struct HashNode {
    Student studentData;
    HashNode* next;
};

class SkillHashTable {
private:
    static const int TABLE_SIZE = 10;
    HashNode* table[TABLE_SIZE];

    int hashFunction(string skill) const {
        int sum = 0;
        for (char c : skill) {
            sum += tolower(c);
        }
        return sum % TABLE_SIZE;
    }

public:
    SkillHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    void insert(Student s) {
        int index = hashFunction(s.getSkill());
        HashNode* newNode = new HashNode{s, nullptr};
        if (!table[index]) {
            table[index] = newNode;
        } else {
            HashNode* temp = table[index];
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    void searchBySkill(string skillQuery) const {
        int index = hashFunction(skillQuery);
        HashNode* temp = table[index];
        bool found = false;
        cout << "\n--- Volunteers matching skill [" << skillQuery << "] ---\n";
        while (temp) {
            if (temp->studentData.getSkill() == skillQuery) {
                cout << " -> " << temp->studentData << endl;
                found = true;
            }
            temp = temp->next;
        }
        if (!found) {
            cout << "[Notice] No volunteers found with skill: " << skillQuery << endl;
        }
    }

    ~SkillHashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            HashNode* curr = table[i];
            while (curr) {
                HashNode* nextNode = curr->next;
                delete curr;
                curr = nextNode;
            }
        }
    }
};

struct LogNode {
    string actionDescription;
    string timestamp;
    LogNode* prev;
    LogNode* next;
};

class DoublyLinkedListLogs {
private:
    LogNode* head;
    LogNode* tail;

public:
    DoublyLinkedListLogs() : head(nullptr), tail(nullptr) {}

    void addLog(string desc, string time) {
        LogNode* newNode = new LogNode{desc, time, nullptr, nullptr};
        if (!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    void displayLogsForward() const {
        if (!head) {
            cout << "\n[Notice] No audit logs recorded yet.\n";
            return;
        }
        cout << "\n--- System Activity Audit Logs (Chronological) ---\n";
        LogNode* temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". [" << temp->timestamp << "] " << temp->actionDescription << endl;
            temp = temp->next;
        }
    }

    ~DoublyLinkedListLogs() {
        LogNode* curr = head;
        while (curr) {
            LogNode* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
    }
};

struct BSTNode {
    int eventId;
    string eventName;
    string venue;
    BSTNode* left;
    BSTNode* right;
};

class EventBST {
private:
    BSTNode* root;

    BSTNode* insertRec(BSTNode* node, int id, string name, string loc) {
        if (!node) return new BSTNode{id, name, loc, nullptr, nullptr};
        if (id < node->eventId) node->left = insertRec(node->left, id, name, loc);
        else if (id > node->eventId) node->right = insertRec(node->right, id, name, loc);
        return node;
    }

    void inorderRec(BSTNode* node) const {
        if (node) {
            inorderRec(node->left);
            cout << "Event ID : " << node->eventId << endl;
            cout << "Title    : " << node->eventName << endl;
            cout << "Venue    : " << node->venue << endl;
            cout << "-----------------------------------\n";
            inorderRec(node->right);
        }
    }

    string searchRec(BSTNode* node, int id) const {
        if (!node) return "";
        if (node->eventId == id) return node->eventName;
        if (id < node->eventId) return searchRec(node->left, id);
        return searchRec(node->right, id);
    }

    void destroyTree(BSTNode* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

public:
    EventBST() : root(nullptr) {}
    ~EventBST() { destroyTree(root); }

    void addEvent(int id, string name, string loc) {
        root = insertRec(root, id, name, loc);
    }

    void displayEvents() const {
        if (!root) {
            cout << "\n[Notice] No events found in BST!\n";
            return;
        }
        cout << "\n--- Upcoming Events (BST In-Order) ---\n";
        inorderRec(root);
    }

    string getEventNameById(int id) const {
        string name = searchRec(root, id);
        if (name == "") throw out_of_range("[Error] Invalid Event ID!");
        return name;
    }
};

class Admin : public User {
private:
    string adminCode;

public:
    Admin() : User("SuperAdmin", "admin@college.edu", 9999) {
        adminCode = "1234";
    }

    bool authenticateAdmin() {
        string inputCode;
        cout << "\n[Security Check] Enter Admin Passcode (default 1234): ";
        cin >> inputCode;
        if (inputCode == adminCode) {
            cout << "-> Access Granted! Welcome, Admin.\n";
            return true;
        }
        cout << "-> Access Denied! Incorrect passcode.\n";
        return false;
    }

    void saveToFile(const Student* arr, int count) {
        ofstream outFile(DB_FILE_PATH);
        if (!outFile) {
            throw DatabaseException("[Error] Unable to open file for writing database at specified path!");
        }
        for (int i = 0; i < count; i++) {
            outFile << arr[i].getId() << "|"
                    << arr[i].getName() << "|"
                    << arr[i].getSkill() << "|"
                    << arr[i].getEmail() << "|"
                    << arr[i].getPassword() << "|"
                    << arr[i].getAppliedEvent() << "|"
                    << arr[i].getAchievements() << "|"
                    << arr[i].getExperienceScore() << "\n";
        }
        outFile.close();
        cout << "-> Successfully persisted data to specified absolute path using fstream.\n";
    }

    void loadFromFile(Student* arr, int& count, SkillHashTable& hashTbl, PriorityVolunteerQueue<Student>& q) {
        ifstream inFile(DB_FILE_PATH);
        if (!inFile) return; 

        string line;
        count = 0;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, s_name, s_skill, s_email, passStr, eventName, achievements, scoreStr;
            if (getline(ss, idStr, '|') &&
                getline(ss, s_name, '|') &&
                getline(ss, s_skill, '|') &&
                getline(ss, s_email, '|') &&
                getline(ss, passStr, '|') &&
                getline(ss, eventName, '|') &&
                getline(ss, achievements, '|') &&
                getline(ss, scoreStr)) {
                
                int id = stoi(idStr);
                long long pass = stoll(passStr);
                int score = stoi(scoreStr);
                arr[count] = Student(id, s_name, s_skill, s_email, pass, eventName, achievements, score);
                hashTbl.insert(arr[count]);
                q.enqueue(arr[count]);
                count++;
            }
        }
        inFile.close();
    }

    void viewAllStudents(const Student* arr, int count) const {
        if (count == 0) {
            cout << "\n[Notice] No registrations recorded.\n";
            return;
        }
        cout << "\n--- All Registered Students (Sorted by Registration) ---\n";
        for (int i = 0; i < count; i++) {
            cout << "[" << i + 1 << "] " << arr[i] << endl; 
        }
        cout << "Total Active Volunteers (Static Count): " << Student::getTotalGlobalStudents() << endl;
    }

    void processQueue(PriorityVolunteerQueue<Student>& q, DoublyLinkedListLogs& logs) {
        if (q.isEmpty()) {
            cout << "\n[Notice] Volunteer queue is empty!\n";
            return;
        }
        cout << "\n--- Processing Queue Applications (First Preference to Higher Scores) ---\n";
        while (!q.isEmpty()) {
            Student s = q.dequeue();
            cout << "Approved Volunteer: " << s.getName() << " | Event: " << s.getAppliedEvent() << " | Score: " << s.getExperienceScore() << endl;
            logs.addLog("Processed priority applicant: " + s.getName(), "2026-11-14");
        }
    }
};

int main() {
    EventBST eventSystem;
    eventSystem.addEvent(101, "SARTHI", "IEEE Auditorium");
    eventSystem.addEvent(102, "AARAMBH", "CO-DEV Lab");
    eventSystem.addEvent(103, "STUDENT_GRAFEST", "Graphic Era Grounds");

    const int MAX_CAPACITY = 100;
    Student* studentRecords = new Student[MAX_CAPACITY];
    int registrationCount = 0;

    PriorityVolunteerQueue<Student> applicationQueue;
    SkillHashTable skillHash;
    DoublyLinkedListLogs auditLogs;
    Admin systemAdmin;

    try {
        systemAdmin.loadFromFile(studentRecords, registrationCount, skillHash, applicationQueue);
    } catch (const exception& e) {
        cout << e.what() << endl;
    }

    int choice;
    do {
        cout << "\n============================================\n";
        cout << "   COLLEGE EVENT VOLUNTEER MATCHER SYSTEM   \n";
        cout << "============================================\n";
        cout << "1. View Upcoming Events (BST)\n";
        cout << "2. Register as Volunteer (With Achievements & Priority)\n";
        cout << "3. Admin: View All Registrations\n";
        cout << "4. Admin: Process Volunteer Queue (Priority Based)\n";
        cout << "5. Search Volunteers by Skill (Hash Table)\n";
        cout << "6. View System Audit Logs (Doubly Linked List)\n";
        cout << "7. Save Database to File (fstream)\n";
        cout << "8. Exit Program\n";
        cout << "Enter your choice (1-8): ";

        if (!(cin >> choice)) {
            cout << "[Error] Invalid input type. Please enter a valid number.\n";
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        try {
            switch (choice) {
            case 1: {
                eventSystem.displayEvents();
                break;
            }
            case 2: {
                if (registrationCount >= MAX_CAPACITY) {
                    cout << "[Error] Storage capacity full!\n";
                    break;
                }
                eventSystem.displayEvents();
                int evId;
                cout << "\nEnter Event ID to apply: ";
                cin >> evId;
                string eventName = eventSystem.getEventNameById(evId);

                Student newStudent;
                newStudent.setStudentDetails(registrationCount + 1, eventName);

                bool duplicate = false;
                for (int i = 0; i < registrationCount; i++) {
                    if (studentRecords[i] == newStudent) {
                        duplicate = true;
                        break;
                    }
                }
                if (duplicate) {
                    throw DuplicateRegistrationException();
                }

                studentRecords[registrationCount] = newStudent;
                skillHash.insert(newStudent);
                applicationQueue.enqueue(newStudent);
                registrationCount++;

                auditLogs.addLog("New student registered: " + newStudent.getName(), "2026-11-14");
                systemAdmin.saveToFile(studentRecords, registrationCount);
                break;
            }
            case 3: {
                if (systemAdmin.authenticateAdmin()) {
                    systemAdmin.viewAllStudents(studentRecords, registrationCount);
                }
                break;
            }
            case 4: {
                if (systemAdmin.authenticateAdmin()) {
                    systemAdmin.processQueue(applicationQueue, auditLogs);
                }
                break;
            }
            case 5: {
                string sk;
                cout << "Enter skill keyword to search (e.g., Coding, Management): ";
                cin >> sk;
                skillHash.searchBySkill(sk);
                break;
            }
            case 6: {
                auditLogs.displayLogsForward();
                break;
            }
            case 7: {
                if (systemAdmin.authenticateAdmin()) {
                    systemAdmin.saveToFile(studentRecords, registrationCount);
                }
                break;
            }
            case 8: {
                systemAdmin.saveToFile(studentRecords, registrationCount);
                cout << "\nExiting application. All data safely saved. Good luck!\n";
                break;
            }
            default: {
                cout << "[Error] Invalid option! Choose between 1 and 8.\n";
            }
            }
        } catch (const exception& ex) {
            cout << "\n[Exception Caught]: " << ex.what() << endl;
        }

    } while (choice != 8);

    delete[] studentRecords;
    return 0;
}