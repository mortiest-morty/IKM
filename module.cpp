#include "Student_Node_and_Game.h"
#include "Student.h"
#include "GameException.h"

// ==================== РЕАЛИЗАЦИЯ КЛАССА STUDENTGAME ====================

// Конструктор по умолчанию - инициализирует пустую игру
StudentGame::StudentGame() : head(nullptr), current(nullptr), count(0), totalRounds(0) {}

// Конструктор копирования - создаёт глубокую копию другой игры
StudentGame::StudentGame(const StudentGame& other) : head(nullptr), current(nullptr), count(0), totalRounds(other.totalRounds) {
    if (!other.isEmpty()) {
        // Копируем всех учеников по имёнам
        StudentNode* otherCurrentNode = other.head;
        do {
            addStudent(otherCurrentNode->getName());
            otherCurrentNode = otherCurrentNode->getNext();
        } while (otherCurrentNode != other.head);

        // Копируем рейтинги
        StudentNode* thisCurrent = head;
        otherCurrentNode = other.head;
        do {
            thisCurrent->getData()->setRating(otherCurrentNode->getRating());
            thisCurrent = thisCurrent->getNext();
            otherCurrentNode = otherCurrentNode->getNext();
        } while (otherCurrentNode != other.head);

        // Восстанавливаем позицию текущего ученика
        if (other.current != nullptr) {
            int steps = 0;
            StudentNode* temp = other.head;
            while (temp != other.current) {
                steps++;
                temp = temp->getNext();
            }
            thisCurrent = head;
            for (int i = 0; i < steps; i++) {
                thisCurrent = thisCurrent->getNext();
            }
            current = thisCurrent;
        }
    }
}

// Деструктор - освобождает память
StudentGame::~StudentGame() {
    clear();
}

// Очистка списка - удаляет всех учеников
void StudentGame::clear() {
    if (isEmpty()) return;

    // Разрываем циклическую связь (превращаем круг в линейный список)
    StudentNode* last = head->getPrev();
    last->setNext(nullptr);

    // Удаляем все узлы последовательно
    StudentNode* currentPtr = head;
    while (currentPtr != nullptr) {
        StudentNode* next = currentPtr->getNext();
        delete currentPtr;
        currentPtr = next;
    }

    // Сбрасываем указатели и счётчики
    head = nullptr;
    current = nullptr;
    count = 0;
    totalRounds = 0;
}

// Загрузка учеников из файла (одно имя на строку)
bool StudentGame::loadStudentsFromFile(const std::string& filename) {
    using namespace std;

    ifstream file(filename);
    if (!file.is_open()) {
        setColor(COLOR_ERROR);
        cout << "ошибка: не удалось открыть файл " << filename << endl;
        setColor(COLOR_DEFAULT);
        return false;
    }

    string line;
    while (getline(file, line)) {
        // Удаляем пробелы в начале и конце строки
        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == string::npos) continue;

        size_t end = line.find_last_not_of(" \t\r\n");
        line = line.substr(start, end - start + 1);

        if (!line.empty()) {
            addStudent(line);
        }
    }

    file.close();
    return true;
}

