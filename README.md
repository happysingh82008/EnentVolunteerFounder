# 🎓 College Event Volunteer Matcher System

A robust, production-grade **C++ Console Application** designed to streamline college event management and intelligently match student volunteers based on their skills, past achievements, and priority scores. Built strictly using object-oriented programming principles and low-level custom data structures **without relying on the C++ Standard Template Library (STL) core containers for application logic**.

---

## 🚀 Key Features

* **Advanced Object-Oriented Design (OOPs):** Implements inheritance, polymorphism, abstract classes (`DashboardInterface`), and pure virtual functions.
* **Custom Data Structures & Algorithms:**
  * **Binary Search Tree (BST):** Fast event lookup and in-order traversal display ($O(\log n)$ search efficiency).
  * **Custom Hash Table with Chaining:** Instant skill-based volunteer querying using ASCII character hashing and bucket chaining.
  * **Priority Queue (Template-based):** Dynamically ranks and prioritizes volunteers based on past achievements and experience scores (first preference to higher scores).
  * **Doubly Linked List:** Chronological system activity audit logs.
* **Persistent File Handling (`fstream`):** Automatically saves and loads all student registrations, preferences, and queue data to/from the absolute path: `C:\Users\happy\OneDrive\CLion\Desktop\pycharm.py\projects\EnentVolunteerFounder\database_students.txt`.
* **Operator Overloading:** Custom operator implementations (`<<`, `==`, `++`, `>>`) for seamless object printing, duplicate detection, and ID incrementation.
* **Robust Exception Handling:** Custom exception classes (`DatabaseException`, `DuplicateRegistrationException`) ensuring fault-tolerant input validation.
* **Static Members & Functions:** Global tracking of active student registrations across instances.

---

## 🛠️ Tech Stack & Concepts

* **Language:** C++ (Standard C++11 / C++14)
* **Paradigms:** Object-Oriented Programming, Generic Programming (Class Templates), Procedural Modularization.
* **File Operations:** Standard C++ File Streams (`<fstream>`, `<sstream>`).

---

## 📂 Project Structure

```text
EnentVolunteerFounder/
│
├── EventMatcher.cpp         # Main source code containing all classes and logic
├── database_students.txt    # Persistent storage file for registered volunteers
└── README.md                # Project documentation
Getting Started & Installation
Prerequisites
Make sure you have a C++ compiler installed (like g++ via MinGW or GCC).

Compilation & Execution (Terminal / PowerShell)
Open your terminal in the project directory (EnentVolunteerFounder).

Compile the source code:g++ EventMatcher.cpp -o EventMatcher.exe
Run the executable:

Bash
./EventMatcher.exe
View Upcoming Events (BST): Displays scheduled college events organized through the Binary Search Tree.

Register as Volunteer: Interactive form capturing student details, primary skill, college email, secure numeric password, past achievements, and an experience score (1-10) for priority queue ranking.

Admin: View All Registrations: Secure admin login (Passcode: 1234) to view all registered volunteers sorted by registration order.

Admin: Process Volunteer Queue: Automatically processes and approves applicants, giving first preference to candidates with higher experience scores.

Search Volunteers by Skill (Hash Table): Instantly queries volunteers matching a specific skill keyword.

View System Audit Logs (Doubly Linked List): Displays chronological audit records of all system actions.

Save Database to File: Manually triggers fstream persistence to the database text file.

Exit Program: Safely saves data and terminates the application.
