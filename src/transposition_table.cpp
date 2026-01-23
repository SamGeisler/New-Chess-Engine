#include "transposition_table.h"

#include <iostream>
#include <cstdlib>

uint64_t rand64(){
    uint64_t result {0};
    for(int i = 0; i<64; i++){
        result |= std::rand()%2;
        result <<= 1;
    }

    return result;
}

namespace ZobristInitKeys{
    std::array<std::array<uint64_t, 16>, 64> pieceAtSquare;//Addressed with Game::Piece enum
    uint64_t blackToMove;
    std::array<uint64_t, 16> castlingRights;
    std::array<uint64_t, 48> epRights;
    void init(){
        srand(0xDEADBEEF);
        for(auto& x : ZobristInitKeys::pieceAtSquare){
            for(auto& y : x){
                y = rand64();

            }
        }
        for(auto& x : ZobristInitKeys::epRights){
            x = rand64();
        }
        for(auto& x: ZobristInitKeys::castlingRights){
            x = rand64();
        }
        ZobristInitKeys::blackToMove = rand64();
    }
}

TTable::Entry* TTable::getEntry(uint64_t key, int depth){
    TTable::Entry* entry = table[key % capacity];

    if(entry != nullptr && entry->key == key && entry->depth >= depth){
        return entry;
    }

    return nullptr;
}

bool TTable::makeEntry(uint64_t key, int depth, NodeType nodeType, double score){
    TTable::Entry*& entry = table[key % capacity];

    //Simple replacement heuristic: Only update if new evaluation was performed to a higher depth
    if(entry != nullptr && entry->depth >= depth){
        std::cout << "Entry collision, not replacing.\n";
        return false;
    }

    entry = new Entry{depth, key, nodeType, score};
    size++;
    std::cout << "New entry! - Type " << static_cast<int>(nodeType) << ".\n";
    return true;
}