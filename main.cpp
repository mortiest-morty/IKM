#include "Student_Node_and_Game.h"
#include "Student.h"

// показ правил
void showRules() {
    using namespace std;

    clearScreen();

    setColor(COLOR_TITLE);
    cout << "\n =======================================\n";
    cout << "|            ПРАВИЛА ИГРЫ               |\n";
    cout << " =======================================\n\n";
    setColor(COLOR_DEFAULT);

    cout << "1. Ученики встают в круг и по очереди делают добрые дела.\n";
    cout << "2. В каждом раунде генерируется случайное число от -10 до 10.\n";
    cout << "3. Если число положительное - движение ПО часовой стрелке.\n";
    cout << "4. Если число отрицательное - движение ПРОТИВ часовой стрелки.\n";
    cout << "5. Ученик, на котором остановились, делает доброе дело (рейтинг +1).\n";
    cout << "6. Следующий раунд начинается со следующего ученика по часовой стрелке.\n";

    setColor(COLOR_ARROW);
    cout << "УДАЧНОЙ ИГРЫ!\n";
    setColor(COLOR_DEFAULT);

    cout << "\nНажмите Enter, чтобы вернуться в меню...";
    cin.get();
}

// показ меню
void showMenu() {
    using namespace std;

    int choice;

    while (true) {
        clearScreen();

        setColor(COLOR_TITLE);
        cout << "\n";
        cout << " /\\_/\\                                            /\\_/\\\n";
        cout << "(^ . ^) ============  ГЛАВНОЕ МЕНЮ  ============ (^ . ^)\n\n";
        setColor(COLOR_DEFAULT);
        cout << " 1 - Начать игру\n";
        cout << " 2 - Правила игры\n";
        cout << " 0 - Выход" << endl;

        string input;
        bool validInput = false;

        while (!validInput) {
            cout << "\nВведите цифру: ";
            getline(cin, input);

            if (input.length() == 1 && isdigit(input[0])) {
                choice = stoi(input);
                if (choice >= 0 && choice <= 2) {
                    validInput = true;
                }
                else {
                    setColor(COLOR_ERROR);
                    cout << "Ошибка! Введите 0, 1 или 2.\n";
                    setColor(COLOR_DEFAULT);
                }
            }
            else {
                setColor(COLOR_ERROR);
                cout << "Ошибка! Введите 0, 1 или 2.\n";
                setColor(COLOR_DEFAULT);
            }
        }

        switch (choice) {
        case 1:
            clearScreen();
            setColor(COLOR_TITLE);
            cout << "       _ _,---._\n";
            cout << "    ,-','       `-.___\n";
            cout << "   /-;'               `._\n";
            cout << "  /\\/          ._   _,'o \\\n";
            cout << " ( /\\       _,--'\\,'\"`. )\n";
            cout << "  |\\      ,'o     \\'    //\\\n";
            cout << "  |      \\        /   ,--'\"\"`-.\n";
            cout << "  :       \\_    _/ ,-'         `-._\n";
            cout << "   \\        `--'  /                )\n";
            cout << "    `.  \\`._    ,'     ________,','\n";
            cout << "      .--`     ,'  ,--` __\\___,;'\n";
            cout << "       \\`.,-- ,' ,`_)--'  /`.,'\n";
            cout << "        \\( ;  | | )      (`-/\n";
            cout << "          `--'| |)       |-/\n";
            cout << "            | | |        | |\n";
            cout << "            | | |,.,-.   | |_\n";
            cout << "            | `./ /   )---`  )\n";
            cout << "           _|  /    ,',   ,-'\n";
            cout << "    -hrr-   ,'|_(    /-<._,' |--,\n";
            cout << "           |    `--'---.     \\/ \\\n";
            cout << "           |          / \\    /\\  \\\n";
            cout << "         ,-^---._     |  \\  /  \\  \\\n";
            cout << "      ,-'        \\----'   \\/    \\--`.\n";
            cout << "     /            \\              \\   \\\n";
            setColor(COLOR_ARROW);
            cout << "\n\n        ИГРА НАЧАЛАСЬ!   И СЕССИЯ ТОЖЕ Х-Х\n\n";
            setColor(COLOR_DEFAULT);
            wait(3000);
            clearScreen();
            task_16();
            break;
        case 2:
            showRules();
            break;
        case 0:
            clearScreen();
            setColor(COLOR_TITLE);
            cout << "\n  ^-^ Спасибо за игру! До новых встреч! ^-^\n\n";
            setColor(COLOR_DEFAULT);
            return;
        }
    }
}

int main() {
    using namespace std;

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_ALL, "rus");

    showMenu();

    return 0;
}