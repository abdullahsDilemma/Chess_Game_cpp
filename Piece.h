#ifndef PIECE_H
#define PIECE_H

#include <cmath>
#include <iostream>

using namespace std;

enum Color { WHITE, BLACK, NONE };
enum PieceType { KING, QUEEN, BISHOP, KNIGHT, ROOK, PAWN, EMPTY };

class Piece {
public:
    Color color;
    char symbol;
    PieceType type;
    bool hasMoved;

    Piece(Color c, char s, PieceType t);
    virtual ~Piece();
    virtual bool isValidMove(int startX, int startY, int endX, int endY) = 0;
    void setMoved();
};

class King : public Piece {
public:
    King(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Queen : public Piece {
public:
    Queen(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Bishop : public Piece {
public:
    Bishop(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Knight : public Piece {
public:
    Knight(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Rook : public Piece {
public:
    Rook(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

class Pawn : public Piece {
public:
    Pawn(Color c);
    bool isValidMove(int startX, int startY, int endX, int endY) override;
};

#endif