// Генерация случайных учеников из предопределённого списка (100 имён)
void StudentGame::generateRandomStudents(int numStudents) {
    using namespace std;

    clear();

    // Статический массив со 100 именами
    const char* randomNames[] = {
        "Ваня", "Евлампий", "Юлик", "Ярик", "Миша", "Добрыня", "Даня", "Петя",
        "Владик", "Макар", "Андрей", "Антон", "Алек", "Игорь", "Алмаз",
        "Иннокентий", "Веник", "Артурик", "Вася", "Боря", "Яков", "Никита",
        "Виталик", "Лёва", "Игнат", "Маша", "Аглая", "Алиса", "Лиза", "Даша",
        "Марфа", "Кира", "Арина", "Карина", "Лия", "Люба", "Галя", "Надя",
        "Аня", "Света", "Олеся", "Вера", "Лариса", "Ася", "Злата", "Альбина",
        "Нина", "Анжела", "Катя", "Рая", "Рома", "Коля", "Саша", "Женя", "Лена",
        "Оля", "Таня", "Ира","Наташа", "Кристина", "Виктор", "Сергей", "Дмитрий",
        "Алексей", "Максим", "Артём", "Илья", "Кирилл", "Тимофей", "Егор", "Матвей",
        "Семён", "Глеб","Богдан", "Руслан", "Олег", "Вадим", "Данил", "Захар", "Платон",
        "Степан", "Фёдор", "Юрий", "Стас", "Эдуард", "Родион", "Тарас", "Филипп",
        "Ярослав", "Арина", "Вероника", "Диана", "Ева", "Жанна", "Зоя", "Камилла",
        "Лада", "Милана", "Полина", "Рита"
    };

    int namesCount = sizeof(randomNames) / sizeof(randomNames[0]);  // = 100

    srand((unsigned int)time(nullptr));

    // Добавляем случайных учеников
    for (int i = 0; i < numStudents; i++) {
        int randomIndex = rand() % namesCount;
        try {
            addStudent(randomNames[randomIndex]);
        }
        catch (const GameException& e) {
            setColor(COLOR_ERROR);
            cout << e.what() << endl;
            setColor(COLOR_DEFAULT);
            return;
        }
    }

    setColor(COLOR_TITLE);
    cout << "\nсгенерировано " << count << " учеников случайным образом" << endl;
    setColor(COLOR_DEFAULT);
}

// Добавление ученика в конец циклического списка
void StudentGame::addStudent(const std::string& name) {
    using namespace std;

    StudentNode* newNode = nullptr;

    // Обработка ошибки выделения памяти
    try {
        newNode = new StudentNode(name);
    }
    catch (std::bad_alloc& e) {
        throw GameException("Ошибка: недостаточно памяти для добавления ученика");
    }

    if (isEmpty()) {
        // Первый узел: замыкаем на себя
        newNode->setNext(newNode);
        newNode->setPrev(newNode);
        head = newNode;
        current = newNode;
        count = 1;
        return;
    }

    // Вставляем новый узел перед head (в конец списка)
    StudentNode* last = head->getPrev();

    newNode->setNext(head);
    newNode->setPrev(last);

    last->setNext(newNode);
    head->setPrev(newNode);

    count++;
}

// Вывод списка учеников в строку (текущий подсвечивается жёлтым)
void StudentGame::printStudentsInline(std::ostream& out) const {
    using namespace std;

    if (isEmpty()) {
        out << "список учеников пуст";
        return;
    }

    StudentNode* currentPtr = head;
    do {
        // Подсветка текущего ученика при выводе в консоль
        if (&out == &cout && currentPtr == current) {
            setColor(COLOR_ARROW);
            out << currentPtr->getName() << " (" << currentPtr->getRating() << ")";
            setColor(COLOR_DEFAULT);
        }
        else {
            out << currentPtr->getName() << " (" << currentPtr->getRating() << ")";
        }
        currentPtr = currentPtr->getNext();
        if (currentPtr != head) {
            out << " - ";
        }
    } while (currentPtr != head);
}

// Вывод списка учеников в виде таблицы
void StudentGame::printResultsColumn(std::ostream& out) const {
    using namespace std;

    if (isEmpty()) {
        out << "нет учеников" << endl;
        return;
    }

    setColor(COLOR_TITLE);
    out << "\n+----------------------+----------+\n";
    out << "|        имя           | рейтинг  |\n";
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);

    StudentNode* currentPtr = head;
    do {
        out << "| " << currentPtr->getName();
        // Выравнивание по левому краю
        int nameLen = currentPtr->getName().length();
        for (int i = nameLen; i < 20; i++) out << " ";
        out << " |    " << currentPtr->getRating();
        // Выравнивание рейтинга
        if (currentPtr->getRating() < 10) out << "     |\n";
        else if (currentPtr->getRating() < 100) out << "    |\n";
        else out << "   |\n";
        currentPtr = currentPtr->getNext();
    } while (currentPtr != head);

    setColor(COLOR_TITLE);
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);
}

