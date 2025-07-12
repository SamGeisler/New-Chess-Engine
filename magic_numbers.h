#pragma once

#include <cstdint>

namespace MagicNumbers{
    uint64_t genDestRook(int src, uint64_t pieces);
    uint64_t genDestBishop(int src, uint64_t pieces);

    extern uint64_t rookNums[64];
    extern uint64_t bishopNums[64];
    extern int rookWidths[64];
    extern int bishopWidths[64];
}