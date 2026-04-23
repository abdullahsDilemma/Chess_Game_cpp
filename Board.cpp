/*#include "Board.h"
#include <iostream>
#include <fstream>
#include <cctype>

using namespace std;

Board::Board() {
    lastPawnDoubleMoveCol = -1;
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) board[i][j] = nullptr;
    setupBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) delete board[i][j];
}

void Board::setupBoard() {
    board[0][0] = new Rook(BLACK); board[0][1] = new Knight(BLACK); board[0][2] = new Bishop(BLACK);
    board[0][3] = new Queen(BLACK); board[0][4] = new King(BLACK); board[0][5] = new Bishop(BLACK);
    board[0][6] = new Knight(BLACK); board[0][7] = new Rook(BLACK);
    for (int i = 0; i < 8; i++) board[1][i] = new Pawn(BLACK);

    board[7][0] = new Rook(WHITE); board[7][1] = new Knight(WHITE); board[7][2] = new Bishop(WHITE);
    board[7][3] = new Queen(WHITE); board[7][4] = new King(WHITE); board[7][5] = new Bishop(WHITE);
    board[7][6] = new Knight(WHITE); board[7][7] = new Rook(WHITE);
    for (int i = 0; i < 8; i++) board[6][i] = new Pawn(WHITE);
}

void Board::display() {
    cout << "  0 1 2 3 4 5 6 7" << endl;
    cout << "  ---------------" << endl;
    for (int i = 0; i < 8; i++) {
        cout << i << "|";
        for (int j = 0; j < 8; j++) {
            if (board[i][j]) cout << board[i][j]->symbol << " ";
            else cout << ". ";
        }
        cout << "|" << endl;
    }
}

bool Board::isPathClear(int startX, int startY, int endX, int endY) {
    int dx = endX - startX;
    int dy = endY - startY;
    int xDir = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int yDir = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = startX + xDir;
    int y = startY + yDir;

    while (x != endX || y != endY) {
        if (board[x][y] != nullptr) return false;
        x += xDir;
        y += yDir;
    }
    return true;
}

bool Board::isInCheck(Color color) {
    int kx, ky;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->type == KING && board[i][j]->color == color) {
                kx = i; ky = j; break;
            }
        }
    }

    Color enemyColor = (color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* p = board[i][j];
            if (p && p->color == enemyColor) {
                if (p->isValidMove(i, j, kx, ky)) {
                    if (p->type == KNIGHT || isPathClear(i, j, kx, ky)) {
                        if (p->type == PAWN) {
                            if (abs(j - ky) == 1) return true;
                        }
                        else {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->color == color) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (isMoveLegal(i, j, x, y, color)) return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isMoveLegal(int startX, int startY, int endX, int endY, Color turnColor) {
    if (startX < 0 || startX > 7 || endX < 0 || endX > 7) return false;
    Piece* src = board[startX][startY];
    Piece* dest = board[endX][endY];

    if (!src || src->color != turnColor) return false;
    if (dest && dest->color == turnColor) return false;

    if (!src->isValidMove(startX, startY, endX, endY)) return false;
    if (src->type != KNIGHT && !isPathClear(startX, startY, endX, endY)) return false;

    if (src->type == PAWN) {
        if (startY == endY && dest != nullptr) return false;
        if (abs(startY - endY) == 1 && dest == nullptr) {
            if (abs(startX - endX) == 1 && endY == lastPawnDoubleMoveCol) {
                int enemyPawnRow = startX;
                if (board[enemyPawnRow][endY] != nullptr && board[enemyPawnRow][endY]->type == PAWN && board[enemyPawnRow][endY]->color != turnColor) {
                    return true;
                }
            }
            return false;
        }
    }

    board[endX][endY] = src;
    board[startX][startY] = nullptr;

    bool kingSafe = !isInCheck(turnColor);

    board[startX][startY] = src;
    board[endX][endY] = dest;

    return kingSafe;
}

bool Board::tryMove(int startX, int startY, int endX, int endY, Color turnColor) {
    Piece* src = board[startX][startY];

    if (src && src->type == KING && abs(startY - endY) == 2) {
        return handleCastling(startX, startY, endX, endY);
    }

    if (!isMoveLegal(startX, startY, endX, endY, turnColor)) return false;

    if (src->type == PAWN && abs(startY - endY) == 1 && board[endX][endY] == nullptr) {
        return handleEnPassant(startX, startY, endX, endY);
    }

    Piece* dest = board[endX][endY];
    board[endX][endY] = src;
    board[startX][startY] = nullptr;
    delete dest;

    if (src->type == PAWN && abs(startX - endX) == 2) lastPawnDoubleMoveCol = endY;
    else lastPawnDoubleMoveCol = -1;

    if (src->type == PAWN && (endX == 0 || endX == 7)) {
        promotePawn(endX, endY);
    }

    src->setMoved();
    return true;
}

void Board::promotePawn(int x, int y) {
    cout << "Pawn Promotion! Enter Q, R, B, or N: ";
    char choice;
    cin >> choice;
    Color c = board[x][y]->color;
    delete board[x][y];

    switch (toupper(choice)) {
    case 'R': board[x][y] = new Rook(c); break;
    case 'B': board[x][y] = new Bishop(c); break;
    case 'N': board[x][y] = new Knight(c); break;
    default:  board[x][y] = new Queen(c); break;
    }
}

bool Board::handleCastling(int startX, int startY, int endX, int endY) {
    Piece* king = board[startX][startY];
    if (isInCheck(king->color)) return false;

    int rookY = (endY > startY) ? 7 : 0;
    Piece* rook = board[startX][rookY];

    if (!rook || rook->type != ROOK || rook->hasMoved) return false;

    int step = (endY > startY) ? 1 : -1;
    for (int y = startY + step; y != rookY; y += step) {
        if (board[startX][y] != nullptr) return false;
    }

    int passedY = startY + step;
    board[startX][passedY] = king;
    board[startX][startY] = nullptr;
    if (isInCheck(king->color)) {
        board[startX][startY] = king;
        board[startX][passedY] = nullptr;
        return false;
    }
    board[startX][startY] = king;
    board[startX][passedY] = nullptr;

    board[endX][endY] = king;
    board[startX][startY] = nullptr;
    king->setMoved();

    int rookNewY = endY - step;
    board[startX][rookNewY] = rook;
    board[startX][rookY] = nullptr;
    rook->setMoved();

    return true;
}

bool Board::handleEnPassant(int startX, int startY, int endX, int endY) {
    Piece* src = board[startX][startY];
    if (!src || src->type != PAWN || abs(startY - endY) != 1 || board[endX][endY] != nullptr)
        return false;

    delete board[startX][endY];
    board[startX][endY] = nullptr;

    board[endX][endY] = src;
    board[startX][startY] = nullptr;
    src->setMoved();

    lastPawnDoubleMoveCol = -1;

    return true;
}

void Board::saveGame(string filename, Color turn) {
    ofstream file(filename);
    if (!file) return;

    file << (turn == WHITE ? "W" : "B") << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                file << board[i][j]->symbol << " " << board[i][j]->hasMoved << " ";
            else
                file << "XX 0 ";
        }
        file << endl;
    }
    file.close();
    cout << "Game Saved to " << filename << endl;
}

bool Board::loadGame(string filename, Color& turn) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found!" << endl;
        return false;
    }

    char turnChar;
    file >> turnChar;
    turn = (turnChar == 'W') ? WHITE : BLACK;

    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) { delete board[i][j]; board[i][j] = nullptr; }

    string sym;
    bool moved;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            file >> sym >> moved;
            if (sym == "XX") continue;

            Color c = isupper(sym[0]) ? WHITE : BLACK;
            char s = sym[0];
            Piece* p = nullptr;
            switch (tolower(s)) {
            case 'k': p = new King(c); break;
            case 'q': p = new Queen(c); break;
            case 'r': p = new Rook(c); break;
            case 'b': p = new Bishop(c); break;
            case 'n': p = new Knight(c); break;
            case 'p': p = new Pawn(c); break;
            }
            if (p) {
                if (moved) p->setMoved();
                board[i][j] = p;
            }
        }
    }
    return true;
}*/








