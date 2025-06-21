#include "global.h"
#include "bb_utils.h"
#include "generate_moves.h"
#include "control.h"

//All of the following return a bitboard of possible destinations (disregarding same-color collisions), which is converted into move structs in generate_moves
void gen_pawn_white(uint64_t bb, move* move_arr);
void gen_pawn_black(uint64_t bb, move* move_arr);
void gen_ep_white(move* move_arr);
void gen_ep_black(move* move_arr);

void (*gen_pawn[2]) (uint64_t, move*) = {&gen_pawn_white,&gen_pawn_black};
void (*gen_enpassant[2]) (move*) = {&gen_ep_white, &gen_ep_black};

//Get bitboard of possible destinations from bitboard of pieces to move
uint64_t gen_knight(int src, int color);
uint64_t gen_bishop(int src, int color);
uint64_t gen_rook(int src, int color);
uint64_t gen_queen(int src, int color);
uint64_t gen_king(int src, int color);

uint64_t (*get_piece_dest_bb[5]) (int, int) = {&gen_knight, &gen_bishop, &gen_rook, &gen_queen, &gen_king};

void append_moves(int src, uint64_t dest_bb, move* move_arr);


static int m_i;//Move array index

int generate_moves(move* move_arr, int color){
    m_i = 0;
    //gen_pawn[color](board.bitboards[color] & board.bitboards[PAWN], move_arr);
    gen_enpassant[color](move_arr);

    for(int piece = KNIGHT; piece<=KING; piece++){
        uint64_t src_bb = board.bitboards[color] & board.bitboards[piece];
        while(src_bb){
            int src_bsf = bitScanForward(src_bb);//Index of leftmost piece
            src_bb &= ~SHIFT(src_bsf);

            //append_moves(src_bsf, get_piece_dest_bb[piece-3](src_bsf, color), move_arr);
        }
    }

    return m_i;
}

uint64_t gen_knight(int src, int color){
    return knightDest[src] & ~board.bitboards[color];
}

uint64_t gen_king(int src, int color){
    return kingDest[src] & ~board.bitboards[color];
}

uint64_t gen_rook(int src, int color){
    uint64_t int_bb = rookDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);
    return rookDestInt[src][index] & ~board.bitboards[color];
}

uint64_t gen_bishop(int src, int color){
    uint64_t int_bb = bishopDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);
    return bishopDestInt[src][index] & ~board.bitboards[color];
}

uint64_t gen_queen(int src, int color){
    uint64_t int_bb = rookDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);
    uint64_t dest_bb = rookDestInt[src][index];

    int_bb = bishopDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);
    dest_bb |= bishopDestInt[src][index];

    dest_bb &= ~board.bitboards[color];

    return dest_bb;
}

void append_moves(int src, uint64_t dest_bb, move* move_arr){
    int dest_bsf;
    while(dest_bb){
        int dest_bsf = bitScanForward(dest_bb);
        dest_bb &= ~SHIFT(dest_bsf);

        move_arr[m_i++] = (move){src, dest_bsf, 0};
    }
}

void gen_pawn_white(uint64_t bb, move* move_arr){
    uint64_t TDB;//Temporary destination bitboard
    uint64_t TPB;//Temporary promotion bitboard
    uint64_t DPP;//Double pawn push temporary bitboard
    uint64_t bsf;//Temporary bitscan forward result

    //Left attacks
    TDB = (bb >> 9) & NOT_H_FILE & board.bitboards[BLACK];
    TPB = TDB & ~NOT_8_RANK;
    TDB &= NOT_8_RANK;
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+9,bsf,0};
        //printf("PLA insert at %d: %d %d\n",m_i,bsf+9, bsf);

        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+9,bsf, 1};
        move_arr[m_i+1] = (move){bsf+9,bsf,2};
        move_arr[m_i+2] = (move){bsf+9,bsf,3};
        move_arr[m_i+3] = (move){bsf+9,bsf,4};
        m_i+=4;
    }

    //RIGHT ATTACKS
    TDB = (bb >> 7) & NOT_A_FILE & board.bitboards[BLACK];
    TPB = TDB & ~NOT_8_RANK;
    TDB &= NOT_8_RANK;
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+7,bsf,0};
        //printf("PRA insert at %d: %d %d\n",m_i,bsf+7, bsf);

        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+7,bsf, 1};
        move_arr[m_i+1] = (move){bsf+7,bsf, 2};
        move_arr[m_i+2] = (move){bsf+7,bsf, 3};
        move_arr[m_i+3] = (move){bsf+7,bsf, 4};
        m_i+=4;
    }

    //PUSHES
    TDB = (bb >> 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);
    TPB = TDB & ~NOT_8_RANK;//Promotions
    TDB &= NOT_8_RANK;//Remove promotions from normal pushes
    DPP = ((TDB & RANK_3)>>8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+8,bsf,0};
        //printf("PP insert at %d: %d %d\n",m_i,bsf+8, bsf);

        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+8,bsf, 1};
        move_arr[m_i+1] = (move){bsf+8,bsf, 2};
        move_arr[m_i+2] = (move){bsf+8,bsf, 3};
        move_arr[m_i+3] = (move){bsf+8,bsf, 4};
        m_i+=4;
    }
    while (DPP){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(DPP);
        DPP &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf+16,bsf,0};

        m_i++;
    }
}

