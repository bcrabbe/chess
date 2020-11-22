#ifndef chess_main_h
#define chess_main_h
#include "sput.h"
#include "debug.h"
#include <stdbool.h>

// chess board size
#define SIZE 8

typedef enum color {
                    BLACK,
                    WHITE
} color;

typedef enum pieceType {
                        BISHOP,
                        CASTLE,
                        KING,
                        KNIGHT,
                        PAWN,
                        QUEEN
} pieceType;

typedef struct piece {
  color color;
  bool hasMoved;
  pieceType type;
  bool isTaken;
} piece;

typedef piece *** board;

typedef enum dim {
    X = 0,
    Y = 1
} dim;

//chess.c
board createBoard();
piece * createPiece(color color, pieceType type);
void printPiece(piece * p);
void printBoard(board brd);
void freeBoard(board brd);
void freePieces(board brd);
board cloneBoard(board brd);
piece * clonePiece(piece * p);
board stringToBoard(char * str);
bool compareBoards(board a, board b);

//move.c
bool isValidMove(board b, int start[2], int end[2]);
void move_unitTests();
#endif
