#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "bb_utils.h"

//Display
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

//Engine/game settings
extern int AICOLOR;
#define MAXDEPTH 6

//Misc. utility
#define SHIFT(x) (((uint64_t)1)<<(x))
#define NOT_A_FILE 0xFEFEFEFEFEFEFEFE
#define NOT_H_FILE 0x7F7F7F7F7F7F7F7F
#define NOT_1_RANK 0x00FFFFFFFFFFFFFF
#define NOT_8_RANK 0xFFFFFFFFFFFFFF00
#define RANK_3 0x0000FF0000000000
#define RANK_6 0x0000000000FF0000
#define FULL_BOARD 0xFFFFFFFFFFFFFFF
#define WHITE_KS_CASTLE_MASK 0x7000000000000000
#define WHITE_QS_CASTLE_MASK 0x1A00000000000000
#define BLACK_KS_CASTLE_MASK 0x0000000000000070
#define BLACK_QS_CASTLE_MASK 0x000000000000001A

extern double PIECE_VALUES[5];

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
extern uint64_t rookDestTrunc[64];//Excludes last dest squares in row/col
extern uint64_t bishopDestTrunc[64];//Excludes last dest squares in diagonal

//Sliding piece magic numbers for each source square
extern uint64_t rookMN[64];
extern uint64_t bishopMN[64];

//Compression width of corresponding magic numbers
extern int rookMN_w[64];
extern int bishopMN_w[64];

//InBetweenLookup (bitboard of squares between two given squares, exclusive)
extern uint64_t inBetween[64][64];

//Sliding piece destination BB for each source square and piece intersection arrangements allocated & loaded at runtime
//Indexed by magic number multiplication
extern uint64_t* rookDestInt[64];
extern uint64_t* bishopDestInt[64];

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

//Used in board_arr and for rendering 
//Equal to the above enum, +8 for black pieces
enum board_array_values{
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



typedef struct {
    uint64_t bitboards[8];//Same order as enum: White pieces, black pieces, all pawns, all knights, all bishops, all rooks, all queens, all kings
} board_t;
typedef struct{
    char castle_flags;//[3] - white ks, [2] - white qs, [1] - black ks, [0] - black qs (1 indicates available)
    char ep_right;//Location of valid space to perform en passant to, 0 if none
    char to_move;
    int fmr_count;//Half move rule count
} metadata_t;//All gamestate info other than board position

extern board_t board;
extern metadata_t MD;
extern unsigned int board_arr[64];//Maintained along side bitboards, and used during move generation.

typedef struct{
    char src;
    char dest;
    char promo; //0-no promotion, 1-knight, 2-bishop, 3-rook, 4-queen
} move;


#endif