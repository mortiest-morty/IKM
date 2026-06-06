#include "Student_Node_and_Game.h"
#include "Student.h"

// ==================== РЕАЛИЗАЦИЯ КЛАССА STUDENTNODE ====================

// Конструктор по умолчанию - создаёт пустой узел
StudentNode::StudentNode() : data(nullptr), prev(nullptr), next(nullptr) {}

// Конструктор с указателем на Student
StudentNode::StudentNode(Student* student) : data(student), prev(nullptr), next(nullptr) {}

// Конструктор с именем - создаёт нового Student и сохраняет указатель
StudentNode::StudentNode(const std::string& name) : data(new Student(name)), prev(nullptr), next(nullptr) {}

// Деструктор - удаляет объект Student
StudentNode::~StudentNode() {
    delete data;
}

// Установка данных узла
void StudentNode::setData(Student* student) {
    data = student;
}

// Установка предыдущего узла
void StudentNode::setPrev(StudentNode* prevNode) {
    prev = prevNode;
}

// Установка следующего узла
void StudentNode::setNext(StudentNode* nextNode) {
    next = nextNode;
}

// Получение имени ученика (если данные есть)
std::string StudentNode::getName() const {
    return data ? data->getName() : "";
}

// Получение рейтинга ученика (если данные есть)
int StudentNode::getRating() const {
    return data ? data->getRating() : 0;
}

// Увеличение рейтинга ученика (если данные есть)
void StudentNode::incrementRating() {
    if (data) data->incrementRating();
}