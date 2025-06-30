#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

extern const int index64[64];

int bitScanForward(uint64_t bb);
int count_bits(uint64_t bb);

void print_bb(uint64_t bb);

void init_board(char* board_init);
void update_board_arr();
void update_bb();
void print_board_arr();
void print_metadata();

void notation(int pos, char* out);


#endif