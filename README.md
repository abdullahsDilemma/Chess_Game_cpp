# C++ Chess Game

## Overview

This project is a console-based Chess game implemented in C++. It simulates a complete two-player chess experience with full rule enforcement, including legal move validation, check/checkmate detection, castling, en passant, pawn promotion, and game saving/loading functionality.

The game is designed using object-oriented programming principles, where each chess piece is represented as a class derived from a base `Piece` class. The board state is managed through a dynamic 8×8 grid of pointers.

---

## Features

* Complete 8×8 chess board representation
* Object-oriented design for all chess pieces
* Legal move validation for all pieces
* Check and checkmate detection
* Special moves:

  * Castling
  * En Passant
  * Pawn Promotion
* Game save and load system
* Turn-based gameplay logic

---

## Technologies Used

* C++
* Object-Oriented Programming (OOP)
* File handling (`fstream`)
* Standard libraries (`iostream`, `cctype`, `cmath`)

---

## How It Works

* The board is represented as a 2D array of `Piece*`
* Each piece type overrides movement logic using polymorphism
* Moves are validated through a combination of:

  * Piece-specific rules
  * Path checking
  * King safety (check prevention)
* Special rules like castling and en passant are handled separately in the board logic

---

## Controls (Conceptual)

Moves are processed using coordinate input:

```
startX startY → endX endY
```

Example:

```
6 4 → 4 4
```

---

## Save / Load Feature

The game can save the current board state to a file and reload it later, preserving:

* Piece positions
* Turn information
* Move history flags

---

## Future Improvements

* Add graphical interface (SFML / Qt)
* Add AI opponent
* Add move history / undo system
* Improve checkmate validation efficiency

---

## Author

Muhammad Abdullah Khan
