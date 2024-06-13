#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <chrono>
#include <fstream>

using namespace std;
HANDLE hConsole;


// функция для проверки, можно ли разместить число в данной ячейке
bool isvalid(vector<vector<int>>& board, int row, int col, int num)
{
    // проверяем, что число не встречается в том же ряду или столбце
    for (int i = 0; i < 9; ++i)
    {
        if (board[row][i] == num || board[i][col] == num)
        {
            return false;
        }
    }

    // проверяем, что число не встречается в том же 3x3 блоке
    int startrow = (row / 3) * 3;
    int startcol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (board[startrow + i][startcol + j] == num)
            {
                return false;
            }
        }
    }

    return true;
}

// функция для рекурсивной генерации судоку
bool generatesudoku(vector<vector<int>>& board, int row, int col, int way)
{
    // если достигнут конец строки, переходим на следующую строку
    if (col == 9)
    {
        col = 0;
        ++row;

        // если достигнут конец доски, судоку сгенерировано успешно
        if (row == 9)
        {
            return true;
        }
    }

    // если ячейка уже заполнена, переходим к следующей
    if (board[row][col] != 0)
    {
        return generatesudoku(board, row, col + 1, way);
    }

    // сортируем числа от 1 до 9
    vector<int> nums = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    switch (way)
    {
    case 1:
        shuffle(nums.begin(), nums.end(), mt19937(random_device()()));
        break;
    case 2:
        break;
    case 3:
        reverse(nums.begin(), nums.end());
        break;
    default:
        break;
    }

    for (int num : nums)
    {
        if (isvalid(board, row, col, num))
        {
            board[row][col] = num;

            // рекурсивно заполняем следующую ячейку
            if (generatesudoku(board, row, col + 1, way))
            {
                return true;
            }

            // если сгенерированное судоку невалидно, очищаем ячейку и пробуем другое число
            board[row][col] = 0;
        }
    }

    return false;
}


//функция для проверки судоку на единственность решения
bool checksudoku(vector<vector<int>>& sudoku, vector<vector<int>>& sudoku1_9, vector<vector<int>>& sudoku9_1)
{
    sudoku1_9 = sudoku;
    (generatesudoku(sudoku1_9, 0, 0, 2));

    sudoku9_1 = sudoku;
    (generatesudoku(sudoku9_1, 0, 0, 3));

    if (sudoku1_9 == sudoku9_1)
    {
        return true;
    }
    return false;
}

// Функция для удаления 30 цифр из судоку
void removeDigits(vector<vector<int>>& sudoku, vector<vector<int>>& sudoku1_9, vector<vector<int>>& sudoku9_1, int numDigitsToRemove)
{
    vector<int> row_and_col(80);
    for (int a = 0; a < 80; a++)
    {
        row_and_col[a] = a;
    }

    shuffle(row_and_col.begin(), row_and_col.end(), mt19937(random_device()()));

    for (int i = 0; i < numDigitsToRemove;)
    {
        int counter = 0;
        for (int position : row_and_col)
        {
            int row = position / 9;
            int col = position - (row * 9);

            if (sudoku[row][col] != 0)
            {
                int musor = sudoku[row][col];
                sudoku[row][col] = 0;
                if (checksudoku(sudoku, sudoku1_9, sudoku9_1))
                {
                    i++;
                    break;
                }
                else
                {
                    sudoku[row][col] = musor;
                    counter++;
                }
            }
        }
        if (counter > 80)
        {
            break;
        }
    }
}

