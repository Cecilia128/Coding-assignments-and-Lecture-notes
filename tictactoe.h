#ifndef TICTACTOE_H
#define TICTACTOE_H
#include <grid.h>
#include <gwindow.h>
#include "testing/SimpleTest.h"
enum Player {
    COMPUTER,
    USER,
    NONE
};

class TicTacToe
{
    Grid<Player> grid;

public:
    /*
     * TicTacToe constructor
     * -------------------------
     * Initilize all elements in grid to be NONE.
     */
    TicTacToe();

    /*
     * void display()
     * ------------------------
     * show the current state of grid in the screen.
     */
    void display();

    /*
     * int evalue()
     * ------------------------
     * evaluate current state, it's possible that COMPUTER or USER wins, or it's a cat's game, or the game is not over
     */
    int evaluate();

    /*
     * bool isMoveLeft()
     * ------------------------
     * evaluate whether there are NONE left to move
     */
    bool isMoveLeft();

    /*
     * void set(int row, int column, Player player)
     * ----------------------------------------------
     * set the position in grid[row][column] to be player
     */
    void set(int row, int column, Player player);

    /* int minimax(int depth, bool isMax)
     * -----------------------------------------------
     * Recursively, maximize the score when it's COMPUTER's turn and minimize the score when it's USER's turn.
     * If it takes longer steps to end the game, the score will be lower.
     */
    int minimax(int depth, bool isMax);

    /* int findBestMove();
     * ------------------------
     * use minimax algorithm to find current best move for computer s.t. it's most possible for COMPUTER to win and least for USER.
     */
    int findBestMove();

    /* Player get(int row, int column);
     * -----------------------------------
     * return the player in grid[row][column]
     */
    Player get(int row, int column);
};

void start_game();


#endif // TICTACTOE_H
