#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "chess.h"
#include <stdbool.h>

bool isValidMove(board brd, int start[2], int end[2]);
bool bishop(board brd, int start[2], int end[2]);
bool castle(board brd, int start[2], int end[2]);
bool king(board brd, int start[2], int end[2]);
bool knight(board brd, int start[2], int end[2]);
bool pawn(board brd, int start[2], int end[2]);
bool queen(board brd, int start[2], int end[2]);
bool isEnPassant(board brd, int start[2], int end[2]);
int sign(int x);
bool isWithinBoard(int position[2]);
bool isChecked(board brd, int spot[2], color color);
bool isKingChecked(board brd, color color);
bool isKingCheckedAfterMove(board brd, color color, int start[2], int end[2]);

//unit test functions
void move_unitTests();
void common_unitTest();
void pawn_unitTest();
void bishop_unitTest();
void knight_unitTest();
void isChecked_unitTest();
void king_unitTest();
void castle_unitTest();

bool isValidMove(board brd, int start[2], int end[2]) {
  if(!isWithinBoard(start) || !isWithinBoard(end) || (start[X] == end[X] && start[Y] == end[Y])) return false;
  piece * mover = brd[start[X]][start[Y]];
  if(mover == NULL) return false;
  if((brd[end[X]][end[Y]] != NULL) && (brd[end[X]][end[Y]]->color == mover->color)) return false;// TODO: does this apply ?
  printPiece(mover);
  if(mover->type != KING && isKingChecked(brd, mover->color)) return false;
  if(isKingCheckedAfterMove(brd, mover->color, start, end)) return false;
  switch(mover->type) {
  case BISHOP:
    return bishop(brd, start, end);
  case CASTLE:
    return castle(brd, start, end);
  case KING:
    return king(brd, start, end);
  case KNIGHT:
    return knight(brd, start, end);
  case PAWN:
    return pawn(brd, start, end);
  case QUEEN:
     return queen(brd, start, end);
  default:
    return false;
  };
}

bool isWithinBoard(int position[2]) {
  return !(position[X] >= SIZE || position[X] < 0 || position[Y] >= SIZE || position[Y] < 0);
}

bool bishop(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  //  bishop must always move equally in x and y
  if(abs(move[X]) != abs(move[Y])) return false;
  // check that its not blocked
  for(
      int i = start[X] + sign(move[X]), j = start[Y] + sign(move[Y]);
      i != end[X];
      i += sign(move[X]), j += sign(move[Y])) {
    if(brd[i][j] != NULL) return false;
  }
  return true;
}

bool castle(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  //  must move in only one direction
  if(abs(move[X]) && abs(move[Y])) return false;

  dim dim = abs(move[X]) ? X : Y;
  int dir = sign(move[dim]);
  for(
      int i = start[dim] + dir;
      i != end[dim];
      i += dir) {
    if(brd[dim == X ? i : start[X]][dim == Y ? i : start[Y]] != NULL) return false;
  }
  //TODO castling
  return true;
}

bool king(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  if(abs(move[X]) > 1 || abs(move[Y]) > 1) return false;
  return true;
}

bool knight(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  if(!((abs(move[X]) == 1 && abs(move[Y]) == 2) || (abs(move[X]) == 2 && abs(move[Y]) == 1))) return false;
  return true;
}

bool pawn(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  piece * mover = brd[start[X]][start[Y]];
  if(abs(move[X]) > 1) return false;
  if(abs(move[Y]) > 2) return false;
  //if double move check that piece never moved and it is not blocked
  if(abs(move[Y]) == 2 && mover->hasMoved && brd[start[X]][start[Y] + sign(move[Y])] != NULL) return false;
  //if it move horizontally it must take a piece or en passant
  if(abs(move[X]) == 1 && ((brd[end[X]][end[Y]] == NULL) || isEnPassant(brd, start, end))) return false;
  //black starts at the top and moves down
  if(mover->color == BLACK && sign(move[Y]) == -1) return false;
  if(mover->color == WHITE && sign(move[Y]) == 1) return false;
  //TODO: check for block
  return true;
}

bool isEnPassant(board brd, int start[2], int end[2]) {
  //TODO
  return false;
}

bool queen(board brd, int start[2], int end[2]) {
  return bishop(brd, start, end) || castle(brd, start, end);// TODO: exlc castling
}

// is a $color piece safe on this spot?
bool isChecked(board brd, int spot[2], color color) {
  for(int i = 0; i < SIZE; ++i) {
    for(int j = 0; j < SIZE; ++j) {
      if(brd[i][j] != NULL && brd[i][j]->color != color) {
        int attackerPosition[2] = { i, j };
        if(isValidMove(brd, attackerPosition, spot)) {
          return true;
        }
      }
    }
  }
  return false;
}