// this is the other one with grapics

#include "Board.h"
#include <iostream>
#include <fstream>
#include <cctype>
#include <windows.h> 

using namespace std;

void color(int bg, int txt) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg << 4) | txt);
}

Board::Board() {
    lastPawnDoubleMoveCol = -1;
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) board[i][j] = nullptr;
    setupBoard();
}

Board::~Board() {
    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) delete board[i][j];
}

void Board::setupBoard() {
    board[0][0] = new Rook(BLACK); board[0][1] = new Knight(BLACK); board[0][2] = new Bishop(BLACK);
    board[0][3] = new Queen(BLACK); board[0][4] = new King(BLACK); board[0][5] = new Bishop(BLACK);
    board[0][6] = new Knight(BLACK); board[0][7] = new Rook(BLACK);
    for (int i = 0; i < 8; i++) board[1][i] = new Pawn(BLACK);

    board[7][0] = new Rook(WHITE); board[7][1] = new Knight(WHITE); board[7][2] = new Bishop(WHITE);
    board[7][3] = new Queen(WHITE); board[7][4] = new King(WHITE); board[7][5] = new Bishop(WHITE);
    board[7][6] = new Knight(WHITE); board[7][7] = new Rook(WHITE);
    for (int i = 0; i < 8; i++) board[6][i] = new Pawn(WHITE);
}

