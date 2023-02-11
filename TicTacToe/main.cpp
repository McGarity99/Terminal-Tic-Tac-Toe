/*
    Author: Hunter McGarity | 2023
*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <chrono>
#include <thread>
#include <tuple>

using std::cout;
using std::cin;
using std::endl;
using std::string;

char gameBoard[3][3];   //represents game board
bool keepPlaying = true;    //represents continued game state
bool resetState = false;

/*
    Position enum represents each space on the gameboard.
    Used primarily for identifying which directions to check for
    3-in-a-row connections.
*/
enum Position {
                TopLeft, MidLeft, BottomLeft,
                TopMid, MidMid, BottomMid,
                TopRight, MidRight, BottomRight
                };

/*
    Represents the player's previously-used coordinates.
*/
struct PlayerMove {
    int row = -1;
    int col = -1;
} lastPlayerMove;

/*Function Signatures*/
void printWelcome();
void setupBoard(char board[3][3]);
void printBoard(char board[3][3]);
void playerTurn();
void compTurn();
void placePiece(int row, int col, bool isPlayer);
void printWin(bool isPlayer);
void printDraw();
void setLastPlayerMove(int row, int col);
void reset();

Position findPosition(int row, int col);

bool threeRow(int row, int col, bool isPlayer);
bool checkLeft(int row, int col, char searchPiece);
bool checkRight(int row, int col, char searchPiece);
bool checkLeftRight(int row, int col, char searchPiece);
bool checkUp(int row, int col, char searchPiece);
bool checkDown(int row, int col, char searchPiece);
bool checkUpDown(int row, int col, char searchPiece);
bool checkUpRight(int row, int col, char searchPiece);
bool checkDownRight(int row, int col, char searchPiece);
bool checkUpLeft(int row, int col, char searchPiece);
bool checkDownLeft(int row, int col, char searchPiece);
bool checkUpRightDownLeft(int row, int col, char searchPiece);
bool checkUpLeftDownRight(int row, int col, char searchPiece);
bool isDraw();

std::tuple<int, int> fallBackCoordinates(int rowCoor, int colCoor);


int main() {
    printWelcome();
    srand(time(0));
    setupBoard(gameBoard);
    printBoard(gameBoard);

    while (keepPlaying) {
	    playerTurn();   //player gives coordinates and piece is placed
        if (!keepPlaying) {
            break;
        } else if (resetState) {
            reset();
            continue;
        } else {
            printBoard(gameBoard);
        }   //if player won and wants to play again, reset; else print the board and continue

        if (keepPlaying) {
            compTurn(); 
        }
        if (!keepPlaying) {
            break;
        } else if (resetState) {
            reset();
            continue;
        } else {
            printBoard(gameBoard);
        }   //if comp won and player wants to play again, reset; else print the board and continue
    }   //gameplay loop

    return 0;
}

/*
    Print welcome message.
*/

void printWelcome() {
    char input;
    cout << "\nWelcome to Command-Line Tic-Tac-Toe!" << endl;
    cout << "Play 1-on-1 against the computer" << endl;
    cout << "Player Pieces: X" << endl;
    cout << "Comp Pieces: O" << endl;
    cout << endl;
    cout << "Press [B] to begin, [E] to exit: ";

    cin >> input;
    if (input != 'b' && input != 'B') {
        exit(0);
    }
}

/*
    Initialize the gameboard to all empty spaces.
*/

void setupBoard(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board[i][j] = '-';
        }
    }
}

/*
    Print the contents of the gameboard.
*/

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

/*
    Print the appropriate message for a win based on if the victor is the player or the comp.
*/

void printWin(bool isPlayer) {
    printBoard(gameBoard);
    if (isPlayer) {
        cout << endl;
        cout << "Player Wins!" << endl;
        cout << "GAME OVER" << endl;
    } else {
        cout << endl;
        cout << "Comp Wins!" << endl;
        cout << "GAME OVER" << endl;
    }
    cout << "\nPlay Again?" << endl;
    cout << "[Y]es or [N]o : ";
    char input;
    cin >> input;
    if (input == 'Y' || input == 'y') {
        resetState = true;
    } else {
        resetState = false;
        keepPlaying = false;
    }
}

/*
    Print an appropirate message if the game ends in a draw.
*/

void printDraw() {
    printBoard(gameBoard);
    cout << "It's a DRAW!" << endl;
    cout << "GAME OVER" << endl;
    cout << "\nPlay Again?" << endl;
    cout << "[Y]es or [N]o : ";
    char input;
    cin >> input;
    if (input == 'Y' || input == 'y') {
        resetState = true;
    } else {
        resetState = false;
        keepPlaying = false;
    }
}

/*
    Reset the game state to play another round.
*/

