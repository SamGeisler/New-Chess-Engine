#pragma once

#include <cstdint>
#include <array>
#include <stack>

#include "move.h"

class Game{
private:
    uint64_t genPawn(int src, int color);
    uint64_t genPawnWhite(uint64_t pawnPos);
    uint64_t genPawnBlack(uint64_t pawnPos);
    uint64_t genKnight(int src, int color);
    uint64_t genBishop(int src, int color);
    uint64_t genRook(int src, int color);
    uint64_t genQueen(int src, int color);
    uint64_t genKing(int src, int color);
    uint64_t (Game::*getPieceDest[6]) (int, int) = {&Game::genPawn, &Game::genKnight, &Game::genBishop, &Game::genRook, &Game::genQueen, &Game::genKing};
    
    static uint64_t genXrayRook(uint64_t pieces, uint64_t blockers, int src);
    static uint64_t genXrayBishop(uint64_t pieces, uint64_t blockers, int src);



public:
    Game(int _playerColor, std::string_view _initBoard);

    enum BBIndex : int{
        WHITE,
        BLACK,
        PAWN,
        KNIGHT,
        BISHOP,
        ROOK,
        QUEEN,
        KING
    };

    enum Piece : int{
        EMPTY,
        WHITE_PAWN = 2,
        WHITE_KNIGHT,
        WHITE_BISHOP,
        WHITE_ROOK,
        WHITE_QUEEN,
        WHITE_KING = 7,
        BLACK_PAWN = 10,
        BLACK_KNIGHT,
        BLACK_BISHOP,
        BLACK_ROOK,
        BLACK_QUEEN,
        BLACK_KING
    };

    enum EndFlag : int{
        WHITE_WINS = -1,
        DRAW,
        BLACK_WINS,
        NO_END,
    };

    struct Metadata {
        int castleFlags {0xF};
        int epRight {0};
        int toMove {0};
        int fmrCount {0};
    };

    uint64_t bitboards[8];
    Piece boardArr[64];

    Metadata metadata;

    std::stack<Metadata> MD_stack;

    void printBoardArr();
    void printMetadata();
    void printBB(BBIndex bb);
    void printBB(uint64_t bb);

    int generateMoves(std::array<Move, 220> &moveArr);//Returns number of moves
    int generateMovesCheck(std::array<Move, 220> &moveArr);

    bool isInCheck(int color);
    uint64_t getAttacking(int color);
    uint64_t getPinned(int kingPos, int color);

    void executeMove(const Move& m);
    void unexecuteMove(const Move& m, Piece destSquare);

    void initBoard(std::string_view initFEN);
    
    EndFlag gameEnd(int color, int isInCheck, int numMoves);

    int playerColor;
};