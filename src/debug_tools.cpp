#include "lookup.h"

#include <iostream>
#include <string>

void DebugTools::printBB(uint64_t bb){
    std::cout << "Bitboard: 0x" << std::hex << bb << std::dec << ":\n";
    for(int i = 0; i<64; i++){
        std::cout << ((SHIFT(i) & bb) ? 'X' : '.') << " ";
        if(i%8==7) std::cout << "\n";
    }
}

std::string DebugTools::notation(int pos){
    std::string output{"00"};
    output[0] = 'a' + pos%8;
    output[1] = '1' + (63-pos)/8;

    return output;
}