void reset() {
    cout << "\n\n\n\n\n";
    printWelcome();
    srand(time(0));
    setupBoard(gameBoard);
    printBoard(gameBoard);
    keepPlaying = true;
    resetState = false;
}

/*
    Get user input for row, col and place a new piece.
*/

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
    setLastPlayerMove(rowNum, colNum);
    if (threeRow(rowNum, colNum, true)) {
        printWin(true);
    } else {
        if (isDraw()) {
            printDraw();
        }
    }
}

/*
    Function for the AI to make "smarter" decisions. Attempt to find an empty space adjacent to the player's last move.
    If all 3 attempts fail, resort to random empty space.
*/

void compTurn() {
    int decision = rand() % 2;
    int rowCoor = rand() % 3;
    int colCoor = rand() % 3;

    if (decision == 0) {
        unsigned int attempts = 0;
        while (gameBoard[rowCoor][colCoor] != '-' || 
            (abs(rowCoor - lastPlayerMove.row) <= 1) == false ||
            (abs(colCoor - lastPlayerMove.col) <= 1) == false) {
                if (!abs(rowCoor - lastPlayerMove.row) <= 1) {
                    rowCoor = rand() % 3;
                }
                if (!abs(colCoor - lastPlayerMove.col) <= 1) {
                    colCoor = rand() % 3;
                }
                attempts++;
                if (attempts == 3) {
                    std::tuple<int, int> result = fallBackCoordinates(rowCoor, colCoor);
                    rowCoor = std::get<0>(result);
                    colCoor = std::get<1>(result);
                    break;
                }   //if unable to find empty adjacent space within 3 attempts, fall back to random coordinates
        }   //find valid coordinates adjacent to the player's last move
    } else {

        std::tuple<int, int> result = fallBackCoordinates(rowCoor, colCoor);
        rowCoor = std::get<0>(result);
        colCoor = std::get<1>(result);
    }
	
	placePiece(rowCoor, colCoor, false);
    if (threeRow(rowCoor, colCoor, false)) {
        printWin(false);
    } else {
        if (isDraw()) {
            printDraw();
        }
    }
}

/*
    Resort to using random coordinates once the "smart foordinates" functionality fails.
    Return a tuple of the row, col values for the new piece.
*/

std::tuple<int, int> fallBackCoordinates(int rowCoor, int colCoor) {
    while (gameBoard[rowCoor][colCoor] != '-') {
            rowCoor = rand() % 3;
            if (gameBoard[rowCoor][colCoor] == '-') {
                break;
            } else {
                colCoor = rand() % 3;
            }
    } //validate the row/col coordinates chosen
    return std::make_tuple(rowCoor, colCoor);
}

/*
    Place a char on the gameboard at the given coordinates.
    Place 'X' for player, 'O' for comp.
*/

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

/*
    For the purposes of the AI, log the coordinates of the player's last move.
*/

void setLastPlayerMove(int row, int col) {
    lastPlayerMove.row = row;
    lastPlayerMove.col = col;
}

/*
    Return whether the gameboard is full.
*/

bool isDraw() {
    for (int i = 0; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (gameBoard[i][j] == '-') {
                return false;
            }
        }
    }
    return true;
}

/*
    Return a Position instance corresponding to the given coordinates.
*/

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
    char searchPiece = isPlayer ? 'X' : 'O';
    switch (pos) {
        case Position::TopLeft:
            return (checkRight(row, col, searchPiece) ||
                    checkDown(row, col, searchPiece) ||
                    checkDownRight(row, col, searchPiece)
                    );
            break;
        case Position::MidLeft:
            return (checkRight(row, col, searchPiece) ||
                    checkUpDown(row, col, searchPiece)
                    );
            break;
        case Position::BottomLeft:
            return (checkUp(row, col, searchPiece) ||
                    checkRight(row, col, searchPiece) ||
                    checkUpRight(row, col, searchPiece)
                    );
            break;
        case Position::TopMid:
            return (checkDown(row, col, searchPiece) ||
                    checkLeftRight(row, col, searchPiece)
                    );
            break;
        case Position::MidMid:
            return (checkLeftRight(row, col, searchPiece) ||
                    checkUpDown(row, col, searchPiece) ||
                    checkUpLeftDownRight(row, col, searchPiece) ||
                    checkUpRightDownLeft(row, col, searchPiece)
                    );
            break;
        case Position::BottomMid:
            return (checkUp(row, col, searchPiece) ||
                    checkLeftRight(row, col, searchPiece)
                    );
            break;
        case Position::TopRight:
            return (checkLeft(row, col, searchPiece) ||
                    checkDown(row, col, searchPiece) ||
                    checkDownLeft(row, col, searchPiece)
                    );
            break;
        case Position::MidRight:
            return (checkLeft(row, col, searchPiece) ||
                    checkUpDown(row, col, searchPiece)
                    );
            break;
        default:
            return (checkUp(row, col, searchPiece) ||
                    checkLeft(row, col, searchPiece) ||
                    checkUpLeft(row, col, searchPiece)
                    );
            break;
    }
    return false;
}