// is $color king checked by any ~$color piece
bool isKingChecked(board brd, color color) {
  for(int i = 0; i < SIZE; ++i) {
    for(int j = 0; j < SIZE; ++j) {
      if(brd[i][j] != NULL && brd[i][j]->color == color && brd[i][j] -> type == KING) {
        int kingPosition[2] = { i, j };
        return isChecked(brd, kingPosition, color);
      }
    }
  }
  return false;
}

bool isKingCheckedAfterMove(board brd, color color, int start[2], int end[2]) {
  board brdAfterMove = cloneBoard(brd);
  brdAfterMove[end[X]][end[Y]] = clonePiece(brd[start[X]][start[Y]]);
  free(brdAfterMove[start[X]][start[Y]]);
  brdAfterMove[start[X]][start[Y]] = NULL;
  return isKingChecked(brdAfterMove, color);
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

void move_unitTests() {
  sput_start_testing();
  sput_set_output_stream(NULL);

  sput_enter_suite("move.common");
  sput_run_test(common_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.castle");
  sput_run_test(castle_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.pawn");
  sput_run_test(pawn_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.knight");
  sput_run_test(knight_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.bishob");
  sput_run_test(bishop_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.king");
  sput_run_test(king_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.isChecked");
  sput_run_test(isChecked_unitTest);
  sput_leave_suite();

  sput_finish_testing();
}

void common_unitTest() {
  board brd = createBoard();
  int negtve[2] = { -1, -1 };
  int zeroMinusOne[2] = { 0, -1 };

  int middle[2] = { 4, 4 };
  int outOfBounds[2] = { 3, 9 };
  int sevenSeven[2] = {7 , 7};

  sput_fail_unless(!isValidMove(brd, outOfBounds, middle), "start out of bounds.");
  sput_fail_unless(!isValidMove(brd, negtve, middle), "start -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, zeroMinusOne, middle), "start -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, sevenSeven), "no piece at start move.");
  sput_fail_unless(!isValidMove(brd, middle, middle), "start cannot equal end.");

  brd[middle[X]][middle[Y]] = createPiece(BLACK, BISHOP);

  sput_fail_unless(!isValidMove(brd, middle, zeroMinusOne), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, negtve), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, outOfBounds), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, middle), "no move.");

  sput_fail_unless(isValidMove(brd, middle, sevenSeven), "diagonal move should pass.");

  int whtKing[2] = { 7, 7 };//is checked by middle bishop
  brd[whtKing[X]][whtKing[Y]] = createPiece(WHITE, KING);

  int whtCastle[2] = { 0, 0 };
  int whtCastleTo[2] = { 7, 0 };//
  brd[0][0] = createPiece(WHITE, CASTLE);
  sput_fail_unless(!isValidMove(brd, whtCastle, whtCastleTo), "should fail bcs white king is threatened.");
  freeBoard(brd);
}

void pawn_unitTest() {
  int blkStart[2] = { 1, 1 };
  int blkEndSingle[2] = { 1, 2 };
  int blkEndDouble[2] = { 1, 3 };
  int blkEndTake[2] = { 2, 2 };
  int blkEndTake2[2] = { 0, 2 };
  board brd = createBoard();
  brd[blkStart[X]][blkStart[Y]] = createPiece(BLACK, PAWN);
  brd[blkEndTake[X]][blkEndTake[Y]] = createPiece(WHITE, PAWN);
  sput_fail_unless(isValidMove(brd, blkStart, blkEndSingle), "1 postion move");
  sput_fail_unless(isValidMove(brd, blkStart, blkEndDouble), "2 postion move");
  sput_fail_unless(isValidMove(brd, blkStart, blkEndTake), "valid take");
  sput_fail_unless(!isValidMove(brd, blkStart, blkEndTake2), "valid take, no piece there");

  int whtStart[2] = { 1, 6 };
  int whtEndSingle[2] = { 1, 5 };
  int whtEndDouble[2] = { 1, 4 };
  int whtEndTake[2] = { 2, 5 };
  int whtEndTake2[2] = { 0, 5 };
  brd[whtStart[X]][whtStart[Y]] = createPiece(WHITE, PAWN);
  brd[whtEndTake[X]][whtEndTake[Y]] = createPiece(BLACK, PAWN);
  sput_fail_unless(isValidMove(brd, whtStart, whtEndSingle), "1 postion move");
  sput_fail_unless(isValidMove(brd, whtStart, whtEndDouble), "2 postion move");
  sput_fail_unless(isValidMove(brd, whtStart, whtEndTake), "valid take");
  sput_fail_unless(!isValidMove(brd, whtStart, whtEndTake2), "valid take, no piece there");

  freeBoard(brd);
}

void castle_unitTest() {
  board brd = createBoard();
  int threeThree[2] = { 3, 3 };
  brd[threeThree[X]][threeThree[Y]] = createPiece(WHITE, CASTLE);
  int threeSix[2] = { 3, 6 };
  sput_fail_unless(isValidMove(brd, threeThree, threeSix), "valid +y move.");
  int sixThree[2] = { 6, 3 };
  sput_fail_unless(isValidMove(brd, threeThree, sixThree), "valid +x move.");
  int oneThree[2] = { 1, 3 };
  sput_fail_unless(isValidMove(brd, threeThree, oneThree), "valid -x move.");
  int threeOne[2] = { 3, 1 };
  sput_fail_unless(isValidMove(brd, threeThree, threeOne), "valid -y move.");

  int fourFour[2] = { 4, 4 };
  int twoTwo[2] = { 2, 2 };
  int fourTwo[2] = { 4, 2 };
  int twoFour[2] = { 2, 4 };
  sput_fail_unless(!isValidMove(brd, threeThree, twoTwo), "invalid move.");
  sput_fail_unless(!isValidMove(brd, threeThree, fourTwo), "invalid move.");
  sput_fail_unless(!isValidMove(brd, threeThree, twoFour), "invalid move.");
  sput_fail_unless(!isValidMove(brd, threeThree, fourFour), "invalid move.");
}

void knight_unitTest() {
  board brd = createBoard();

  int start[2] = { 3, 3 };
  brd[start[X]][start[Y]] = createPiece(BLACK, KNIGHT);

  int pstve[2] = { 4, 5 };
  sput_fail_unless(isValidMove(brd, start, pstve), "valid move");

  int ngtve[2] = {1 , 2};
  sput_fail_unless(isValidMove(brd, start, ngtve), "valid move.");

  int noY[2] = {1 , 3};
  sput_fail_unless(!isValidMove(brd, start, noY), "O y component");

  int noX[2] = {3 , 5};
  sput_fail_unless(!isValidMove(brd, start, noX), "O x component");

  freeBoard(brd);
}

void bishop_unitTest() {
  board brd = createBoard();

  int start[2] = { 3, 3 };
  brd[start[X]][start[Y]] = createPiece(BLACK, BISHOP);

  int nonDiagonalEnd[2] = { 4, 9 };
  sput_fail_unless(!isValidMove(brd, start, nonDiagonalEnd), "non diagonal move should fail.");

  int pstveDiagonalEnd[2] = {7 , 7};
  sput_fail_unless(isValidMove(brd, start, pstveDiagonalEnd), "diagonal move should pass.");

  int negtveDiagonalEnd[2] = {1 , 1};
  sput_fail_unless(isValidMove(brd, start, negtveDiagonalEnd), "ntve diagonal move should pass.");

  freeBoard(brd);
}

void isChecked_unitTest() {
  board brd = createBoard();

  int bishop[2] = { 3, 3 };
  brd[bishop[X]][bishop[Y]] = createPiece(BLACK, BISHOP);
  int checkedByBishop[2] = {0, 0};
  sput_fail_unless(isChecked(brd, checkedByBishop, WHITE), "is checked by bishop.");
  sput_fail_unless(!isChecked(brd, checkedByBishop, BLACK), "checked by bishop but same color.");
  int alsoCheckedByBishop[2] = {7 , 7};
  sput_fail_unless(isChecked(brd, alsoCheckedByBishop, WHITE), "also checked by bishop.");
  int notChecked[2] = {2, 9};
  sput_fail_unless(!isChecked(brd, notChecked, WHITE), "not checked.");
  freeBoard(brd);
}

void king_unitTest() {
  board brd = createBoard();

  int whtKing[2] = { 3, 3 };
  brd[whtKing[X]][whtKing[Y]] = createPiece(WHITE, KING);
  int end[2] = { 2, 2 };
  sput_fail_unless(isValidMove(brd, whtKing, end), "valid move no check .");
  brd[3][1] = createPiece(BLACK, CASTLE);//checks whtKing spot
  sput_fail_unless(isValidMove(brd, whtKing, end), "starting spot is checked, valid move.");
  brd[2][4] = createPiece(BLACK, CASTLE); //checks end spot
  sput_fail_unless(isChecked(brd, end, WHITE), "castle should check end.");
  sput_fail_unless(!isValidMove(brd, whtKing, end), "valid move but is checked by castle on 2,4.");
  freeBoard(brd);
}
