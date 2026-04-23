#include <iostream>
#include <windows.h> 
#include "Board.h"

using namespace std;

int main() {
    Board gameBoard;
    Color turn = WHITE;
    bool gameOver = false;
    string command;
    int startX, startY, endX, endY;

    cout << "--- ABDULLAH'S CHESS ---" << endl;
    cout << "Moves: R C R C " << endl;
    cout << "Commands: SAVE, LOAD, EXIT" << endl;

    while (!gameOver) {
        gameBoard.display();

        if (gameBoard.isInCheck(turn)) {
            if (gameBoard.isCheckmate(turn)) {
                Beep(300, 2000);
                cout << "CHECKMATE! " << (turn == WHITE ? "Black" : "White") << " Wins!" << endl;
                break;
            }
            cout << "CHECK!" << endl;
        }

        cout << (turn == WHITE ? "White" : "Black") << "'s turn: ";
        cin >> command;

        if (command == "EXIT") break;
        if (command == "SAVE") {
            gameBoard.saveGame("chess_save.txt", turn);
            continue;
        }
        if (command == "LOAD") {
            if (gameBoard.loadGame("chess_save.txt", turn)) cout << "Game Loaded!" << endl;
            continue;
        }

        startX = stoi(command);
        cin >> startY >> endX >> endY;

        if (gameBoard.tryMove(startX, startY, endX, endY, turn)) {
            Beep(700, 150);
            turn = (turn == WHITE) ? BLACK : WHITE;
        }
        else {
            cout << "Invalid Move!" << endl;
        }
    }

    return 0;
}