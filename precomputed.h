#pragma once

#include <cstdint>
#include <vector>

namespace Precomputed{
    extern uint64_t inbetweenBB[64][64];

    extern std::vector<std::vector<uint64_t>> rookDestIntersected;
    extern std::vector<std::vector<uint64_t>> bishopDestIntersected;

    bool loadPrecompute();
}