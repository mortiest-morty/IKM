#include "Student.h"


// ==================== РЕАЛИЗАЦИЯ КЛАССА STUDENT ====================

// Конструктор по умолчанию - создаёт ученика с пустым именем и рейтингом 0
Student::Student() : name(""), rating(0) {}

// Конструктор с параметром - создаёт ученика с указанным именем и рейтингом 0
Student::Student(const std::string& name) : name(name), rating(0) {}

// Конструктор копирования - создаёт копию другого ученика
Student::Student(const Student& other) : name(other.name), rating(other.rating) {}

// Деструктор
Student::~Student() {}

// Возвращает имя ученика
std::string Student::getName() const {
    return name;
}

// Возвращает рейтинг ученика
int Student::getRating() const {
    return rating;
}

// Устанавливает имя ученика
void Student::setName(const std::string& name) {
    this->name = name;
}

// Устанавливает рейтинг ученика
void Student::setRating(int value) {
    rating = value;
}

// Увеличивает рейтинг ученика на 1
void Student::incrementRating() {
    rating++;
}

// Оператор присваивания
Student& Student::operator=(const Student& other) {
    if (this != &other) {
        name = other.name;
        rating = other.rating;
    }
    return *this;
}

// Оператор сравнения (равенство)
bool Student::operator==(const Student& other) const {
    return (name == other.name && rating == other.rating);
}

// Оператор сравнения (неравенство)
bool Student::operator!=(const Student& other) const {
    return !(*this == other);
}

// Оператор вывода в поток
std::ostream& operator<<(std::ostream& out, const Student& student) {
    out << student.name << " (рейтинг: " << student.rating << ")";
    return out;
}