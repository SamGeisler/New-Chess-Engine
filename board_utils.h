#ifndef BOARD_UTILS_H
#define BOARD_UTILS_H

#include <stringView>
#include <string>

extern const int index64[64];

int bitScanForward(uint64_t bb);
int countBits(uint64_t bb);

void printBb(uint64_t bb);

void initBoard(std::stringView boardInit);
void updateBoardArr();
void updateBb();
void printBoardArr();
void printMetadata();

std::string notation(int pos);


#endif