// функция для вывода судоку на экран
void printsudoku(const vector<vector<int>>& board, vector<vector<int>>& startnumbers)
{
    int counter = 0;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    for (int i = 1; i <= 21; i++)
    {
        cout << "\t\t\t\t\t";
        if (i == 1 || i == 3 || i == 5 || i == 7 || i == 15 || i == 17 || i == 19 || i == 21)
        {
            SetConsoleTextAttribute(hConsole, 3);
            cout << "------------- ";

            SetConsoleTextAttribute(hConsole, 2);
            cout << "------------- ";

            SetConsoleTextAttribute(hConsole, 3);
            cout << "-------------";
        }
        else if (i == 8 || i == 10 || i == 12 || i == 14)
        {
            SetConsoleTextAttribute(hConsole, 2);
            cout << "------------- ";

            SetConsoleTextAttribute(hConsole, 3);
            cout << "------------- ";

            SetConsoleTextAttribute(hConsole, 2);
            cout << "-------------";
        }
        else if (i == 2 || i == 4 || i == 6 || i == 16 || i == 18 || i == 20)
        {
            for (int j = 1; j <= 41; j++)
            {
                if (j == 1 || j == 5 || j == 9 || j == 13 || j == 29 || j == 33 || j == 37 || j == 41)
                {
                    SetConsoleTextAttribute(hConsole, 3);
                    cout << "|";
                }
                else if (j == 15 || j == 19 || j == 23 || j == 27)
                {
                    SetConsoleTextAttribute(hConsole, 2);
                    cout << "|";
                }
                else if (j - ((j / 2) * 2) == 0)
                {
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << " ";
                }
                else
                {
                    int row = counter / 9;
                    int col = counter - (row * 9);


                    int musor = 0;
                    for (int i = 0; i < 31; i++)
                    {
                        if (row == startnumbers[i][1] && col == startnumbers[i][2])
                        {
                            musor++;
                        }
                    }
                    if (musor > 0)
                    {
                        SetConsoleTextAttribute(hConsole, 14);
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, 6);
                    }

                    if (board[row][col] != 0)
                    {
                        cout << board[row][col];
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, 8);
                        cout << " ";
                    }

                    counter++;
                }
            }
        }
        else
        {
            for (int j = 1; j <= 41; j++)
            {
                if (j == 1 || j == 5 || j == 9 || j == 13 || j == 29 || j == 33 || j == 37 || j == 41)
                {
                    SetConsoleTextAttribute(hConsole, 2);
                    cout << "|";
                }
                else if (j == 15 || j == 19 || j == 23 || j == 27)
                {
                    SetConsoleTextAttribute(hConsole, 3);
                    cout << "|";
                }
                else if (j - ((j / 2) * 2) == 0)
                {
                    SetConsoleTextAttribute(hConsole, 7);
                    cout << " ";
                }
                else
                {

                    int row = counter / 9;
                    int col = counter - (row * 9);


                    int musor = 0;
                    for (int i = 0; i < 31; i++)
                    {
                        if (row == startnumbers[i][1] && col == startnumbers[i][2])
                        {
                            musor++;
                        }
                    }
                    if (musor > 0)
                    {
                        SetConsoleTextAttribute(hConsole, 14);
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, 6);
                    }

                    if (board[row][col] != 0)
                    {
                        cout << board[row][col];
                    }
                    else
                    {
                        SetConsoleTextAttribute(hConsole, 8);
                        cout << " ";
                    }

                    counter++;
                }
            }
        }
        cout << endl;
    }
    SetConsoleTextAttribute(hConsole, 7);
}

// функция для контроля игрового процеса
bool play(vector<vector<int>>& sudoku, vector<vector<int>>& sudokusolved, vector<vector<int>>& startnumbers)
{
    auto start = chrono::system_clock::now();
    while (sudoku != sudokusolved)
    {
        system("cls");
        printsudoku(sudoku, startnumbers);
        int row, col, number, musor = 0;
        cout << "\n\nВведите номер ряда, или 0 если хотите завершить игру и посмотреть готовое решение: " << endl;
        cin >> row;
        if (row == 0)
        {
            break;
        }
        cout << "\nВведите номер столбца: " << endl;
        cin >> col;
        cout << "\nВведите цифру от 1 до 9: " << endl;
        cin >> number;

        for (int i = 0; i < 31; i++)
        {
            if ((row - 1) == startnumbers[i][1] && (col - 1) == startnumbers[i][2])
            {
                musor++;
            }
        }
        if (musor > 0)
        {
            cout << "Вы не можете заменить начальные цифры. Попробуйте другой ряд или столбец\n\n\n\n";
            system("pause");
        }
        else
        {
            sudoku[row - 1][col - 1] = number;
        }
    }

    if (sudoku == sudokusolved)
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);

        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;

        system("cls");
        cout << "\t\t\t<==========================================================================>" << endl;
        cout << "\t\t\t|                               Вы победили!                               |" << endl;
        cout << "\t\t\t<==========================================================================>" << endl;
        cout << "\t\t\t\n    Ваше время: " << elapsed_seconds.count() << "Секунд" << endl;
        cout << "\n\n\n";
        system("pause");
        system("cls");
        return true;
    }
    else
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, 10);
        system("cls");
        cout << "\t\t\t<==========================================================================>" << endl;
        cout << "\t\t\t|                              Вы проиграли!                               |" << endl;
        cout << "\t\t\t<==========================================================================>" << endl;
        cout << "\nГотовое решение:\n";
        printsudoku(sudokusolved, startnumbers);
        cout << "\n\n\n";
        system("pause");
        system("cls");
        return true;
    }


}

