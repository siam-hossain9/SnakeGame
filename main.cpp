#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>
#include <fstream>

using namespace std;

const int width = 40;
const int height = 20;

int x, y, fruitX, fruitY, score;
int tailX[100], tailY[100];
int tailLength;

int x2, y2, tailX2[100], tailY2[100], tailLength2, score2;

bool gameOver;
bool isTwoPlayer = false;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir, dir2;

int highScore = 0;

void SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void SetCursorPosition(int x, int y) {
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void ShowMenu() {
    SetColor(15);
    cout << "==============================\n";
    cout << "       SNAKE GAME MENU        \n";
    cout << "==============================\n";
    cout << "1. Single Player\n";
    cout << "2. Two Player\n";
    cout << "Select Game Mode: ";

    char choice;
    while (true) {
        choice = _getch();
        if (choice == '1') {
            isTwoPlayer = false;
            break;
        }
        else if (choice == '2') {
            isTwoPlayer = true;
            break;
        }
    }
}

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 3;
    y = height / 2;
    tailLength = 0;
    score = 0;

    if (isTwoPlayer) {
        dir2 = STOP;
        x2 = 2 * width / 3;
        y2 = height / 2;
        tailLength2 = 0;
        score2 = 0;
    }

    fruitX = rand() % width;
    fruitY = rand() % height;
    HideCursor();
}

void Draw() {
    SetCursorPosition(0, 0);

    SetColor(14);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                SetColor(14);
                cout << "#";
            }

            if (i == y && j == x) {
                SetColor(10);
                cout << "O";
            }
            else if (isTwoPlayer && i == y2 && j == x2) {
                SetColor(9);
                cout << "X";
            }
            else if (i == fruitY && j == fruitX) {
                SetColor(12);
                cout << "F";
            }
            else {
                bool printed = false;

                for (int k = 0; k < tailLength; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        SetColor(2);
                        cout << "o";
                        printed = true;
                        break;
                    }
                }
                if (!printed && isTwoPlayer) {
                    for (int k = 0; k < tailLength2; k++) {
                        if (tailX2[k] == j && tailY2[k] == i) {
                            SetColor(3);
                            cout << "+";
                            printed = true;
                            break;
                        }
                    }
                }
                if (!printed) {
                    SetColor(7);
                    cout << " ";
                }
            }

            if (j == width - 1) {
                SetColor(14);
                cout << "#";
            }
        }
        cout << endl;
    }

    SetColor(14);
    for (int i = 0; i < width + 2; i++) cout << "#";
    cout << endl;

    SetColor(15);
    cout << "Player 1 Score: " << score << "   ";
    if (isTwoPlayer) cout << "Player 2 Score: " << score2 << "   ";
    cout << "High Score: " << highScore << endl;
    cout << "Player 1: WASD | Player 2: Arrow Keys | X to Exit" << endl;
}

void Input() {
    if (_kbhit()) {
        int key = _getch();

        if (key == 'a' || key == 'A') dir = LEFT;
        else if (key == 'd' || key == 'D') dir = RIGHT;
        else if (key == 'w' || key == 'W') dir = UP;
        else if (key == 's' || key == 'S') dir = DOWN;
        else if (key == 'x' || key == 'X') gameOver = true;

        if (key == 224) {
            key = _getch();
            if (isTwoPlayer) {
                if (key == 75) dir2 = LEFT;
                else if (key == 77) dir2 = RIGHT;
                else if (key == 72) dir2 = UP;
                else if (key == 80) dir2 = DOWN;
            }
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < tailLength; i++) {
        int tempX = tailX[i];
        int tempY = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = tempX;
        prevY = tempY;
    }

    if (isTwoPlayer) {
        int prevX2 = tailX2[0];
        int prevY2 = tailY2[0];
        tailX2[0] = x2;
        tailY2[0] = y2;
        for (int i = 1; i < tailLength2; i++) {
            int tempX2 = tailX2[i];
            int tempY2 = tailY2[i];
            tailX2[i] = prevX2;
            tailY2[i] = prevY2;
            prevX2 = tempX2;
            prevY2 = tempY2;
        }
    }

    switch (dir) {
    case LEFT:  x--; break;
    case RIGHT: x++; break;
    case UP:    y--; break;
    case DOWN:  y++; break;
    default: break;
    }

    if (isTwoPlayer) {
        switch (dir2) {
        case LEFT:  x2--; break;
        case RIGHT: x2++; break;
        case UP:    y2--; break;
        case DOWN:  y2++; break;
        default: break;
        }
    }


    if (x < 0 || x >= width || y < 0 || y >= height) gameOver = true;
    if (isTwoPlayer && (x2 < 0 || x2 >= width || y2 < 0 || y2 >= height)) gameOver = true;


    for (int i = 0; i < tailLength; i++) {
        if (tailX[i] == x && tailY[i] == y) gameOver = true;
    }
    if (isTwoPlayer) {
        for (int i = 0; i < tailLength2; i++) {
            if (tailX2[i] == x2 && tailY2[i] == y2) gameOver = true;
        }
    }


    if (isTwoPlayer) {
        for (int i = 0; i < tailLength2; i++) {
            if (tailX2[i] == x && tailY2[i] == y) gameOver = true;
        }
        if (x == x2 && y == y2) gameOver = true;
        for (int i = 0; i < tailLength; i++) {
            if (tailX[i] == x2 && tailY[i] == y2) gameOver = true;
        }
    }


    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tailLength++;
    }
    if (isTwoPlayer && x2 == fruitX && y2 == fruitY) {
        score2 += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        tailLength2++;
    }
}

bool ShowGameOverScreen() {
    SetCursorPosition(0, height + 4);
    SetColor(12);
    cout << " GAME OVER " << endl;
    SetColor(15);
    cout << "Final Scores:" << endl;
    cout << "Player 1: " << score << endl;
    if (isTwoPlayer) cout << "Player 2: " << score2 << endl;

    if (isTwoPlayer) {
        if (score > score2) cout << "Player 1 Wins!" << endl;
        else if (score2 > score) cout << "Player 2 Wins!" << endl;
        else cout << "It's a Tie!" << endl;
    }

    cout << "\nOptions:" << endl;
    cout << "A - Play Again\n";
    cout << "M - Main Menu\n";
    cout << "X - Exit\n";

    char choice;
    while (true) {
        choice = _getch();
        if (choice == 'a' || choice == 'A') {
            Setup();
            return true;
        }
        else if (choice == 'm' || choice == 'M') {
            return false;
        }
        else if (choice == 'x' || choice == 'X') {
            exit(0);
        }
    }
}

int main() {
    srand(time(0));

    ifstream infile("highscore.txt");
    if (infile.is_open()) {
        infile >> highScore;
        infile.close();
    }

    while (true) {
        ShowMenu();
        Setup();

        while (!gameOver) {
            Draw();
            Input();
            Logic();

            int speed = 100 - (tailLength * 2);
            if (speed < 40) speed = 40;
            Sleep(speed);
        }

        int maxScore = (score > score2) ? score : score2;
        if (maxScore > highScore) {
            highScore = maxScore;
            ofstream outfile("highscore.txt");
            outfile << highScore;
            outfile.close();
        }

        bool play = ShowGameOverScreen();
        if (!play) {
            gameOver = false;
            system("cls");
            continue;
        }
        gameOver = false;
    }

    return 0;
}