void Board::display() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    cout << "\n   ";
    for (int i = 0; i < 8; i++) cout << " " << i << " ";
    cout << "\n";

    for (int i = 0; i < 8; i++) {
        SetConsoleTextAttribute(hConsole, 15);
        cout << " " << i << " ";

        for (int j = 0; j < 8; j++) {
            int bg = ((i + j) % 2 == 0) ? 7 : 8;
            int txt = 0;

            if (board[i][j]) {
                txt = (board[i][j]->color == WHITE) ? 1 : 4;
            }
            else {
                txt = bg;
            }

            color(bg, txt);

            if (board[i][j])
                cout << " " << board[i][j]->symbol << " ";
            else
                cout << "   ";
        }

        SetConsoleTextAttribute(hConsole, 15);
        cout << "\n";
    }
    cout << "\n";
}

bool Board::isPathClear(int startX, int startY, int endX, int endY) {
    int dx = endX - startX;
    int dy = endY - startY;
    int xDir = (dx == 0) ? 0 : (dx > 0 ? 1 : -1);
    int yDir = (dy == 0) ? 0 : (dy > 0 ? 1 : -1);

    int x = startX + xDir;
    int y = startY + yDir;

    while (x != endX || y != endY) {
        if (board[x][y] != nullptr) return false;
        x += xDir;
        y += yDir;
    }
    return true;
}

bool Board::isInCheck(Color color) {
    int kx, ky;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->type == KING && board[i][j]->color == color) {
                kx = i; ky = j; break;
            }
        }
    }

    Color enemyColor = (color == WHITE) ? BLACK : WHITE;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece* p = board[i][j];
            if (p && p->color == enemyColor) {
                if (p->isValidMove(i, j, kx, ky)) {
                    if (p->type == KNIGHT || isPathClear(i, j, kx, ky)) {
                        if (p->type == PAWN) {
                            if (abs(j - ky) == 1) return true;
                        }
                        else {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool Board::isCheckmate(Color color) {
    if (!isInCheck(color)) return false;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j] && board[i][j]->color == color) {
                for (int x = 0; x < 8; x++) {
                    for (int y = 0; y < 8; y++) {
                        if (isMoveLegal(i, j, x, y, color)) return false;
                    }
                }
            }
        }
    }
    return true;
}

bool Board::isMoveLegal(int startX, int startY, int endX, int endY, Color turnColor) {
    if (startX < 0 || startX > 7 || endX < 0 || endX > 7) return false;
    Piece* src = board[startX][startY];
    Piece* dest = board[endX][endY];

    if (!src || src->color != turnColor) return false;
    if (dest && dest->color == turnColor) return false;

    if (!src->isValidMove(startX, startY, endX, endY)) return false;
    if (src->type != KNIGHT && !isPathClear(startX, startY, endX, endY)) return false;

    if (src->type == PAWN) {
        if (startY == endY && dest != nullptr) return false;
        if (abs(startY - endY) == 1 && dest == nullptr) {
            if (abs(startX - endX) == 1 && endY == lastPawnDoubleMoveCol) {
                int enemyPawnRow = startX;
                if (board[enemyPawnRow][endY] != nullptr && board[enemyPawnRow][endY]->type == PAWN && board[enemyPawnRow][endY]->color != turnColor) {
                    return true;
                }
            }
            return false;
        }
    }

    board[endX][endY] = src;
    board[startX][startY] = nullptr;

    bool kingSafe = !isInCheck(turnColor);

    board[startX][startY] = src;
    board[endX][endY] = dest;

    return kingSafe;
}

