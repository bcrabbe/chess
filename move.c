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

//unit test functions
void move_unitTests();
void common_unitTest();
void pawn_unitTest();
void bishop_unitTest();

bool isValidMove(board brd, int start[2], int end[2]) {
  if(!isWithinBoard(start) || !isWithinBoard(end) || (start[X] == end[X] && start[Y] == end[Y])) return false;
  piece * mover = brd[start[X]][start[Y]];
  if(mover == NULL) return false;
  if ((brd[end[X]][end[Y]] != NULL) && (brd[end[X]][end[Y]]->color == mover->color)) return false;// TODO: does this apply ?
  printPiece(mover);
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
  return !(position[X] >= SIZE || position[X] < 0);
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
  int dir = sign(move[X]) || sign(move[Y]);
  for(
      int i = start[dim] + sign(dir);
      i != end[dim];
      i += sign(dir)) {
    if(brd[dim == X ? i : start[X]][dim == Y ? i : start[Y]] != NULL) return false;
  }
  //TODO castling
  return true;
}

bool king(board brd, int start[2], int end[2]) {
  return false;
}

bool knight(board brd, int start[2], int end[2]) {
  return false;
}

bool pawn(board brd, int start[2], int end[2]) {
  int move[2] = { end[X] - start[X], end[Y] - start[Y] };
  piece * mover = brd[start[X]][start[Y]];
  if(abs(move[X]) > 1) return false;
  if(abs(move[Y]) > 2) return false;
  if(abs(move[Y]) == 2 && mover->hasMoved) return false;
  //if it move horizontally it must take a piece or en passent
  if(abs(move[X]) == 1 && ((brd[end[X]][end[Y]] == NULL) || isEnPassant(brd, start, end)))
  //black starts at the top and moves down
  if(mover->color == BLACK && sign(move[Y]) == -1) return false;
  if(mover->color == WHITE && sign(move[Y]) == 1) return false;
  return true;
}

bool isEnPassant(board brd, int start[2], int end[2]) {
  return false;
}

bool queen(board brd, int start[2], int end[2]) {
  return bishop(brd, start, end) || castle(brd, start, end);// TODO: exlc castling
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


  sput_enter_suite("move.pawn");
  sput_run_test(pawn_unitTest);
  sput_leave_suite();

  sput_enter_suite("move.bishob");
  sput_run_test(bishop_unitTest);
  sput_leave_suite();

  sput_finish_testing();
  bishop_unitTest();
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
  sput_fail_unless(!isValidMove(brd, middle, sevenSeven), "no piece at start move");

  brd[middle[X]][middle[Y]] = createPiece(BLACK, BISHOP);

  sput_fail_unless(!isValidMove(brd, middle, zeroMinusOne), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, negtve), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, outOfBounds), "end -ve out of bounds.");
  sput_fail_unless(!isValidMove(brd, middle, middle), "no move.");

  sput_fail_unless(isValidMove(brd, middle, sevenSeven), "diagonal move should pass.");

  //  freePieces(brd);
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
  sput_fail_unless(isValidMove(brd, blkStart, blkEndTake2), "valid take");

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