// Движение по часовой стрелке (вперёд)
void StudentGame::moveClockwise(int steps) {
    if (isEmpty() || steps == 0) return;

    for (int i = 0; i < steps; i++) {
        current = current->getNext();
    }
}

// Движение против часовой стрелки (назад)
void StudentGame::moveCounterClockwise(int steps) {
    if (isEmpty() || steps == 0) return;

    for (int i = 0; i < steps; i++) {
        current = current->getPrev();
    }
}

// Генерация случайного числа от -10 до 10
int StudentGame::getRandomNumber() const {
    return rand() % 21 - 10;
}

// Основная игровая логика - проведение раундов
void StudentGame::playGame(int rounds, int startRound, bool& exitToMenu) {
    using namespace std;

    exitToMenu = false;  // инициализация флага выхода

    if (isEmpty()) {
        setColor(COLOR_ERROR);
        cout << "нет учеников для игры!" << endl;
        setColor(COLOR_DEFAULT);
        return;
    }

    srand((unsigned int)time(nullptr));

    for (int i = 0; i < rounds; i++) {
        int round = startRound + i;

        // Анимация перед раундом
        clearScreen();
        printComputer(round);
        wait(2000);
        clearScreen();

        // Протокол игры
        setColor(COLOR_TITLE);
        cout << "\n========== протокол игры ==========" << endl;
        setColor(COLOR_DEFAULT);
        cout << "список учеников: ";
        printStudentsInline(cout);
        cout << endl;

        int randomNum = getRandomNumber();

        setColor(COLOR_TITLE);
        cout << "\n-------------------------------------\n";
        cout << "~(^_^)~ " << " раунд " << round << "  ~(^_^)~\n";
        cout << "-------------------------------------\n" << endl;
        setColor(COLOR_DEFAULT);
        cout << "  выпавшее число: " << randomNum << endl;

        int steps = abs(randomNum);

        // Определяем направление движения
        if (randomNum > 0) {
            cout << "  направление: по часовой стрелке (+" << steps << " шагов)" << endl;
            moveClockwise(steps);
        }
        else if (randomNum < 0) {
            cout << "  направление: против часовой стрелки (" << steps << " шагов)" << endl;
            moveCounterClockwise(steps);
        }
        else {
            cout << "  направление: остаёмся на месте" << endl;
        }

        // Ученик делает доброе дело
        cout << "  текущий ученик (на котором остановился счёт): ";
        setColor(COLOR_ARROW);
        cout << current->getName() << endl;
        setColor(COLOR_DEFAULT);

        current->incrementRating();

        cout << "  ^-^ доброе дело сделал: " << current->getName() << endl;
        cout << "  рейтинг после доброго дела: " << current->getRating() << endl;

        totalRounds++;

        // Меню после раунда 
        if (i < rounds - 1) {
            int menuChoice = 2;
            while (menuChoice == 2) {
                while (true) {
                    setColor(COLOR_TITLE);
                    cout << "\n====== *-* МЕНЮ *-* ======" << endl;
                    setColor(COLOR_DEFAULT);
                    cout << "1 - следующий раунд" << endl;
                    cout << "2 - показать статистику" << endl;
                    cout << "0 - выход в главное меню" << endl;
                    cout << "\nвведите цифру: ";

                    string inputStr;
                    getline(cin, inputStr);

                    if (inputStr.length() == 1 && isdigit(inputStr[0])) {
                        menuChoice = stoi(inputStr);
                        if (menuChoice == 0 || menuChoice == 1 || menuChoice == 2) {
                            break;
                        }
                    }
                    setColor(COLOR_ERROR);
                    cout << "ошибка! введите 0, 1 или 2.\n";
                    setColor(COLOR_DEFAULT);
                }

                // Показать статистику
                if (menuChoice == 2) {
                    setColor(COLOR_TITLE);
                    cout << "\n========== ТЕКУЩАЯ СТАТИСТИКА ==========\n" << endl;
                    setColor(COLOR_DEFAULT);

                    cout << "текущий список учеников: ";
                    printStudentsInline(cout);
                    cout << endl << endl;

                    printSortedByRating(cout);

                    setColor(COLOR_ARROW);
                    cout << "\nсыграно раундов: " << totalRounds << endl;
                    setColor(COLOR_DEFAULT);
                    cout << "\n";
                    continue;  // возвращаемся в меню
                }

                // Выход в главное меню
                if (menuChoice == 0) {
                    exitToMenu = true;
                    return;
                }
                // menuChoice == 1 - следующий раунд
            }
        }
    }
}

