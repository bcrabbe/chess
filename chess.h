//
//  main.h
//  logo
//
//  Created by ben on 18/01/2015.
//  Copyright (c) 2015 ben. All rights reserved.
//

#ifndef chess_main_h
#define chess_main_h
#include "sput.h"
#include "debug.h"
#include <stdbool.h>

/******************************************************************************/
//Options:
#define TESTING 1//runs the test if set
#define VERBOSE 1//prints info to terminal disable for speed.
#define PRINT_ERRORS 1 //turn on/off stderr error messages.
#define MAX_ERROR_STRING_SIZE 600



/******************************************************************************/
//chess.c

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
  //  int position[2];
  bool hasMoved;
  pieceType type;
  bool isTaken;
} piece;

typedef piece *** board;

typedef enum dim {
    X = 0,
    Y = 1
} dim;

board createBoard();
piece * createPiece(color color, pieceType type);
void printPiece(piece * p);
void printBoard(board brd);
void freeBoard(board brd);
void freePieces(board brd);
/******************************************************************************/
//move.c
typedef enum isValidMoveResult {
                                SUCESS,
                                ERROR,
                                TAKES
} isValidMoveResult;

typedef struct isValidMoveResponse {
  isValidMoveResult result;
  char * error;
  piece * takes;
} isValidMoveResponse;

bool isValidMove(board b, int start[2], int end[2]);
void move_unitTests();
#endif
