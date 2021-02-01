#include "tictactoe.h"
#include <iostream>
#include <string>
using namespace std;

void start_game() {
    cout << endl << "Welcome to TicTacToe, the game of three in a row." << endl;
    cout << "I will be X, and you'll be O." << endl;
    cout << "The squares are numbered like this: " << endl;
    // display how the squares are numbered
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            cout << " " << 3*i+(j+1);
            if (j<2) cout << " |";
        }
        cout <<endl;
        if (i<2) cout << "---+---+---" << endl;
    }

    // initialize the game
    TicTacToe game;
    while (true) {
        // when COMPUTER wins
        if (game.evaluate() == 10) {
            cout << "I win." << endl;
            break;
        }

        // when USER wins
        else if (game.evaluate() == -10) {
            cout << "You win." << endl;
            break;
        }

        // when nobody wins
        else if (game.evaluate() == 1) {
            cout << "It's cat's game" << endl;
            break;
        }

        // when the game is not over
        else {

            // use minimax algorithm to find the best move for COMPUTER, then move to it
            int move = game.findBestMove();
            game.set(int((move-1)/3), ((move-1)%3), COMPUTER);
            cout << "I'll move to " << move << "." << endl << "The game now looks like this:" << endl;
            game.display();

            // evaluate current state again
            if (game.evaluate() == 10) {
                cout << "I win." << endl;
                break;
            }
            else if (game.evaluate() == -10) {
                cout << "You win." << endl;
                break;
            }
            else if (game.evaluate() == 1) {
                cout << "It's cat's game" << endl;
                break;
            }

            // promote USER to move
            cout << "Your move." << endl << "What square?";
            int user;

            // avoid user to choose occupied position
            while (true) {
                cin >> user;
                if (game.get(int((user-1)/3),(user-1)%3) != NONE) {
                    cout << "This position has been occupied! Please try again!";
                    continue;
                }
                else break;
            }
            game.set(int((user-1)/3),(user-1)%3,USER);
            cout << "The game now looks like this:" << endl;
            game.display();
        }
    }


}


TicTacToe::TicTacToe()
{
    // initialize the grid to be 3x3 and filled with NONE
    grid.resize(3,3);
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            grid[i][j] = NONE;
        }
    }
}

int TicTacToe::evaluate() {
    // check for rows for X or O victory
    for (int i=0; i<3; i++) {
        if (grid[i][1] == grid[i][0] && grid[i][1] == grid[i][2]) {
            if (grid[i][0] == COMPUTER) return 10;
            else if (grid[i][0] == USER) return -10;
        }
    }

    // check for columns for X or O victory
    for (int i=0; i<3; i++) {
        if (grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) {
            if (grid[0][i] == COMPUTER) return 10;
            else if (grid[0][i] == USER) return -10;
        }
    }

    // check for diagonals for X or O victory
    if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
        if (grid[0][0] == COMPUTER) return 10;
        else if (grid[0][0] == USER) return -10;
    }

    if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
        if (grid[0][2] == COMPUTER) return 10;
        else if (grid[0][2] == USER) return -10;
    }

    // if none of them have won
    if (!isMoveLeft()) return 1;

    // if the game is not over
    return 0;
}

bool TicTacToe::isMoveLeft() {
    // if there exists position to be NONE, return true; otherwier, return false
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (grid[i][j] == NONE) return true;
        }
    }
    return false;
}

void TicTacToe::set(int row, int column, Player player) {
    // set the position in grid[row][column] to be corresponding player
    if (player == USER) grid[row][column] = USER;
    else grid[row][column] = COMPUTER;
}

Player TicTacToe::get(int row, int column) {
    return grid[row][column];
}
int TicTacToe::minimax(int depth, bool isMax) {
    int score = evaluate();
    // if computer has won the game
    if (score == 10) return score-depth;

    // if user has won the game
    if (score == -10) return score-depth;

    // if there are no more moves and no winner
    if (isMoveLeft() == false) return 0-depth;

    // if it's COMPUTER's move
    if (isMax) {
        int best = 1000;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                if (grid[i][j] == NONE) {
                    grid[i][j] = COMPUTER;
                    best = min(best, minimax(depth+1, !isMax));
                    grid[i][j] = NONE;
                }
            }
        }
        return best;
    }
    // if it's USER's move
    else {
        int best = -1000;
        for (int i=0; i<3; i++) {
            for (int j=0; j<3; j++) {
                if (grid[i][j] == NONE) {
                    grid[i][j] = USER;
                    best = max(best, minimax(depth+1, !isMax));
                    grid[i][j] = NONE;
                }
            }
        }
        return best;
    }
}

int TicTacToe::findBestMove() {
    int bestVal = -1000;
    int row = -1;
    int col = -1;
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            if (grid[i][j] == NONE) {
                grid[i][j] = COMPUTER;
                int moveVal = minimax(0,true);
                grid[i][j] = NONE;
                // find the first position reaches bestVal
                if (moveVal > bestVal) {
                    bestVal = moveVal;
                    row = i;
                    col = j;
                }
            }
        }
    }
    return 3*row+col+1;
}
void TicTacToe:: display() {
    for (int i=0; i<3; i++) {
        for (int j=0; j<3; j++) {
            char player;
            // use 'O' to represent USER and 'X' to represent COMPUTER; otherwise, use ' ' to represent NONE
            if (grid[i][j] == USER) player = 'O';
            else if (grid[i][j] == COMPUTER) player = 'X';
            else player = ' ';
            cout << " " << player;
            if (j<2) cout << " |";
        }
        cout <<endl;
        if (i<2) cout << "---+---+---" << endl;
    }
}



PROVIDED_TEST("tic tac toe", TTT) {
    start_game();
}