// Вывод учеников в порядке убывания рейтинга 
void StudentGame::printSortedByRating(std::ostream& out) const {
    using namespace std;

    if (isEmpty()) {
        out << "нет учеников" << endl;
        return;
    }

    // Создаём динамические массивы
    string* names = new string[count];
    int* ratings = new int[count];

    // Заполняем массивы данными из списка
    StudentNode* currentPtr = head;
    for (int i = 0; i < count; i++) {
        names[i] = currentPtr->getName();
        ratings[i] = currentPtr->getRating();
        currentPtr = currentPtr->getNext();
    }

    // Сортировка пузырьком (по убыванию рейтинга)
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (ratings[j] < ratings[j + 1]) {
                int tempRating = ratings[j];
                ratings[j] = ratings[j + 1];
                ratings[j + 1] = tempRating;
                string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }

    // Вывод таблицы
    setColor(COLOR_TITLE);
    out << "\n+----------------------+----------+\n";
    out << "|        имя           | рейтинг  |\n";
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);

    for (int i = 0; i < count; i++) {
        out << "| " << names[i];
        int nameLen = names[i].length();
        for (int j = nameLen; j < 20; j++) out << " ";
        out << " |    " << ratings[i];
        if (ratings[i] < 10) out << "     |\n";
        else if (ratings[i] < 100) out << "    |\n";
        else out << "   |\n";
    }

    setColor(COLOR_TITLE);
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);

    // Освобождаем память
    delete[] names;
    delete[] ratings;
}

// Сохранение результатов в файл
void StudentGame::saveResultsToFile(const std::string& filename) const {
    using namespace std;

    ofstream file(filename);
    if (!file.is_open()) {
        setColor(COLOR_ERROR);
        cout << "ошибка: не удалось создать файл " << filename << endl;
        setColor(COLOR_DEFAULT);
        return;
    }

    file << "========== итоговые результаты ==========" << endl;
    file << "\nфинальный список учеников:" << endl;

    file << "+----------------------+----------+\n";
    file << "|        имя           | рейтинг  |\n";
    file << "+----------------------+----------+\n";

    StudentNode* currentPtr = head;
    do {
        file << "| " << currentPtr->getName();
        int nameLen = currentPtr->getName().length();
        for (int i = nameLen; i < 20; i++) file << " ";
        file << " |    " << currentPtr->getRating();
        if (currentPtr->getRating() < 10) file << "     |\n";
        else if (currentPtr->getRating() < 100) file << "    |\n";
        else file << "   |\n";
        currentPtr = currentPtr->getNext();
    } while (currentPtr != head);

    file << "+----------------------+----------+\n";
    file << "\nученики в порядке невозрастания рейтинга:" << endl;

    // Сортировка пузырьком
    string* names = new string[count];
    int* ratings = new int[count];

    currentPtr = head;
    for (int i = 0; i < count; i++) {
        names[i] = currentPtr->getName();
        ratings[i] = currentPtr->getRating();
        currentPtr = currentPtr->getNext();
    }

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (ratings[j] < ratings[j + 1]) {
                int tempRating = ratings[j];
                ratings[j] = ratings[j + 1];
                ratings[j + 1] = tempRating;
                string tempName = names[j];
                names[j] = names[j + 1];
                names[j + 1] = tempName;
            }
        }
    }

    file << "+----------------------+----------+\n";
    file << "|        имя           | рейтинг  |\n";
    file << "+----------------------+----------+\n";
    for (int i = 0; i < count; i++) {
        file << "| " << names[i];
        int nameLen = names[i].length();
        for (int j = nameLen; j < 20; j++) file << " ";
        file << " |    " << ratings[i];
        if (ratings[i] < 10) file << "     |\n";
        else if (ratings[i] < 100) file << "    |\n";
        else file << "   |\n";
    }
    file << "+----------------------+----------+\n";

    delete[] names;
    delete[] ratings;
    file.close();

    setColor(COLOR_TITLE);
    cout << "итоговые результаты сохранены в " << filename << endl;
    setColor(COLOR_DEFAULT);
}

