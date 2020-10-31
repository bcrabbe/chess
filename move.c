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

int sign(int x);
bool isWithinBoard(int position[2]);

//unit test functions
void move_unitTests();
void pawn_unitTest();
void bishop_unitTest();

bool isValidMove(board brd, int start[2], int end[2]) {
  if(!isWithinBoard(start) || !isWithinBoard(end) || (start[X] == end[X] && start[Y] == end[Y])) return false;
  piece * mover = brd[start[X]][start[Y]];
  if(mover == NULL) return false;
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
  return false;
}

bool queen(board brd, int start[2], int end[2]) {
  return bishop(brd, start, end) || castle(brd, start, end);// TODO: exlc castling
}

int sign(int x) {
    return (x > 0) - (x < 0);
}

void move_unitTests() {
  pawn_unitTest();
  bishop_unitTest();
}

void pawn_unitTest() {
  int start[2] = { 0, 1 };
  int end[2] = { 0, 3 };
  board brd = createBoard();
  iprint(isValidMove(brd, start, end));
  freeBoard(brd);
}

void bishop_unitTest() {
  int start[2] = { 0, 1 };
  int end[2] = { 0, 3 };
  board brd = createBoard();
  iprint(isValidMove(brd, start, end));
  freeBoard(brd);
}
