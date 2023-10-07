#include <iostream>
#include <iomanip>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <fstream>
using namespace std;
#pragma region Публічні_змінні;
int moveCount = 0;
const string red = "\033[31m";
const string yellow = "\033[33m";
const string green = "\033[32m";
const string nul = "\033[0m";
int BOARD_SIZE;
vector<vector<int>> board;
int cursorX, cursorY;
chrono::steady_clock::time_point startTime, endTime;
#pragma endregion
#pragma region Ініціалізування_дошки;
void InitializeBoard() {
    cout << "Выберите размер поля (3 для 3x3, 4 для 4x4): ";
    cin >> BOARD_SIZE;

    if (BOARD_SIZE != 3 && BOARD_SIZE != 4) {
        cout << "Недопустимый размер поля. Выберите 3 или 4." << endl;
        return;
    }

    board.resize(BOARD_SIZE, vector<int>(BOARD_SIZE));

    int num = 1;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (num == BOARD_SIZE * BOARD_SIZE) {
                board[i][j] = 0; // Последняя ячейка - пустая
            }
            else {
                board[i][j] = num;
            }
            num++;
        }
    }
    cursorX = BOARD_SIZE - 1;
    cursorY = BOARD_SIZE - 1;
}
#pragma endregion
#pragma region Перевірка_головоломки
bool IsSolvable() {
    int inversionCount = 0;
    vector<int> tiles;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != 0) {
                tiles.push_back(board[i][j]);
            }
        }
    }
    for (int i = 0; i < tiles.size() - 1; ++i) {
        for (int j = i + 1; j < tiles.size(); ++j) {
            if (tiles[i] > tiles[j]) {
                inversionCount++;
            }
        }
    }
    if (BOARD_SIZE == 3) {
        return inversionCount % 2 == 0;
    }
    int emptyRow = 0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                emptyRow = i;
                break;
            }
        }
    }
    return (inversionCount % 2 == 0 && (BOARD_SIZE % 2 == 1 || ((BOARD_SIZE - emptyRow) % 2 == 1)));
}
#pragma endregion
#pragma region Перемішування _елементів
void ShuffleBoard() {
    srand(time(NULL));
    do {
        for (int i = 0; i < BOARD_SIZE; ++i) {
            for (int j = 0; j < BOARD_SIZE; ++j) {
                int x1 = rand() % BOARD_SIZE;
                int y1 = rand() % BOARD_SIZE;
                int x2 = rand() % BOARD_SIZE;
                int y2 = rand() % BOARD_SIZE;
                swap(board[x1][y1], board[x2][y2]);
            }
        }
    } while (!IsSolvable());
}
#pragma endregion
#pragma region Вивод_дошки
void PrintBoard() {
    system("cls");

    for (int i = 0; i < BOARD_SIZE * 6 - 5; ++i) {
        cout << "#";
    }
    if (BOARD_SIZE == 3) {
        cout << "##";
    }
    cout << endl;

    for (int i = 0; i < BOARD_SIZE; ++i) {
        cout << "#";

        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (i == cursorX && j == cursorY) {
                cout << setw(4) << "X";
            }
            else if (board[i][j] == 0) {
                cout << setw(4) << " ";
            }
            else {
                bool isYellow = false;
                bool isGreen = false;

                if (i == cursorX - 1 && j == cursorY) {
                    isYellow = true;
                }
                else if (i == cursorX + 1 && j == cursorY) {
                    isYellow = true;
                }
                else if (i == cursorX && j == cursorY - 1) {
                    isYellow = true;
                }
                else if (i == cursorX && j == cursorY + 1) {
                    isYellow = true;
                }

                if (board[i][j] == i * BOARD_SIZE + j + 1) {
                    isGreen = true;
                }

                if (isYellow) {
                    cout << yellow << setw(4);
                }
                else if (isGreen) {
                    cout << green << setw(4);
                }
                else {
                    cout << setw(4);
                }

                cout << board[i][j] << nul;
            }
        }

        cout << " #" << endl;
    }

    for (int i = 0; i < BOARD_SIZE * 6 - 5; ++i) {
        cout << "#";
    }
    if (BOARD_SIZE == 3) {
        cout << "##";
    }
    cout << endl;
}
#pragma endregion
#pragma region Перевірка_чи_користувач_виграв
bool IsGameWon() {
    int num = 1;
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != num % (BOARD_SIZE * BOARD_SIZE)) {
                return false;
            }
            num++;
        }
    }
    return true;
}
#pragma endregion
#pragma region Перевірка_чи_цей_крок_дійсний
bool IsValidMove(int newRow, int newCol) {
    if (newRow >= 0 && newRow < BOARD_SIZE && newCol >= 0 && newCol < BOARD_SIZE) {
        return (abs(newRow - cursorX) + abs(newCol - cursorY) == 1);
    }
    return false;
}
#pragma endregion
#pragma region Зробити_крок
void MakeMove() {
    char move;
    cout << "Вы сделали " << moveCount << " ходов\n";
    cout << "Введите направление для движения (W/A/S/D, 0 для выхода): ";
    cin >> move;

    if (move == '0') {
        exit(0);
    }

    int newRow = cursorX;
    int newCol = cursorY;

    switch (move) {
    case 'w':
    case 'W':
        newRow--;
        break;
    case 's':
    case 'S':
        newRow++;
        break;
    case 'a':
    case 'A':
        newCol--;
        break;
    case 'd':
    case 'D':
        newCol++;
        break;
    default:
        cout << "Неправильное направление. Повторите ввод." << endl;
        return;
    }

    if (IsValidMove(newRow, newCol)) {
        swap(board[cursorX][cursorY], board[newRow][newCol]);
        cursorX = newRow;
        cursorY = newCol;
        moveCount++;
    }
    else {
        cout << "Неправильное направление. Повторите ввод." << endl;
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
}
#pragma endregion
#pragma region Запитати_чи_хоче_користувач_записати_рекорд
bool AskToSaveRecord() {
    char choice;
    cout << "Хотите сохранить этот рекорд? (Y/N): ";
    cin >> choice;
    return (choice == 'Y' || choice == 'y');
}
#pragma endregion
#pragma region Записування_рекорду
void SaveRecord(int hours, int minutes, int seconds, int moveCount, int boardSize, const string& playerName) {
    string fileName;
    cout << "Введите имя файла для сохранения рекорда: ";
    cin >> fileName;

    ofstream outputFile(fileName);

    if (!outputFile.is_open()) {
        cout << "Не удалось открыть файл для записи." << endl;
        return;
    }

    outputFile << "Поздравляем, " << playerName << "!" << endl;
    outputFile << "Вы успешно прошли " << boardSize << "x" << boardSize << " пятнашки!" << endl;
    outputFile << "Вы прошли за " << hours << " часов " << minutes << " минут " << "и " << seconds << " секунд" << endl;
    outputFile << "Вы сделали " << moveCount << " ходов" << endl;

    outputFile.close();

    cout << "Рекорд успешно сохранен в файле " << fileName << endl;
}
#pragma endregion
#pragma region main
int main() {
    setlocale(LC_ALL, "Rus");
    InitializeBoard();
    ShuffleBoard();
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] == 0) {
                swap(board[i][j], board[BOARD_SIZE - 1][BOARD_SIZE - 1]);
                cursorX = i;
                cursorY = j;
                break;
            }
        }
    }
    startTime = chrono::steady_clock::now(); // Запомнить время начала игры

    while (true) {
        PrintBoard();
        MakeMove();

        if (IsGameWon()) {
            endTime = chrono::steady_clock::now(); // Запомнить время окончания игры
            chrono::duration<int> elapsedTime = chrono::duration_cast<chrono::seconds>(endTime - startTime);
            int hours = elapsedTime.count() / 3600;
            int minutes = (elapsedTime.count() % 3600) / 60;
            int seconds = elapsedTime.count() % 60;
            PrintBoard();
            cout << "Поздравляем! Вы выиграли!" << endl;
            cout << "Вы прошли за " << hours << " часов " << minutes << " минут " << "и " << seconds << " секунд\n";
            cout << "А также вы зделали " << moveCount << " Ходов ";
            if (AskToSaveRecord()) {
                // Сохранить рекорд в файл
                string playerName;
                cout << "Введите ваше имя: ";
                cin >> playerName;
                SaveRecord(hours, minutes, seconds, moveCount, BOARD_SIZE, playerName);
            }
            break;
        }
    }

    return 0;
}
#pragma endregion