bool Board::tryMove(int startX, int startY, int endX, int endY, Color turnColor) {
    Piece* src = board[startX][startY];

    if (src && src->type == KING && abs(startY - endY) == 2) {
        return handleCastling(startX, startY, endX, endY);
    }

    if (!isMoveLegal(startX, startY, endX, endY, turnColor)) return false;

    if (src->type == PAWN && abs(startY - endY) == 1 && board[endX][endY] == nullptr) {
        return handleEnPassant(startX, startY, endX, endY);
    }

    Piece* dest = board[endX][endY];
    board[endX][endY] = src;
    board[startX][startY] = nullptr;
    delete dest;

    if (src->type == PAWN && abs(startX - endX) == 2) lastPawnDoubleMoveCol = endY;
    else lastPawnDoubleMoveCol = -1;

    if (src->type == PAWN && (endX == 0 || endX == 7)) {
        promotePawn(endX, endY);
    }

    src->setMoved();
    return true;
}

bool Board::handleEnPassant(int startX, int startY, int endX, int endY) {
    Piece* src = board[startX][startY];
    if (!src || src->type != PAWN || abs(startY - endY) != 1 || board[endX][endY] != nullptr)
        return false;

    delete board[startX][endY];
    board[startX][endY] = nullptr;

    board[endX][endY] = src;
    board[startX][startY] = nullptr;
    src->setMoved();
    lastPawnDoubleMoveCol = -1;
    return true;
}

void Board::promotePawn(int x, int y) {
    cout << "Pawn Promotion! Enter Q, R, B, or N: ";
    char choice;
    cin >> choice;
    Color c = board[x][y]->color;
    delete board[x][y];

    switch (toupper(choice)) {
    case 'R': board[x][y] = new Rook(c); break;
    case 'B': board[x][y] = new Bishop(c); break;
    case 'N': board[x][y] = new Knight(c); break;
    default:  board[x][y] = new Queen(c); break;
    }
}

bool Board::handleCastling(int startX, int startY, int endX, int endY) {
    Piece* king = board[startX][startY];
    if (isInCheck(king->color)) return false;

    int rookY = (endY > startY) ? 7 : 0;
    Piece* rook = board[startX][rookY];

    if (!rook || rook->type != ROOK || rook->hasMoved) return false;

    int step = (endY > startY) ? 1 : -1;
    for (int y = startY + step; y != rookY; y += step) {
        if (board[startX][y] != nullptr) return false;
    }

    int passedY = startY + step;
    board[startX][passedY] = king;
    board[startX][startY] = nullptr;
    if (isInCheck(king->color)) {
        board[startX][startY] = king;
        board[startX][passedY] = nullptr;
        return false;
    }
    board[startX][startY] = king;
    board[startX][passedY] = nullptr;

    board[endX][endY] = king;
    board[startX][startY] = nullptr;
    king->setMoved();

    int rookNewY = endY - step;
    board[startX][rookNewY] = rook;
    board[startX][rookY] = nullptr;
    rook->setMoved();

    return true;
}

void Board::saveGame(string filename, Color turn) {
    ofstream file(filename);
    if (!file) return;

    file << (turn == WHITE ? "W" : "B") << endl;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (board[i][j])
                file << board[i][j]->symbol << " " << board[i][j]->hasMoved << " ";
            else
                file << "XX 0 ";
        }
        file << endl;
    }
    file.close();
    cout << "Game Saved to " << filename << endl;
}

bool Board::loadGame(string filename, Color& turn) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found!" << endl;
        return false;
    }

    char turnChar;
    file >> turnChar;
    turn = (turnChar == 'W') ? WHITE : BLACK;

    for (int i = 0; i < 8; i++) for (int j = 0; j < 8; j++) { delete board[i][j]; board[i][j] = nullptr; }

    string sym;
    bool moved;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            file >> sym >> moved;
            if (sym == "XX") continue;

            Color c = isupper(sym[0]) ? WHITE : BLACK;
            char s = sym[0];
            Piece* p = nullptr;
            switch (tolower(s)) {
            case 'k': p = new King(c); break;
            case 'q': p = new Queen(c); break;
            case 'r': p = new Rook(c); break;
            case 'b': p = new Bishop(c); break;
            case 'n': p = new Knight(c); break;
            case 'p': p = new Pawn(c); break;
            }
            if (p) {
                if (moved) p->setMoved();
                board[i][j] = p;
            }
        }
    }
    return true;
}