#include "Student_Node_and_Game.h"
#include "Student.h"

// ==================== реализация класса studentgame ====================

StudentGame::StudentGame() : head(nullptr), current(nullptr), count(0) {}

StudentGame::StudentGame(const StudentGame& other) : head(nullptr), current(nullptr), count(0) {
    if (!other.isEmpty()) {
        StudentNode* otherCurrentNode = other.head;
        do {
            addStudent(otherCurrentNode->getName());
            otherCurrentNode = otherCurrentNode->getNext();
        } while (otherCurrentNode != other.head);

        StudentNode* thisCurrent = head;
        otherCurrentNode = other.head;
        do {
            thisCurrent->getData()->setRating(otherCurrentNode->getRating());
            thisCurrent = thisCurrent->getNext();
            otherCurrentNode = otherCurrentNode->getNext();
        } while (otherCurrentNode != other.head);

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

StudentGame::~StudentGame() {
    clear();
}

void StudentGame::clear() {
    if (isEmpty()) return;

    StudentNode* last = head->getPrev();
    last->setNext(nullptr);

    StudentNode* currentPtr = head;
    while (currentPtr != nullptr) {
        StudentNode* next = currentPtr->getNext();
        delete currentPtr;
        currentPtr = next;
    }

    head = nullptr;
    current = nullptr;
    count = 0;
}

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

void StudentGame::generateRandomStudents(int numStudents) {
    using namespace std;

    clear();

    vector<string> randomNames = {
        "Ваня", "Евлампий", "Юлик", "Ярик", "Миша", "Добрыня", "Даня", "Петя",
        "Владик", "Макар", "Андрей", "Антон", "Алек", "Игорь", "Алмаз",
        "Иннокентий", "Веник", "Артурик", "Вася", "Боря", "Яков", "Никита",
        "Виталик", "Лёва", "Игнат", "Маша", "Аглая", "Алиса", "Лиза", "Даша",
        "Марфа", "Кира", "Арина", "Карина", "Лия", "Люба", "Галя", "Надя",
        "Аня", "Света", "Олеся", "Вера", "Лариса", "Ася", "Злата", "Альбина",
        "Нина", "Анжела", "Катя", "Рая"
    };

    srand((unsigned int)time(nullptr));

    for (int i = 0; i < numStudents; i++) {
        int randomIndex = rand() % randomNames.size();
        addStudent(randomNames[randomIndex]);
    }

    setColor(COLOR_TITLE);
    cout << "\nсгенерировано " << count << " учеников случайным образом" << endl;
    setColor(COLOR_DEFAULT);
}

void StudentGame::addStudent(const std::string& name) {
    using namespace std;

    StudentNode* newNode = new StudentNode(name);

    if (isEmpty()) {
        newNode->setNext(newNode);
        newNode->setPrev(newNode);
        head = newNode;
        current = newNode;
        count = 1;
        return;
    }

    StudentNode* last = head->getPrev();

    newNode->setNext(head);
    newNode->setPrev(last);

    last->setNext(newNode);
    head->setPrev(newNode);

    count++;
}

void StudentGame::printStudentsInline(std::ostream& out) const {
    using namespace std;

    if (isEmpty()) {
        out << "список учеников пуст";
        return;
    }

    StudentNode* currentPtr = head;
    do {
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
        int nameLen = currentPtr->getName().length();
        for (int i = nameLen; i < 20; i++) out << " ";
        out << " |    " << currentPtr->getRating();
        if (currentPtr->getRating() < 10) out << "     |\n";
        else if (currentPtr->getRating() < 100) out << "    |\n";
        else out << "   |\n";
        currentPtr = currentPtr->getNext();
    } while (currentPtr != head);

    setColor(COLOR_TITLE);
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);
}

void StudentGame::moveClockwise(int steps) {
    if (isEmpty() || steps == 0) return;

    for (int i = 0; i < steps; i++) {
        current = current->getNext();
    }
}

void StudentGame::moveCounterClockwise(int steps) {
    if (isEmpty() || steps == 0) return;

    for (int i = 0; i < steps; i++) {
        current = current->getPrev();
    }
}

int StudentGame::getRandomNumber() const {
    return rand() % 21 - 10;
}

void StudentGame::playGame(int rounds, int startRound) {
    using namespace std;

    if (isEmpty()) {
        setColor(COLOR_ERROR);
        cout << "нет учеников для игры!" << endl;
        setColor(COLOR_DEFAULT);
        return;
    }

    srand((unsigned int)time(nullptr));

    for (int i = 0; i < rounds; i++) {
        int round = startRound + i;

        clearScreen();
        printComputer(round);
        wait(2000);
        clearScreen();

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

        cout << "  текущий ученик (на котором остановился счёт): ";
        setColor(COLOR_ARROW);
        cout << current->getName() << endl;
        setColor(COLOR_DEFAULT);

        current->incrementRating();

        cout << "  ^-^ доброе дело сделал: " << current->getName() << endl;
        cout << "  рейтинг после доброго дела: " << current->getRating() << endl;

        if (i < rounds - 1) {
            int nextChoice;
            while (true) {
                setColor(COLOR_TITLE);
                cout << "\n====== *-* следующий раунд *-* ======" << endl;
                setColor(COLOR_DEFAULT);
                cout << "1 - следующий раунд" << endl;
                cout << "0 - выход в главное меню" << endl;
                cout << "\nвведите цифру: ";

                string inputStr;
                getline(cin, inputStr);

                if (inputStr.length() == 1 && isdigit(inputStr[0])) {
                    nextChoice = stoi(inputStr);
                    if (nextChoice == 0 || nextChoice == 1) {
                        break;
                    }
                }
                setColor(COLOR_ERROR);
                cout << "ошибка! введите 0 или 1.\n";
                setColor(COLOR_DEFAULT);
            }

            if (nextChoice == 0) {
                return;
            }
        }
    }
}

void StudentGame::printSortedByRating(std::ostream& out) const {
    using namespace std;

    if (isEmpty()) {
        out << "нет учеников" << endl;
        return;
    }

    vector<pair<string, int>> students;
    StudentNode* currentPtr = head;
    do {
        students.push_back({ currentPtr->getName(), currentPtr->getRating() });
        currentPtr = currentPtr->getNext();
    } while (currentPtr != head);

    stable_sort(students.begin(), students.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });

    setColor(COLOR_TITLE);
    out << "\n+----------------------+----------+\n";
    out << "|        имя           | рейтинг  |\n";
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);
    for (size_t i = 0; i < students.size(); i++) {
        out << "| " << students[i].first;
        int nameLen = students[i].first.length();
        for (int j = nameLen; j < 20; j++) out << " ";
        out << " |    " << students[i].second;
        if (students[i].second < 10) out << "     |\n";
        else if (students[i].second < 100) out << "    |\n";
        else out << "   |\n";
    }
    setColor(COLOR_TITLE);
    out << "+----------------------+----------+\n";
    setColor(COLOR_DEFAULT);
}

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

    vector<pair<string, int>> students;
    currentPtr = head;
    do {
        students.push_back({ currentPtr->getName(), currentPtr->getRating() });
        currentPtr = currentPtr->getNext();
    } while (currentPtr != head);

    stable_sort(students.begin(), students.end(),
        [](const pair<string, int>& a, const pair<string, int>& b) {
            return a.second > b.second;
        });

    file << "+----------------------+----------+\n";
    file << "|        имя           | рейтинг  |\n";
    file << "+----------------------+----------+\n";
    for (size_t i = 0; i < students.size(); i++) {
        file << "| " << students[i].first;
        int nameLen = students[i].first.length();
        for (int j = nameLen; j < 20; j++) file << " ";
        file << " |    " << students[i].second;
        if (students[i].second < 10) file << "     |\n";
        else if (students[i].second < 100) file << "    |\n";
        else file << "   |\n";
    }
    file << "+----------------------+----------+\n";

    file.close();
    setColor(COLOR_TITLE);
    cout << "итоговые результаты сохранены в " << filename << endl;
    setColor(COLOR_DEFAULT);
}