// ==================== ГЛАВНАЯ ФУНКЦИЯ ИГРЫ ====================

// task_16 - главная функция игры (выбор ввода, игровой цикл, вывод результатов)
void task_16(StudentGame*& lastGame) {
    using namespace std;

    cout << endl;

    StudentGame game;

    // Новая игра - выбор способа ввода (без продолжения)
    try {
        int inputMethod;
        setColor(COLOR_TITLE);
        cout << "====== ^-^  выбор способа ввода  ^-^ ======\n" << endl;
        setColor(COLOR_DEFAULT);
        cout << " 1 - ввод с консоли" << endl;
        cout << " 2 - рандомная генерация учеников" << endl;
        cout << " 3 - загрузить из файла" << endl;

        string inputStr;
        while (true) {
            cout << "\nвведите цифру: ";
            getline(cin, inputStr);

            if (inputStr.empty()) {
                setColor(COLOR_ERROR);
                cout << "ошибка! пустой ввод.\n";
                setColor(COLOR_DEFAULT);
                continue;
            }

            if (inputStr.length() == 1 && isdigit(inputStr[0])) {
                inputMethod = stoi(inputStr);
                if (inputMethod >= 1 && inputMethod <= 3) {
                    break;
                }
            }
            setColor(COLOR_ERROR);
            cout << "ошибка! введите 1, 2 или 3.\n";
            setColor(COLOR_DEFAULT);
        }

        switch (inputMethod) {
        case 1: {  // Ручной ввод с консоли
            int numStudents;
            while (true) {
                cout << "\nвведите количество учеников: ";
                getline(cin, inputStr);

                bool isNumber = true;
                for (char c : inputStr) {
                    if (!isdigit(c)) {
                        isNumber = false;
                        break;
                    }
                }

                if (isNumber && !inputStr.empty()) {
                    numStudents = stoi(inputStr);
                    if (numStudents > 0) {
                        break;
                    }
                }
                setColor(COLOR_ERROR);
                cout << "ошибка! введите положительное число.\n";
                setColor(COLOR_DEFAULT);
            }

            for (int i = 0; i < numStudents; i++) {
                string name;
                cout << "введите имя ученика " << i + 1 << ": ";
                getline(cin, name);

                while (name.empty()) {
                    setColor(COLOR_ERROR);
                    cout << "имя не может быть пустым! введите имя: ";
                    setColor(COLOR_DEFAULT);
                    getline(cin, name);
                }
                game.addStudent(name);

                setColor(COLOR_TITLE);
                cout << "текущий список: ";
                setColor(COLOR_DEFAULT);
                game.printStudentsInline(cout);
                cout << endl;
            }
            break;
        }
        case 2: {  // Рандомная генерация
            int numStudents;
            while (true) {
                cout << "\nвведите количество учеников (1-100): ";
                getline(cin, inputStr);

                bool isNumber = true;
                for (char c : inputStr) {
                    if (!isdigit(c)) {
                        isNumber = false;
                        break;
                    }
                }

                if (isNumber && !inputStr.empty()) {
                    numStudents = stoi(inputStr);
                    if (numStudents >= 1 && numStudents <= 100) {
                        break;
                    }
                }
                setColor(COLOR_ERROR);
                cout << "ошибка! введите число от 1 до 100.\n";
                setColor(COLOR_DEFAULT);
            }
            game.generateRandomStudents(numStudents);
            break;
        }
        case 3: {  // Загрузка из файла
            string filename;
            cout << "\nвведите имя файла со списком учеников: ";
            getline(cin, filename);

            if (!game.loadStudentsFromFile(filename)) {
                setColor(COLOR_ERROR);
                cout << "не удалось загрузить список учеников. проверьте файл." << endl;
                setColor(COLOR_DEFAULT);
                return;
            }
            break;
        }
        }

        if (game.getCount() == 0) {
            setColor(COLOR_ERROR);
            cout << "нет учеников для игры!" << endl;
            setColor(COLOR_DEFAULT);
            return;
        }

        setColor(COLOR_TITLE);
        cout << "\nзагружено учеников: " << game.getCount() << endl;
        setColor(COLOR_DEFAULT);
        cout << "список учеников: ";
        game.printStudentsInline(cout);
        cout << endl;
    }
    catch (const GameException& e) {
        setColor(COLOR_ERROR);
        cout << e.what() << endl;
        setColor(COLOR_DEFAULT);
        return;
    }

    // ========== ОСНОВНОЙ ИГРОВОЙ ЦИКЛ ==========
    bool continueGame = true;
    int totalRoundsPlayed = 0;
    int currentRound = 1;

    while (continueGame) {
        int rounds;
        string inputStr;
        while (true) {
            cout << "\nвведите количество раундов для этой сессии: ";
            getline(cin, inputStr);

            bool isNumber = true;
            for (char c : inputStr) {
                if (!isdigit(c)) {
                    isNumber = false;
                    break;
                }
            }

            if (isNumber && !inputStr.empty()) {
                rounds = stoi(inputStr);
                if (rounds > 0) {
                    break;
                }
            }
            setColor(COLOR_ERROR);
            cout << "ошибка! введите положительное число.\n";
            setColor(COLOR_DEFAULT);
        }

        bool exitToMenu = false;
        game.playGame(rounds, currentRound, exitToMenu);

        if (exitToMenu) {
            return;  // выход в главное меню
        }

        totalRoundsPlayed += rounds;
        currentRound += rounds;

        // Вопрос о продолжении игры после сессии
        int continueChoice;
        while (true) {
            setColor(COLOR_TITLE);
            cout << "\n====== *-* продолжить игру? *-* ======" << endl;
            setColor(COLOR_DEFAULT);
            cout << "1 - да, добавить ещё раунды" << endl;
            cout << "2 - нет, завершить игру" << endl;
            cout << "\nвведите цифру: ";
            getline(cin, inputStr);

            if (inputStr.length() == 1 && isdigit(inputStr[0])) {
                continueChoice = stoi(inputStr);
                if (continueChoice >= 1 && continueChoice <= 2) {
                    break;
                }
            }
            setColor(COLOR_ERROR);
            cout << "ошибка! введите 1 или 2.\n";
            setColor(COLOR_DEFAULT);
        }

        if (continueChoice == 2) {
            continueGame = false;
        }
    }

    // Вывод итогов
    setColor(COLOR_TITLE);
    cout << "\n=========================================" << endl;
    setColor(COLOR_DEFAULT);
    cout << "всего сыграно раундов: " << totalRoundsPlayed << endl;
    setColor(COLOR_TITLE);
    cout << "=========================================" << endl;
    setColor(COLOR_DEFAULT);

    // Сохраняем результаты в глобальную переменную (для показа в главном меню)
    if (lastGame != nullptr) {
        delete lastGame;
    }
    lastGame = new StudentGame(game);

    cout << "\n";
    game.printSortedByRating(cout);

    // Вопрос о дальнейших действиях
    int afterGameChoice;
    string inputStr;
    while (true) {
        setColor(COLOR_TITLE);
        cout << "\n====== *-* Меню *-* ======" << endl;
        setColor(COLOR_DEFAULT);
        cout << "1 - начать игру заново" << endl;
        cout << "2 - вернуться в главное меню" << endl;
        cout << "\nвведите цифру: ";
        getline(cin, inputStr);

        if (inputStr.length() == 1 && isdigit(inputStr[0])) {
            afterGameChoice = stoi(inputStr);
            if (afterGameChoice >= 1 && afterGameChoice <= 2) {
                break;
            }
        }
        setColor(COLOR_ERROR);
        cout << "ошибка! введите 1 или 2.\n";
        setColor(COLOR_DEFAULT);
    }

    if (afterGameChoice == 1) {
        clearScreen();
        task_16(lastGame);
    }
    else {
        return;
    }
}