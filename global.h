#ifndef GLOBAL_H
#define GLOBAL_H


#define SWIDTH 1200
#define SHEIGHT 800
#define FULLSCREEN 0
#define BGCOLOR 0x222222
#define LIGHTSPACECOLOR 0xEEEEEE
#define DARKSPACECOLOR 0x4444444
#define PS 90

#define XMARG ((SWIDTH - PS*8)/2)
#define YMARG ((SHEIGHT - PS*8)/2)

#define AICOLOR 1
#define MAXDEPTH 6

double PIECE_VALUES[5];

typedef struct{

} board_t;//Location of pieces(bitboards)
typedef struct{

} metadata_t;//All gamestate info other than board position

struct game_state{  //Complete game state info
    board_t* board;
    metadata_t* metadata;
};



#endif