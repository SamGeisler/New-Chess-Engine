#pragma once

#include "game.h"

#include <string>
#include <string_view>


class MoveGenerationTest{
private:
    int perft(int depth, int endingDepth);

public:
    void runTestSuite();
    void perftDebug(std::string_view fen, int depth);

    Game game;

};