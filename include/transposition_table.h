#pragma once

#include <cstdint>
#include <vector>
#include <array>

class TTable {
public:
    enum NodeType {
        PV_NODE,
        ALL_NODE,
        CUT_NODE,
    };
    struct Entry {
        int depth;
        uint64_t key;
        NodeType nodeType;
        double score;//Different interpretation depending on node type:
        /*
            Type 1/PV-node (All moves were searched, improvement found) - Exact score
            Type 2/Cut-node (One move was too good, opponent will never take this route) - Lower bound
            Type 3/All-node (All moves were searched, no improvement found) - Upper bound
        */
    };

private:
    int capacity, size;
    std::vector<Entry*> table;

public:
    TTable(int sizeMB)
        : capacity ( sizeMB * 1024 * 1024 / sizeof(Entry*) ), table (capacity, nullptr), size {0}
    {}

    Entry* getEntry(uint64_t key, int depth);
    bool makeEntry(uint64_t key, int depth, NodeType nodeType, double score);

    ~TTable(){
        for(Entry* e: table){
            delete e;
        }
    }
};

namespace ZobristInitKeys{
    extern std::array<std::array<uint64_t, 16>, 64> pieceAtSquare;//Addressed with Game::Piece enum
    extern uint64_t blackToMove;
    extern std::array<uint64_t, 16> castlingRights;
    extern std::array<uint64_t, 48> epRights;
    void init();
}