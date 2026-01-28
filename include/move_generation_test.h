#pragma once

#include "game.h"

#include <string>
#include <string_view>


class MoveGenerationTest{
private:
    int perft(int depth, int endingDepth);

public:
    MoveGenerationTest();
    MoveGenerationTest(int _playerColor, std::string_view _initBoard);
    void runTestSuite();
    void perftDebug(std::string_view fen, int depth);

    Game game;

};