#pragma once
#include <iostream>

struct Move{
    enum Promo : int{
        NONE,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
    };
    int src;
    int dest;
    Promo promo;
};

std::ostream& operator<<(std::ostream& stream, const Move& move);