/*
main

boardUtils
control
    executeMove, unexecuteMove, gameEnd -> Game class
    getEngineMove -> Engine class
generateMoves
    generateMoves (many sub-functions) -> Game class
        (Only the main function declared as a member.)
loadPrecompute
    loadPrecompute (many sub-functions) -> Bitboard lookup namespace
perft
    Everything -> perft class
playerInput
    everything -> interface class
render
    everything -> interface class
*/


#ifndef GLOBAL_H
#define GLOBAL_H

#include <inttypes.h>

#include <iostream>
#include <iomanip>
#include <string>
#include <stringView>

#include "boardUtils.h"

namespace RenderConst{
    constexpr int SWIDTH {1200};
    constexpr int SHEIGHT {800};
    constexpr bool FULLSCREEN {false};
    constexpr uint32T BGCOLOR {0x222222};
    constexpr uint32T LIGHT_SPACE_COLOR {0xEEEEEE};
    constexpr uint32T DARK_SPACE_COLOR {0x444444};
    constexpr uint32T LIGHT_ATTACKING {0xEE5555};
    constexpr uint32T DARK_ATTACKING {0xAA2222};
    constexpr int SQUARE_WIDTH {90};
    constexpr int XMARG() { return (SWIDTH - SQUARE_WIDTH*8)/2; }
    constexpr int YMARG() { return (SHEIGHT - SQUARE_WIDTH*8)/2; }
};

namespace Color{
    enum Color{
        WHITE,
        BLACK
    };
};

//Display
/*
#define SWIDTH 1200
#define SHEIGHT 800
#define FULLSCREEN 0
#define BGCOLOR 0x222222
#define LIGHTSPACECOLOR 0xEEEEEE
#define DARKSPACECOLOR 0x4444444
#define LIGHTATTACKING 0xEE5555
#define DARKATTACKING 0xAA2222
#define PS 90
#define XMARG ((SWIDTH - PS*8)/2)
#define YMARG ((SHEIGHT - PS*8)/2)
*/

//Engine/game settings
extern int AICOLOR;
#define MAXDEPTH 6

/*
//Misc. utility
#define SHIFT(x) (((uint64_t)1)<<(x))
#define NOT_A_FILE 0xFEFEFEFEFEFEFEFE
#define NOT_H_FILE 0x7F7F7F7F7F7F7F7F
#define NOT_1_RANK 0x00FFFFFFFFFFFFFF
#define NOT_8_RANK 0xFFFFFFFFFFFFFF00
#define FULL_BOARD 0xFFFFFFFFFFFFFFF
#define WHITE_KS_CASTLE_MASK 0x7000000000000000
#define WHITE_QS_CASTLE_MASK 0x1C00000000000000
#define BLACK_KS_CASTLE_MASK 0x0000000000000070
#define BLACK_QS_CASTLE_MASK 0x000000000000001C
*/

//extern double PIECE_VALUES[5];

/*
//Bitboards for ranks, files, and diagonals
extern uint64_t RANKS[8];
extern uint64_t FILES[8];
extern uint64_t DIAGONALS_NE[15];
extern uint64_t DIAGONALS_NW[15];
extern int diagLU_NE[64];
extern int diagLU_NW[64];

//Bitboards of possible destinations (assuming empty board) for each source square
extern uint64_t knightDest[64];
extern uint64_t kingDest[64];
extern uint64_t rookDest[64];
extern uint64_t bishopDest[64];
extern uint64_t rookDestTrunc[64];//Excludes last dest squares in row/col
extern uint64_t bishopDestTrunc[64];//Excludes last dest squares in diagonal
*/

//Sliding piece magic numbers for each source square
extern uint64_t rookMN[64];
extern uint64_t bishopMN[64];

//Compression width of corresponding magic numbers
extern int rookMNW[64];
extern int bishopMNW[64];

//InBetweenLookup (bitboard of squares between two given squares, exclusive)
extern uint64_t inBetween[64][64];

//Sliding piece destination BB for each source square and piece intersection arrangements allocated & loaded at runtime
//Indexed by magic number multiplication
extern uint64_t* rookDestInt[64];
extern uint64_t* bishopDestInt[64];



/*
//Used throughout
enum colors{
    WHITE,
    BLACK
};

//Mainly used for bitboards (first and second bbs in board.bitboards are white and black pieces respectively)
enum pieces{
    UNUSED1,
    UNUSED2,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

//Used in boardArr and for rendering 
//Equal to the above enum, +8 for black pieces
enum boardArrayValues{
    EMPTY,
    NULL_PIECE1,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    NULL_PIECE2,
    NULL_PIECE3,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

*/
/*
struct Board{
    uint64_t bitboards[8];//Same order as enum: White pieces, black pieces, all pawns, all knights, all bishops, all rooks, all queens, all kings
};

struct Metadata{
    char castleFlags;//[3] - white ks, [2] - white qs, [1] - black ks, [0] - black qs (1 indicates available)
    char epRight;//Location of valid space to perform en passant to, 0 if none
    char toMove;
    int fmrCount;//Half move rule count
};//All gamestate info other than board position

extern Board board;
extern Metadata MD;
extern unsigned int boardArr[64];//Maintained along side bitboards, and used during move generation.

*/





#endif