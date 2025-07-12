#ifndef GENERATE_MOVES_H
#define GENERATE_MOVES_H

#include <array>

int generateMoves(std::array<Move, 220> &moveArr, int color);//Returns number of moves
int generateMovesCheck(std::array<Move, 220>& moveArr, int color);

int isInCheck(int color);
uint64_t getAttackingBitboard(int color);
uint64_t getPinned(int kingPos, int color);//Returns pinned squares of passed color

#endif