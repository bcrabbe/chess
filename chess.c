//
//  main.c
//  logo
//
//  Created by ben on 18/01/2015.
//  Copyright (c) 2015 ben. All rights reserved.
//
//#define sprint(s) printf(#s " = ""%s""\n",s);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chess.h"

board createBoard();
board startingPositions(board brd);
piece * createPiece(color color, pieceType type);
void printPiece(piece * p);
void freeBoard(board brd);
void freePieces(board brd);
void printBoard(board brd);
//unit test functions
void unitTests();
void chess_unitTests();
void createBoard_test();

int main(int argc, const char * argv[]) {
  if(TESTING) {
    unitTests();
    return 1;
  }
  if(argc != 2) {
    fprintf(stderr, "ERROR: expected a .txt file path as 1st argument.\nExiting.\n");
    exit(0);
  }
  return 1;
}

/* allocates dynamic 2d array using pointer to array of pointers to rows
   once allocated grid may be passed to function and accessed using standard grid[j][i]*/
board createBoard() {
  board brd = NULL;
  //here we allocate an array to hold the pointers (which will point to the start of each row)
  brd = (board)calloc((size_t)(SIZE), sizeof(piece *));
  if(brd == NULL) {
    printf("calloc failed\n");
    exit(0);
  }
  //here we allocate the proper array which will hold each cell status
  brd[0] = (piece **)calloc((size_t)(SIZE * SIZE), sizeof(piece *));
  if(brd[0] == NULL) {
    printf("calloc failed\n");
    exit(0);
  }
  // here we set each pointer in the first array to the first element of each row
  for(int i = 0; i < SIZE; ++i) {
    brd[i] = brd[0] + (SIZE * i);
  }
  //return pointer to array of pointers to rows
  return brd;
}


const static pieceType TOP_ROW[] = {
                                CASTLE,
                                KNIGHT,
                                BISHOP,
                                KING,
                                QUEEN,
                                BISHOP,
                                KNIGHT,
                                CASTLE,
};

const static pieceType BOTTOM_ROW[] = {
                                CASTLE,
                                KNIGHT,
                                BISHOP,
                                QUEEN,
                                KING,
                                BISHOP,
                                KNIGHT,
                                CASTLE,
};

/* White start at bottom, black at top*/
board startingPositions(board brd) {
  freePieces(brd);
  for(int i = 0; i < SIZE; ++i) {
    brd[i][0] = createPiece(BLACK, TOP_ROW[i]);
    brd[i][1] = createPiece(BLACK, PAWN);
    brd[i][6] = createPiece(WHITE, PAWN);
    brd[i][7] = createPiece(WHITE, BOTTOM_ROW[i]);
  }
  return brd;
}

/* board loadBoard(char * str) { */
/*   board brd = createBoard(); */

/*   const char newline[2] = '\n'; */
/*   char *token; */

/*   /\* get the first token *\/ */
/*   token = strtok(str, newline); */

/*   /\* walk through other tokens *\/ */
/*   while( token != NULL ) { */
/*     printf( " %s\n", token ); */

/*     token = strtok(NULL, s); */
/*   } */

/*   return(0); */
/* } */

piece * createPiece(color color, pieceType type) {
  piece * pce = calloc(1, sizeof(piece));
  pce->color = color;
  pce->hasMoved = false;
  pce->isTaken = false;
  pce->type = type;
  return pce;
}

const static char * pieceTypeString[] = {
                                         "BISHOP",
                                         "CASTLE",
                                         "KING",
                                         "KNIGHT",
                                         "PAWN",
                                         "QUEEN"
};

const static char * colorString[] = {
                                     "BLACK",
                                     "WHITE"
};

void printPiece(piece * p) {
  printf("%s - %s \n", colorString[p->color], pieceTypeString[p->type]);
}

void printBoard(board brd) {
  printf("\nBoard:\n");
  int i,j;
  for(j = 0; j < SIZE; ++j) {
    printf("\n");
    for(i = 0; i < SIZE; ++i) {
      if(brd[i][j] == NULL) {
        sprint(".");
      } else {
        printPiece(brd[i][j]);
      }
    }
  }
  printf("\n");
}

void freePieces(board brd) {
  for(int i = 0; i < SIZE ; ++i) {
      for(int j = 0; j < SIZE; ++j) {
        if(brd[i][j] != NULL) {
          free(brd[i][j]);
        }
      }
  }
}

/* Frees a grid allocated with allocateGrid function */
void freeBoard(board brd) {
  freePieces(brd);
  free((brd[0]));
  free((brd));
}

void unitTests()
{
  printf("********************************************************************\n");
  printf("\n*                       UNIT TESTS                                 *\n\n");
  printf("********************************************************************\n\n");

  chess_unitTests();

  printf("\n\n\n********************************************************************\n");
  printf("\n*                       Testing move.c                             *\n\n");
  printf("********************************************************************\n\n");
  move_unitTests();

}

void chess_unitTests() {

  sput_start_testing();
  sput_set_output_stream(NULL);

  sput_enter_suite("createBoard");
  sput_run_test(createBoard_test);
  sput_leave_suite();

  sput_finish_testing();
}

void createBoard_test() {
  board brd = createBoard();
  startingPositions(brd);
  printBoard(brd);
  freeBoard(brd);
  return;
}

void loadBoard_test() {
  board brd = createBoard();
  printBoard(brd);
  freeBoard(brd);
  return;
}
