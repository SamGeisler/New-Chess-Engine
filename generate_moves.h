#ifndef GENERATE_MOVES_H
#define GENERATE_MOVES_H

int generate_moves(move* move_arr, int color);//Returns number of moves
int generate_moves_check(move* move_arr, int color);

int is_in_check(int color);
uint64_t get_attacking_bitboard(int color);
uint64_t get_pinned(int king_pos, int color);//Returns pinned squares of passed color

#endif