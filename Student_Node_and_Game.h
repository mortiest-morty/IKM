#ifndef STUDENT_NODE_AND_GAME_H
#define STUDENT_NODE_AND_GAME_H

#include <string>
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

// ==================== КОНСТАНТЫ ЦВЕТОВ ====================
#define COLOR_DEFAULT 7   // белый/серый цвет для обычного текста
#define COLOR_ERROR 4     // красный цвет для сообщений об ошибках
#define COLOR_ARROW 6     // жёлтый цвет для подсветки текущего ученика
#define COLOR_TITLE 11    // голубой цвет для заголовков

// ==================== ФУНКЦИИ ДЛЯ РАБОТЫ С КОНСОЛЬЮ ====================

// Установка цвета текста в консоли
inline void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// Очистка экрана консоли
inline void clearScreen() {
    system("cls");
}

// Пауза на указанное количество миллисекунд
inline void wait(int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// Вывод картинки компьютера с номером текущего раунда
inline void printComputer(int round) {
    setColor(COLOR_TITLE);
    std::cout << "              ,---------------------------,\n";
    std::cout << "              |  /---------------------\\  |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |                       | |\n";
    std::cout << "              | |      Раунд " << round;
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

// Предварительное объявление класса Student
class Student;

// ==================== КЛАСС УЗЛА СПИСКА ====================

// Класс узла для двусвязного циклического списка
class StudentNode {
private:
    Student* data;      // указатель на объект Student
    StudentNode* prev;  // указатель на предыдущий узел
    StudentNode* next;  // указатель на следующий узел

public:
    StudentNode();                              // конструктор по умолчанию
    StudentNode(Student* student);              // конструктор с указателем на Student
    StudentNode(const std::string& name);       // конструктор с именем (создаёт нового Student)
    ~StudentNode();                             // деструктор

    Student* getData() { return data; }                     // получить данные узла
    //const Student* getData() const { return data; }         // получить данные (константная версия)
    StudentNode* getNext() const { return next; }           // получить следующий узел
    StudentNode* getPrev() const { return prev; }           // получить предыдущий узел
    //StudentNode* getHead() const { return head; }   // получить голову списка

    void setData(Student* student);             // установить данные узла
    void setPrev(StudentNode* prevNode);        // установить предыдущий узел
    void setNext(StudentNode* nextNode);        // установить следующий узел

    std::string getName() const;                // получить имя ученика
    int getRating() const;                      // получить рейтинг ученика
    void incrementRating();                     // увеличить рейтинг ученика на 1
};

// ==================== КЛАСС ИГРЫ ====================

// Класс для управления игрой с учениками
class StudentGame {
private:
    StudentNode* head;      // голова списка (первый ученик)
    StudentNode* current;   // текущий ученик (с которого начинается отсчёт)
    int count;              // количество учеников в списке
    int totalRounds;        // количество раундов

public:
    StudentGame();                              // конструктор по умолчанию
    StudentGame(const StudentGame& other);      // конструктор копирования
    ~StudentGame();                             // деструктор

    void clear();                               // очистка списка (удаление всех учеников)
    bool loadStudentsFromFile(const std::string& filename);  // загрузка учеников из файла
    void generateRandomStudents(int numStudents);            // генерация случайных учеников
    void addStudent(const std::string& name);                // добавление ученика в конец списка

    int getCount() const { return count; }      // получить количество учеников
    bool isEmpty() const { return head == nullptr; } // проверка, пуст ли список

    void printStudentsInline(std::ostream& out) const;   // вывод списка строкой (для игры)
    void printResultsColumn(std::ostream& out) const;    // вывод списка таблицей (для итогов)
    void printSortedByRating(std::ostream& out) const;   // вывод учеников по убыванию рейтинга
    void saveResultsToFile(const std::string& filename) const; // сохранение результатов в файл

    void playGame(int rounds, int startRound, bool& exitToMenu);       // запуск игры
    int getRandomNumber() const;                         // генерация случайного числа от -10 до 10
    void moveClockwise(int steps);                       // движение по часовой стрелке
    void moveCounterClockwise(int steps);                // движение против часовой стрелки

    StudentNode* getCurrent() const { return current; }  // получить текущего ученика

    StudentNode* getHead() const { return head; }       // получить текущего ученика
    int getTotalRounds() const { return totalRounds; }  // получить кол-во раундов
    void setTotalRounds(int rounds) { totalRounds = rounds; }  
};

// Глобальные функции
void task_16(StudentGame*& lastGame);
void showMenu(StudentGame*& lastGame);
void showRules();   // показать правила игры

#endif