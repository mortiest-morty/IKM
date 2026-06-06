#ifndef GAME_EXCEPTION_H
#define GAME_EXCEPTION_H

#include <exception>
#include <string>

// Свой класс исключений для игры
class GameException : public std::exception {
private:
    std::string message;

public:
    // Конструктор принимает сообщение об ошибке
    GameException(const std::string& msg) : message(msg) {}

    // Метод what() возвращает сообщение об ошибке
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
