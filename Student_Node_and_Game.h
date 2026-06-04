#ifndef STUDENT_NODE_AND_GAME_H
#define STUDENT_NODE_AND_GAME_H

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>   
#include <fstream>    
#include <ctime>      
#include <cstdlib>    
#include <sstream>    
#include <thread>
#include <chrono>

#include <windows.h>

#ifndef NOMINMAX
#define NOMINMAX
#endif

// ====================  ŒÕ—“¿Õ“€ ÷¬≈“Œ¬ ====================
#define COLOR_DEFAULT 7
#define COLOR_ERROR 4
#define COLOR_ARROW 6
#define COLOR_TITLE 11

// ==================== ‘”Õ ÷»» –¿¡Œ“€ — ÷¬≈“ŒÃ »  ŒÕ—ŒÀ‹ﬁ ====================

inline void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

inline void clearScreen() {
    system("cls");
}

inline void wait(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline void printComputer(int round) {
    setColor(COLOR_TITLE);
    std::cout << "              ,---------------------------,\n";
    std::cout << "              |  /---------------------\\  |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |      –‡ÛÌ‰ " << round;
    int spaces = 11 - std::to_string(round).length();
    for (int i = 0; i < spaces; i++) std::cout << " ";
    std::cout << "| |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              |  \\_____________________/  |\n";
    std::cout << "              |___________________________|\n";
    std::cout << "            ,---\\_____     []     _______/------,\n";
    std::cout << "          /         /______________\\           /|\n";
    std::cout << "        /___________________________________ /  | ___\n";
    std::cout << "        |                                   |   |    )\n";
    std::cout << "        |  _ _ _                 [-------]  |   |   (\n";
    std::cout << "        |  o o o                 [-------]  |  /    _)_\n";
    std::cout << "        |__________________________________ |/     /  /\n";
    std::cout << "    /-------------------------------------/|      ( )/\n";
    std::cout << "  /-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/ /\n";
    std::cout << "/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/-/ /\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    setColor(COLOR_DEFAULT);
}

class Student;

// ====================  À¿—— ”«À¿ —œ»— ¿ ====================

class StudentNode {
private:
    Student* data;
    StudentNode* prev;
    StudentNode* next;

public:
    StudentNode();
    StudentNode(Student* student);
    StudentNode(const std::string& name);
    ~StudentNode();

    Student* getData() { return data; }
    const Student* getData() const { return data; }
    StudentNode* getNext() const { return next; }
    StudentNode* getPrev() const { return prev; }

    void setData(Student* student);
    void setPrev(StudentNode* prevNode);
    void setNext(StudentNode* nextNode);

    std::string getName() const;
    int getRating() const;
    void incrementRating();
};

// ====================  À¿—— »√–€ ====================

class StudentGame {
private:
    StudentNode* head;
    StudentNode* current;
    int count;

public:
    StudentGame();
    StudentGame(const StudentGame& other);
    ~StudentGame();

    void clear();
    bool loadStudentsFromFile(const std::string& filename);
    void generateRandomStudents(int numStudents);
    void addStudent(const std::string& name);

    int getCount() const { return count; }
    bool isEmpty() const { return head == nullptr; }

    void printStudentsInline(std::ostream& out) const;
    void printResultsColumn(std::ostream& out) const;
    void printSortedByRating(std::ostream& out) const;
    void saveResultsToFile(const std::string& filename) const;

    void playGame(int rounds, int startRound = 1);
    int getRandomNumber() const;
    void moveClockwise(int steps);
    void moveCounterClockwise(int steps);

    StudentNode* getCurrent() const { return current; }
};

void task_16();
void showMenu();
void showRules();

#endif