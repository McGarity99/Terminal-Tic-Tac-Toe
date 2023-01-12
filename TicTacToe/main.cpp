#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>

using std::cout;
using std::cin;
using std::endl;
using std::string;

char gameBoard[3][3];   //represents game board

enum Position {
                TopLeft, MidLeft, BottomLeft,
                TopMid, MidMid, BottomMid,
                TopRight, MidRight, BottomRight
                };

/*Function Signatures*/
void printWelcome();
void setupBoard(char board[3][3]);
void printBoard(char board[3][3]);
void playerTurn();
void compTurn();
void placePiece(int row, int col, bool isPlayer);
Position findPosition(int row, int col);

bool threeRow(int row, int col, bool isPlayer);
bool checkLeft(int row, int col, bool isPlayer);
bool checkRight(int row, int col, bool isPlayer);
bool checkLeftRight(int row, int col, bool isPlayer);
bool checkUp(int row, int col, bool isPlayer);
bool checkDown(int row, int col, bool isPlayer);
bool checkUpDown(int row, int col, bool isPlayer);
bool checkUpRight(int row, int col, bool isPlayer);
bool checkDownRight(int row, int col, bool isPlayer);
bool checkUpLeft(int row, int col, bool isPlayer);
bool checkDownLeft(int row, int col, bool isPlayer);
bool checkUpRightDownLeft(int row, int col, bool isPlayer);
bool checkUpLeftDownRight(int row, int col, bool isPlayer);


int main() {
    printWelcome();
    srand(time(0));
    setupBoard(gameBoard);
    printBoard(gameBoard);
    while (true) {
	playerTurn();
	printBoard(gameBoard);
	compTurn();
	printBoard(gameBoard);
    }
    return 0;
}

void printWelcome() {
    char input;
    cout << "\nWelcome to Command-Line Tic-Tac-Toe!" << endl;
    cout << "Play 1-on-1 against the computer" << endl;
    cout << "Player Pieces: X" << endl;
    cout << "Comp Pieces: O" << endl;
    cout << endl;
    cout << "Press [B] to begin, [E] to exit" << endl;

    cin >> input;
    if (input != 'b' && input != 'B') {
        exit(0);
    }
    //sleep(1.5);
}

void setupBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '-';
        }
    }
}

void printBoard(char board[3][3]) {
    cout << " ";
    for (int i = 0; i < 3; i++) {
        cout << " " << i;
    }
    cout << endl;
    for (int i = 0; i < 3; i++) {
        cout << i << " ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j <= 1) cout << "|";
        }
        cout << endl;
    }
}

void playerTurn() {
    bool rowGood = false;
    bool colGood = false;
    string rowStr;
    string colStr;
    int rowNum;
    int colNum;

    cout << endl;
    cout << "Enter row and column." << endl;
    cout << "NOTE: Non-numeric input interpreted as 0" << endl;
    while (!rowGood) {
        cout << "Row: ";
        cin >> rowStr;
        rowNum = std::atoi(rowStr.c_str());
        if (rowNum >= 0 && rowNum <= 2) {
            rowGood = true;
        } else {
            cout << "ERROR: Row out of range" << endl;
        }
    }

    while (!colGood) {
        cout << "Col: ";
        cin >> colStr;
        colNum = std::atoi(colStr.c_str());
        if (colNum >= 0 && colNum <= 2) {
            colGood = true;
        } else {
            cout << "ERROR: Col out of range" << endl;
        }
    }

    placePiece(rowNum, colNum, true);
}

void compTurn() {
	int rowCoor = rand() % 3;
	int colCoor = rand() % 3;

	while (gameBoard[rowCoor][colCoor] != '-') {
		rowCoor = rand() % 3;
		if (gameBoard[rowCoor][colCoor] == '-') {
			break;
		} else {
			colCoor = rand() % 3;
		}
	} //validate the row/col coordinates chosen
	
	placePiece(rowCoor, colCoor, false);
}

void placePiece(int row, int col, bool isPlayer) {
    if (gameBoard[row][col] == '-') {
        switch (isPlayer) {
            case true:
                gameBoard[row][col] = 'X';
                break;
            case false:
                gameBoard[row][col] = 'O';
                break;
        }
    } else {
        if (isPlayer) {
            cout << "Provided coordinates already used. Try again." << endl;
            playerTurn();
            return;
        }
    }
}

Position findPosition(int row, int col) {
    switch (row) {
        case 0: //in Top row
            switch (col) {
                case 0:
                    return Position::TopLeft;
                    break;
                case 1:
                    return Position::TopMid;
                    break;
                default:
                    return Position::TopRight;
                    break;
            }
            break;
        case 1: //in Mid row
            switch (col) {
                case 0:
                    return Position::MidLeft;
                    break;
                case 1:
                    return Position::MidMid;
                    break;
                default:
                    return Position::MidRight;
                    break;
            }
            break;
        default:    //in Bottom row
            switch (col) {
                case 0:
                    return Position::BottomLeft;
                    break;
                case 1:
                    return Position::BottomMid;
                    break;
                default:
                    return Position::BottomRight;
                    break;
            }
            break;
    }
    return Position::MidMid;
}

/*
    This is the "master function" for determining if the most recently-placed
    game piece forms a three-in-a-row connection. If so, return true, else return false.
    It operates on the given coordinate's Position value, as determined by the findPosition function.
*/

bool threeRow(int row, int col, bool isPlayer) {
    Position pos = findPosition(row, col);
    switch (pos) {
        case Position::TopLeft:
            return (checkRight(row, col, isPlayer) ||
                    checkDown(row, col, isPlayer) ||
                    checkDownRight(row, col, isPlayer)
                    );
            break;
        case Position::MidLeft:
            return (checkRight(row, col, isPlayer) ||
                    checkUpDown(row, col, isPlayer)
                    );
            break;
        case Position::BottomLeft:
            return (checkUp(row, col, isPlayer) ||
                    checkRight(row, col, isPlayer) ||
                    checkUpRight(row, col, isPlayer)
                    );
            break;
        case Position::TopMid:
            return (checkDown(row, col, isPlayer) ||
                    checkLeftRight(row, col, isPlayer)
                    );
            break;
        case Position::MidMid:
            return (checkLeftRight(row, col, isPlayer) ||
                    checkUpDown(row, col, isPlayer) ||
                    checkUpLeftDownRight(row, col, isPlayer) ||
                    checkUpRightDownLeft(row, col, isPlayer)
                    );
            break;
        case Position::BottomMid:
            return (checkUp(row, col, isPlayer) ||
                    checkLeftRight(row, col, isPlayer)
                    );
            break;
        case Position::TopRight:
            return (checkLeft(row, col, isPlayer) ||
                    checkDown(row, col, isPlayer) ||
                    checkDownLeft(row, col, isPlayer)
                    );
            break;
        case Position::MidRight:
            return (checkLeft(row, col, isPlayer) ||
                    checkUpDown(row, col, isPlayer)
                    );
            break;
        default:
            return (checkUp(row, col, isPlayer) ||
                    checkLeft(row, col, isPlayer) ||
                    checkUpLeft(row, col, isPlayer)
                    );
            break;
    }
    return false;
}
