#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chess.h"
#include <ctype.h>

board createBoard();
board startingPositions(board brd);
piece * createPiece(color color, pieceType type);
void printPiece(piece * p);
void freeBoard(board brd);
void freePieces(board brd);
void printBoard(board brd);
board cloneBoard(board brd);
piece * clonePiece(piece * p);
piece * charToPiece(char c);
board stringToBoard(char * str);
bool boardsEqual(board a, board b);
//unit test functions
void unitTests();
void chess_unitTests();
void createBoard_test();
void stringToBoard_test();

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
    brd[i][SIZE - 2] = createPiece(WHITE, PAWN);
    brd[i][SIZE - 1] = createPiece(WHITE, BOTTOM_ROW[i]);
  }
  return brd;
}

board stringToBoard(char * str) {
  board brd = createBoard();
  //  char * line = NULL;
  sprint(str);
  int i = 0, j = 0;
  for(j = 0; j < SIZE; ++j) {
    for(i = 0; i < SIZE; ++i) {
      char c = str[i + (j * SIZE)];
      piece * p = charToPiece(c);
      brd[i][j] = p;
    }
  }
  return brd;
}

piece * charToPiece(char c) {
  pieceType type;
  switch(toupper(c)) {
  case 'B':
    type = BISHOP;
    break;
  case 'C':
    type = CASTLE;
    break;
  case 'H':
    type = KNIGHT;
    break;
  case 'K':
    type = KING;
    break;
  case 'Q':
    type = QUEEN;
    break;
  case 'P':
    type = PAWN;
    break;
  default:
    return NULL;
  }
  color color;
  if(isupper((int) c)) {
    color = BLACK;
  } else {
    color = WHITE;
  }
  return createPiece(color, type);
}

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
        printf("-");
      } else {
        printPiece(brd[i][j]);
      }
    }
  }
  printf("\n");
}

board cloneBoard(board brd) {
  board nBrd = createBoard();
  int i, j;
  for(j = 0; j < SIZE; ++j) {
    for(i = 0; i < SIZE; ++i) {
      if(brd[i][j] != NULL) {
        nBrd[i][j] = clonePiece(brd[i][j]);
      }
    }
  }
  return nBrd;
}

piece * clonePiece(piece * p) {
  if(p == NULL) return NULL;
  piece * clone = calloc(1, sizeof(piece));
  return memcpy(clone, p, sizeof(piece));
}

bool boardsEqual(board a, board b) {
  int i, j;
  for(j = 0; j < SIZE; ++j) {
    for(i = 0; i < SIZE; ++i) {
      if(a[i][j] != NULL && b[i][j] != NULL) {
        if(memcmp(a[i][j], b[i][j], sizeof(piece)) != 0 ) return false;
      } else if (a[i][j] != b[i][j]) {
        return false;
      }
    }
  }
  return true;
}

void freePieces(board brd) {
  for(int i = 0; i < SIZE ; ++i) {
      for(int j = 0; j < SIZE; ++j) {
        if(brd[i][j] != NULL) {
          free(brd[i][j]);
          brd[i][j] = NULL;
        }
      }
  }
}

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

  sput_enter_suite("stringToBoard");
  sput_run_test(stringToBoard_test);
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

void stringToBoard_test() {
  board parsedInitialBoard = stringToBoard("CHBKQBHC\
PPPPPPPP\
--------\
--------\
--------\
--------\
pppppppp\
chbqkbhc");
  printBoard(parsedInitialBoard);
  board createdInitialBoard = createBoard();
  startingPositions(createdInitialBoard);
  sput_fail_unless(boardsEqual(parsedInitialBoard, createdInitialBoard), "parsedBoard with starting positions equals the a boards initialised using startingPositions f.");
  freeBoard(createdInitialBoard);
  freeBoard(parsedInitialBoard);
  return;
}
