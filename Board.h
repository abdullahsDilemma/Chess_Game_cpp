#ifndef BOARD_H
#define BOARD_H

#include "Piece.h"
#include <vector>
#include <string>

class Board {
private:
    Piece* board[8][8];
    int lastPawnDoubleMoveCol;

public:
    Board();
    ~Board();
    //void color(int bg, int txt);


    void setupBoard();
    void display();

    bool tryMove(int startX, int startY, int endX, int endY, Color turnColor);
    bool isPathClear(int startX, int startY, int endX, int endY);

    bool isInCheck(Color kingColor);
    bool isCheckmate(Color kingColor);

    bool isMoveLegal(int startX, int startY, int endX, int endY, Color turnColor);

    void promotePawn(int x, int y);
    bool handleCastling(int startX, int startY, int endX, int endY);
    bool handleEnPassant(int startX, int startY, int endX, int endY);

    void saveGame(string filename, Color turn);
    bool loadGame(string filename, Color& turn);
};

#endif