void gen_pawn_black(uint64_t bb, move* move_arr){
    uint64_t TDB;//Temporary destination bitboard
    uint64_t TPB;//Temporary promotion bitboard
    uint64_t DPP;//Double pawn push bitboard
    uint64_t bsf;//Temporary bitscan forward result

    //LEFT ATTACKS (white perspective)
    TDB = (bb << 7) & NOT_H_FILE & board.bitboards[WHITE];
    TPB = TDB & ~NOT_1_RANK;
    TDB &= NOT_1_RANK;
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-7,bsf,0};
        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-7,bsf,1};
        move_arr[m_i+1] = (move){bsf-7,bsf, 2};
        move_arr[m_i+2] = (move){bsf-7,bsf, 3};
        move_arr[m_i+3] = (move){bsf-7,bsf, 4};
        m_i+=4;
    }

    //RIGHT ATTACKS (white perspective)
    TDB = (bb << 9) & NOT_A_FILE & board.bitboards[WHITE];
    TPB = TDB & ~NOT_1_RANK;
    TDB &= NOT_1_RANK;
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-9,bsf, 0};
        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-9,bsf, 1};
        move_arr[m_i+1] = (move){bsf-9,bsf, 2};
        move_arr[m_i+2] = (move){bsf-9,bsf, 3};
        move_arr[m_i+3] = (move){bsf-9,bsf, 4};
        m_i+=4;
    }

    //PUSHES
    TDB = (bb << 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);
    TPB = TDB & ~NOT_1_RANK;
    TDB &= NOT_1_RANK;
    DPP = ((TDB & RANK_6)<<8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);
    while (TDB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TDB);
        TDB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-8,bsf,0};
        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-8,bsf, 1};
        move_arr[m_i+1] = (move){bsf-8,bsf, 2};
        move_arr[m_i+2] = (move){bsf-8,bsf, 3};
        move_arr[m_i+3] = (move){bsf-8,bsf, 4};
        m_i+=4;
    }
    while (DPP){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(DPP);
        DPP &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-16,bsf, 0};

        m_i++;
    }
}

void gen_ep_white(move* move_arr){
    if(MD.ep_right && MD.ep_right > 23) return;
    uint64_t src_bb = ( SHIFT(MD.ep_right+9) | SHIFT(MD.ep_right+7) ) & board.bitboards[WHITE] & board.bitboards[PAWN];
    while(src_bb){
        int src_bsf = bitScanForward(src_bb);
        src_bb &= ~SHIFT(src_bsf);
        move_arr[m_i++] = (move){src_bsf,MD.ep_right, 0};
    }
}

void gen_ep_black(move* move_arr){
    if(MD.ep_right && MD.ep_right < 40) return;
    uint64_t src_bb = ( SHIFT(MD.ep_right-9) | SHIFT(MD.ep_right-7) ) & board.bitboards[BLACK] & board.bitboards[PAWN];
    while(src_bb){
        int src_bsf = bitScanForward(src_bb);
        src_bb &= ~SHIFT(src_bsf);
        move_arr[m_i++] = (move){src_bsf,MD.ep_right, 0};
    }
}
