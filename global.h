#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>
#include<inttypes.h>

//Display
#define SWIDTH 1200
#define SHEIGHT 800
#define FULLSCREEN 0
#define BGCOLOR 0x222222
#define LIGHTSPACECOLOR 0xEEEEEE
#define DARKSPACECOLOR 0x4444444
#define PS 90
#define XMARG ((SWIDTH - PS*8)/2)
#define YMARG ((SHEIGHT - PS*8)/2)

//Engine/game settings
#define AICOLOR 1
#define MAXDEPTH 6

//Misc. utility
#define SHIFT(x) (((uint64_t)1)<<x)

double PIECE_VALUES[5];

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

//Only used in board_arr and for rendering
enum board_array_values{
    EMPTY,
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,
    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING
};

unsigned int board_arr[64];//Only used for rendering and controls - not used at all by get_ai_move. Updates to match bitboards after AI makes its move
//White pieces are pawn=1, knight=2, bishop=3,rook=4,queen=5,king=6, +8 for black pieces

struct board_t{
    uint64_t bitboards[8];//Same order as enum: White pieces, black pieces, all pawns, all knights, all bishops, all rooks, all queens, all kings
} board;

typedef struct{
    char castle_flags;//Bottom 4 bits - white ks, white qs, black ks, black qs (1 indicates available)
    char ep_right;//Location of valid space to perform en passant from, 0 if none
    char fmr_count;//Fifty move rule count
} metadata_t;//All gamestate info other than board position

typedef struct{
    char src;
    char dest;
    char promo; //0-knight, 1-bishop, 2-rook, 3-queen
} move;

metadata_t MD;

#endif