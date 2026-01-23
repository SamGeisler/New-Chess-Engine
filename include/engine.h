#pragma once

#include "move.h"
#include "game.h"
#include "transposition_table.h"

class Engine{
private:
    Game& gameRef;
    TTable ttable;

    friend void engineBenchmark();

    Move bestMove;

    static constexpr double PIECE_VALUES[7] = {0.0, 0.0, 1.0, 3.0, 3.2, 5.0, 9.0};//Aligned with Game::BBIndex enum

    double staticEval(int color);
    double minimaxEval(int depth, int color, double alpha, double beta);
    double quiescenceSearch(int color, double alpha, double beta);

    int maxSearchDepth {5};
public:
    Engine(Game& _gameRef)
        : gameRef {_gameRef}, ttable {128}
    {}

    Move getMove();
};