/*
    Check for a 3-in-a-row connection to the left of the placed piece.
*/

bool checkLeft(int row, int col, char searchPiece) {
    bool result = true;
    for (int i = col - 1; i >= 0; i--) {
        if (gameBoard[row][i] != searchPiece) {
            result = false;
            break;
        }
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the right of the placed piece.
*/

bool checkRight(int row, int col, char searchPiece) {
    bool result = true;
    for (int i = col + 1; i <= 2; i++) {
        if (gameBoard[row][i] != searchPiece) {
            result = false;
            break;
        }
    }
    return result;
}

/*
    Check for a 3-in-a-row connection above the placed piece.
*/

bool checkUp(int row, int col, char searchPiece) {
    bool result = true;
    for (int i = row - 1; i >= 0; i--) {
        if (gameBoard[i][col] != searchPiece) {
            result = false;
            break;
        }
    }
    return result;
}

/*
    Check for a 3-in-a-row connection below the placed piece.
*/

bool checkDown(int row, int col, char searchPiece) {
    bool result = true;
    for (int i = row + 1; i <= 2; i++) {
        if (gameBoard[i][col] != searchPiece) {
            result = false;
            break;
        }
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the up and right of the placed piece.
*/

bool checkUpRight(int row, int col, char searchPiece) {
    bool result = true;
    int irow = row - 1;
    int icol = col + 1;
    while (irow >= 0 && icol <= 2) {
        if (gameBoard[irow][icol] != searchPiece) {
            result = false;
            break;
        }
        irow--;
        icol++;
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the down and left of the placed piece.
*/

bool checkDownRight(int row, int col, char searchPiece) {
    bool result = true;
    int irow = row + 1;
    int icol = col + 1;
    while (irow <= 2 && icol <= 2) {
        if (gameBoard[irow][icol] != searchPiece) {
            result = false;
            break;
        }
        irow++;
        icol++;
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the up and left of the placed piece.
*/

bool checkUpLeft(int row, int col, char searchPiece) {
    bool result = true;
    int irow = row - 1;
    int icol = col - 1;
    while (irow >= 0 && icol >= 0) {
        if (gameBoard[irow][icol] != searchPiece) {
            result = false;
            break;
        }
        irow--;
        icol--;
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the down and left of the placed piece.
*/

bool checkDownLeft(int row, int col, char searchPiece) {
    bool result = true;
    int irow = row + 1;
    int icol = col - 1;
    while (irow <= 2 && icol >= 0) {
        if (gameBoard[irow][icol] != searchPiece) {
            result = false;
            break;
        }
        irow++;
        icol--;
    }
    return result;
}

/*
    Check for a 3-in-a-row connection to the left and right of the placed piece.
*/

bool checkLeftRight(int row, int col, char searchPiece) {
    bool leftResult = true;
    bool rightResult = true;
    if (gameBoard[row][col - 1] != searchPiece) {
        leftResult = false;
    }   //if space to left is not also X/O
    if (gameBoard[row][col + 1] != searchPiece) {
        rightResult = false;
    }   //if space to right is not also X/O
    return (leftResult && rightResult);
}

/*
    Check for a 3-in-a-row connection above and below the placed piece.
*/

bool checkUpDown(int row, int col, char searchPiece) {
    bool upResult = true;
    bool downResult = true;
    if (gameBoard[row - 1][col] != searchPiece) {
        upResult = false;
    }   //if space above is not also X/O
    if (gameBoard[row + 1][col] != searchPiece) {
        downResult = false;
    }   //if space below is not also X/O
    return (upResult && downResult);
}

/*
    Check for a 3-in-a-row connection to the up-right and down-left of the placed piece.
*/

bool checkUpRightDownLeft(int row, int col, char searchPiece) {
    bool upResult = true;
    bool downResult = true;
    if (gameBoard[row - 1][col + 1] != searchPiece) {
        upResult = false;
    }   //if space above & right is not also X/O
    if (gameBoard[row + 1][col - 1] != searchPiece) {
        downResult = false;
    }   //if space down & left is not also X/O
    return (upResult && downResult);
}

/*
    Check for a 3-in-a-row connection to the up-left and down-right of the placed piece.
*/

bool checkUpLeftDownRight(int row, int col, char searchPiece) {
    bool upResult = true;
    bool downResult = true;
    if (gameBoard[row - 1][col - 1] != searchPiece) {
        upResult = false;
    }   //if space above & left is not also X/O
    if (gameBoard[row + 1][col + 1] != searchPiece) {
        downResult = false;
    }   //if space down & right is not also X/O
    return (upResult && downResult);
}