// функция для запуска игры
bool generatespeedplay()
{
    setlocale(0, "ru");
    vector<vector<int>> sudoku(9, vector<int>(9, 0));
    vector<vector<int>> sudokusolved(9, vector<int>(9, 0));
    vector<vector<int>> sudoku1_9(9, vector<int>(9, 0));
    vector<vector<int>> sudoku9_1(9, vector<int>(9, 0));
    vector<vector<int>> startnumbers(81, vector<int>(3, 0));

    int counter = 0;

    if (generatesudoku(sudokusolved, 0, 0, 1))
    {
        sudoku = sudokusolved;

        removeDigits(sudoku, sudoku1_9, sudoku9_1, 50);

        for (int row = 0; row < 9; row++)
        {
            for (int col = 0; col < 9; col++)
            {
                if (sudoku[row][col] != 0)
                {
                    startnumbers[counter][1] = row;
                    startnumbers[counter][2] = col;
                    counter++;
                }
            }
        }

        if (play(sudoku, sudokusolved, startnumbers))
        {
            return true;
        }

    }
    else
    {
        cout << "не удалось сгенерировать судоку." << endl;
    }
}

// функция для Вывода правил судоку
bool Pravila()
{
    system("cls");
    setlocale(0, "ru");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);

    cout << "У судоку есть всего одно правило." << endl << endl;
    cout << "Необходимо заполнить свободные клетки" << endl << endl;
    cout << "цифрами от 1 до 9 так, чтобы в каждой строке," << endl << endl;
    cout << "в каждом столбце и в каждом малом квадрате 3x3" << endl << endl;
    cout << "каждая цифра встречалась бы только один раз." << endl << endl << endl;

    system("pause");
    system("cls");
    return true;
}

// функция для Вывода логотипа игры
void logo()
{
    setlocale(0, "ru");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    cout << "\t\t\t **************     ***         ***     ************         *************      ***         ***     ***         ***" << endl;
    cout << "\t\t\t***************     ***         ***     **************      ***************     ***        ***      ***         ***" << endl;
    cout << "\t\t\t***                 ***         ***     ***        ****     ***         ***     ***       ***       ***         ***" << endl;
    cout << "\t\t\t***                 ***         ***     ***         ***     ***         ***     ***      ***        ***         ***" << endl;
    cout << "\t\t\t***                 ***         ***     ***         ***     ***         ***     ***     ***         ***         ***" << endl;
    cout << "\t\t\t***                 ***         ***     ***         ***     ***         ***     ***    ***          ***         ***" << endl;
    cout << "\t\t\t**************      ***         ***     ***         ***     ***         ***     ***   ***           ***         ***" << endl;
    cout << "\t\t\t **************     ***         ***     ***         ***     ***         ***     ********            ***         ***" << endl;
    cout << "\t\t\t            ***     ***         ***     ***         ***     ***         ***     *******             ***         ***" << endl;
    cout << "\t\t\t            ***     ***         ***     ***         ***     ***         ***     ***  ***            ***         ***" << endl;
    cout << "\t\t\t            ***     ***         ***     ***         ***     ***         ***     ***    ***          ***         ***" << endl;
    cout << "\t\t\t            ***     ***         ***     ***        ****     ***         ***     ***      ***        ***         ***" << endl;
    cout << "\t\t\t***************     ***************     **************      ***************     ***        ***      ***************" << endl;
    cout << "\t\t\t**************       *************      ************         *************      ***         ***      ************* " << endl << endl;
}

// функция для Вывода меню
bool menu()
{
    setlocale(0, "ru");
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, 10);
    logo();
    cout << "\n\n\t\t\t\t\t\t\t\t\t       Меню" << endl;
    cout << "\n\nСгенерировать судоку на скорость - 1" << endl;
    cout << "\nОзнакомится с правилами игры - 2" << endl;
    cout << "\nВыйти из игры - 3" << endl;
    cout << "\nВыберете действие: ";
    SetConsoleTextAttribute(hConsole, 7);

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
        system("cls");
        if (generatespeedplay())
        {
            if (menu())
            {
                return true;
            }
        }
        break;
    case 2:
        if (Pravila())
        {
            if (menu())
            {
                return true;
            }
        }
        break;
    case 3:
        return true;
        break;
    default:
        cout << "Некоректный ввод команды, попробуйте ещё раз.";
        break;
    }
}


//void registration()
//{
//    setlocale(0, "ru");
//    char username[20];
//    char password[20];
//    cout << "Введите ваш логин: ";
//    cin >> username;
//    cout << "Введите ваш пароль: ";
//    cin >> password;
//
//    ofstream filout;
//    filout.open("registration.txt", ios::in | ios::out | ios::ate);
//    if (!filout)
//    {
//        cout << "\nНевозможно открыть файл\n";
//    }
//    else
//    {
//        char a[20] = "111111111111";
//        filout << a << endl;
//        filout << username << endl;
//        filout << password << endl;
//    }
//    cout << "\nВы зарегестрированы.";
//}

// Начальная точка программы
int main()
{
    //registration();
    menu();
    return 0;
}