void task_16() {
    using namespace std;

    cout << endl;

    StudentGame game;

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
    case 1: {
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
        }
        break;
    }
    case 2: {
        int numStudents;
        while (true) {
            cout << "\nвведите количество учеников (1-50): ";
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
                if (numStudents >= 1 && numStudents <= 50) {
                    break;
                }
            }
            setColor(COLOR_ERROR);
            cout << "ошибка! введите число от 1 до 50.\n";
            setColor(COLOR_DEFAULT);
        }
        game.generateRandomStudents(numStudents);
        break;
    }
    case 3: {
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

    bool continueGame = true;
    int totalRoundsPlayed = 0;
    int currentRound = 1;

    while (continueGame) {
        int rounds;
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

        game.playGame(rounds, currentRound);
        totalRoundsPlayed += rounds;
        currentRound += rounds;

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

    setColor(COLOR_TITLE);
    cout << "\n=========================================" << endl;
    setColor(COLOR_DEFAULT);
    cout << "всего сыграно раундов: " << totalRoundsPlayed << endl;
    setColor(COLOR_TITLE);
    cout << "=========================================" << endl;
    setColor(COLOR_DEFAULT);

    int outputChoice;
    while (true) {
        setColor(COLOR_TITLE);
        cout << "\n====== *-* вывод итоговых результатов *-* ======" << endl;
        setColor(COLOR_DEFAULT);
        cout << "1 - вывод в консоль" << endl;
        cout << "2 - сохранение в файл" << endl;
        cout << "\nвведите цифру: ";
        getline(cin, inputStr);

        if (inputStr.length() == 1 && isdigit(inputStr[0])) {
            outputChoice = stoi(inputStr);
            if (outputChoice >= 1 && outputChoice <= 2) {
                break;
            }
        }
        setColor(COLOR_ERROR);
        cout << "ошибка! введите 1 или 2.\n";
        setColor(COLOR_DEFAULT);
    }

    if (outputChoice == 1) {
        game.printResultsColumn(cout);
        game.printSortedByRating(cout);
    }
    else {
        string resultFile;
        cout << "введите имя файла для сохранения итоговых результатов: ";
        getline(cin, resultFile);
        game.saveResultsToFile(resultFile);
    }

    int afterGameChoice;
    while (true) {
        setColor(COLOR_TITLE);
        cout << "\n====== *-* что дальше? *-* ======" << endl;
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
        task_16();
    }
    else {
        return;
    }
}