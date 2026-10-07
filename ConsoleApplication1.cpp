#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_R = 10;
const int MAX_C = 10;

int board[MAX_R][MAX_C];
bool opened[MAX_R][MAX_C];
bool flagged[MAX_R][MAX_C];

int rows = 0;
int cols = 0;
int totalMines = 0;

void printBoard(bool showAll) {
    cout << "   ";
    for (int j = 0; j < cols; j++) {
        char colName = 'A' + j;
        cout << colName << " ";
    }
    cout << endl;

    for (int i = 0; i < rows; i++) {
        if (i + 1 < 10) cout << " ";
        cout << i + 1 << " ";

        for (int j = 0; j < cols; j++) {
            if (showAll) {
                if (board[i][j] == -1) {
                    cout << "* ";
                }
                else if (board[i][j] == 0) {
                    cout << "  ";
                }
                else {
                    cout << board[i][j] << " ";
                }
            }
            else {
                if (flagged[i][j]) {
                    cout << "F ";
                }
                else if (!opened[i][j]) {
                    cout << ". ";
                }
                else {
                    if (board[i][j] == 0) {
                        cout << "  ";
                    }
                    else {
                        cout << board[i][j] << " ";
                    }
                }
            }
        }
        cout << endl;
    }
}

int countMinesAround(int r, int c) {
    int count = 0;
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            int nr = r + dr;
            int nc = c + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {
                if (board[nr][nc] == -1) {
                    count++;
                }
            }
        }
    }
    return count;
}

void placeMines(int firstR, int firstC) {
    int placed = 0;
    while (placed < totalMines) {
        int mr = rand() % rows;
        int mc = rand() % cols;
        if (mr == firstR && mc == firstC) continue;
        if (board[mr][mc] != -1) {
            board[mr][mc] = -1;
            placed++;
        }
    }

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j] != -1) {
                board[i][j] = countMinesAround(i, j);
            }
        }
    }
}

void openCell(int r, int c) {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return;
    if (opened[r][c] || flagged[r][c]) return;

    opened[r][c] = true;

    if (board[r][c] == 0) {
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr != 0 || dc != 0) {
                    openCell(r + dr, c + dc);
                }
            }
        }
    }
}

bool checkWinByFlags() {
    int correctFlags = 0;
    int totalFlags = 0;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (flagged[i][j]) {
                totalFlags++;
                if (board[i][j] == -1) {
                    correctFlags++;
                }
            }
        }
    }

    return (correctFlags == totalMines && totalFlags == totalMines);
}

bool checkWinByOpen() {
    int openCount = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (opened[i][j]) {
                openCount++;
            }
        }
    }
    return openCount == (rows * cols - totalMines);
}

int main() {
    srand(time(0));

    while (true) {
        cout << "0 - Exit" << endl;
        cout << "1 - Easy" << endl;
        cout << "2 - Medium" << endl;
        cout << "3 - Hard" << endl;

        int level;
        if (!(cin >> level)) break;

        if (level == 0) {
            break;
        }
        else if (level == 1) {
            rows = 5;
            cols = 5;
            totalMines = 3;
        }
        else if (level == 2) {
            rows = 8;
            cols = 8;
            totalMines = 8;
        }
        else if (level == 3) {
            rows = 10;
            cols = 10;
            totalMines = 15;
        }
        else {
            cout << "Error" << endl;
            continue;
        }

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = 0;
                opened[i][j] = false;
                flagged[i][j] = false;
            }
        }

        bool firstMove = true;

        while (true) {
            printBoard(false);

            char action;
            char colLetter;
            int rowNumber;
            cin >> action >> colLetter >> rowNumber;

            if (action == '0') {
                break;
            }

            int c = -1;
            if (colLetter >= 'A' && colLetter <= 'Z') {
                c = colLetter - 'A';
            }
            else if (colLetter >= 'a' && colLetter <= 'z') {
                c = colLetter - 'a';
            }

            int r = rowNumber - 1;

            if (r < 0 || r >= rows || c < 0 || c >= cols) {
                cout << "Error" << endl;
                continue;
            }

            if (action == 'f' || action == 'F') {
                if (!opened[r][c]) {
                    flagged[r][c] = !flagged[r][c];
                }

                if (!firstMove && checkWinByFlags()) {
                    printBoard(true);
                    cout << "Win" << endl;
                    break;
                }
                continue;
            }

            if (action == 'o' || action == 'O') {
                if (flagged[r][c]) {
                    continue;
                }

                if (firstMove) {
                    placeMines(r, c);
                    firstMove = false;
                }

                if (board[r][c] == -1) {
                    printBoard(true);
                    cout << "Lose" << endl;
                    break;
                }

                openCell(r, c);

                if (checkWinByOpen() || checkWinByFlags()) {
                    printBoard(true);
                    cout << "Win" << endl;
                    break;
                }
            }
            else {
                cout << "Error" << endl;
            }
        }
    }

    return 0;
}