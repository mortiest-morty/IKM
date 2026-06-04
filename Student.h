#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>

// класс student - представляет одного ученика с именем и рейтингом
class Student {
private:
    std::string name;   // имя ученика
    int rating;         // рейтинг ученика (количество добрых дел)

public:
    Student();                         // Конструктор по умолчанию
    Student(const std::string& name);  // конструктор с параметром
    Student(const Student& other);     // конструктор копирования
    ~Student();                        // деструктор

    // Геттеры
    std::string getName() const;       // возвращает имя ученика
    int getRating() const;             // возвращает рейтинг ученика

    // Сеттеры
    void setName(const std::string& name);  // устанавливает имя ученика
    void setRating(int value);              // устанавливает рейтинг ученика


    // увеличивает рейтинг ученика на 1 (ученик сделал доброе дело)
    void incrementRating();

    // Перегрузка операторов

    Student& operator=(const Student& other);    // оператор присваивания - копирует данные из другого ученика
    bool operator==(const Student& other) const; // оператор сравнения (равенство) - сравнивает имя и рейтинг
    bool operator!=(const Student& other) const; // оператор сравнения (неравенство) - противоположность равенству

    // оператор вывода в поток - позволяет выводить ученика через cout
    friend std::ostream& operator<<(std::ostream& out, const Student& student);
};

#endif