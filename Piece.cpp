#include "Piece.h"
#include <cmath>

Piece::Piece(Color c, char s, PieceType t) : color(c), symbol(s), type(t), hasMoved(false) {}

Piece::~Piece() {}

void Piece::setMoved() {
    hasMoved = true;
}

King::King(Color c) : Piece(c, c == WHITE ? 'K' : 'k', KING) {}

bool King::isValidMove(int startX, int startY, int endX, int endY) {
    int dX = abs(startX - endX);
    int dY = abs(startY - endY);
    if (dX <= 1 && dY <= 1) return true;
    if (dX == 2 && dY == 0 && !hasMoved) return true;
    return false;
}

Queen::Queen(Color c) : Piece(c, c == WHITE ? 'Q' : 'q', QUEEN) {}

bool Queen::isValidMove(int startX, int startY, int endX, int endY) {
    return (startX == endX || startY == endY) || (abs(startX - endX) == abs(startY - endY));
}

Bishop::Bishop(Color c) : Piece(c, c == WHITE ? 'B' : 'b', BISHOP) {}

bool Bishop::isValidMove(int startX, int startY, int endX, int endY) {
    return abs(startX - endX) == abs(startY - endY);
}

Knight::Knight(Color c) : Piece(c, c == WHITE ? 'N' : 'n', KNIGHT) {}

bool Knight::isValidMove(int startX, int startY, int endX, int endY) {
    return (abs(startX - endX) == 2 && abs(startY - endY) == 1) ||
        (abs(startX - endX) == 1 && abs(startY - endY) == 2);
}

Rook::Rook(Color c) : Piece(c, c == WHITE ? 'R' : 'r', ROOK) {}

bool Rook::isValidMove(int startX, int startY, int endX, int endY) {
    return startX == endX || startY == endY;
}

Pawn::Pawn(Color c) : Piece(c, c == WHITE ? 'P' : 'p', PAWN) {}

bool Pawn::isValidMove(int startX, int startY, int endX, int endY) {
    int dir = (color == WHITE) ? -1 : 1;
    if (startY == endY && endX == startX + dir) return true;
    if (startY == endY && endX == startX + (2 * dir) && !hasMoved) return true;
    if (abs(startY - endY) == 1 && endX == startX + dir) return true;
    return false;
}