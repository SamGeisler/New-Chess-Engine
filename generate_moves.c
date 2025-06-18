#include "global.h"
#include "bb_utils.h"
#include "generate_moves.h"
#include "control.h"

//All of the following return a bitboard of possible destinations (disregarding same-color collisions), which is converted into move structs in generate_moves
void gen_pawn_white(uint64_t bb, move* move_arr);
void gen_pawn_black(uint64_t bb, move* move_arr);

void (*gen_pawn[2]) (uint64_t, move*) = {&gen_pawn_white,&gen_pawn_black};
void gen_knight(uint64_t bb, move* move_arr, int color);
void gen_king(uint64_t bb, move* move_arr, int color);
void gen_rook(uint64_t bb, move* move_arr, int color);
void gen_bishop(uint64_t bb, move* move_arr, int color);
void gen_queen(uint64_t bb, move* move_arr, int color);


static int m_i;//Move array index

int generate_moves(move* move_arr, int color){
    m_i = 0;
    gen_pawn[color](board.bitboards[color] & board.bitboards[PAWN], move_arr);
    gen_knight(board.bitboards[color] & board.bitboards[KNIGHT], move_arr, color);
    gen_bishop(board.bitboards[color] & board.bitboards[BISHOP], move_arr, color);
    gen_rook(board.bitboards[color] & board.bitboards[ROOK], move_arr, color);
    gen_queen(board.bitboards[color] & board.bitboards[QUEEN], move_arr, color);
    gen_king(board.bitboards[color] & board.bitboards[KING], move_arr, color);

    return m_i;
}

void gen_knight(uint64_t bb, move* move_arr, int color){
    uint64_t dest_bb;
    int src_bsf;//First bsf result (location of knight)
    int dest_bsf;//Second bsf result (destination)
    while(bb){
        src_bsf = bitScanForward(bb);

        bb &= ~SHIFT(src_bsf);

        dest_bb = knightDest[src_bsf];


        dest_bb &= ~board.bitboards[color];
        

        while(dest_bb){
            dest_bsf = bitScanForward(dest_bb);

            dest_bb &= ~SHIFT(dest_bsf);
            move_arr[m_i] = (move){src_bsf,dest_bsf,0};
            m_i++;
        }
    }
}

void gen_king(uint64_t bb, move* move_arr, int color){
    uint64_t dest_bb;
    int src_bsf;//First bsf result (location of king)
    int dest_bsf;//Second bsf result (destination)
    while(bb){
        src_bsf = bitScanForward(bb);
        bb &= ~SHIFT(src_bsf);

        dest_bb = kingDest[src_bsf];
        dest_bb &= ~board.bitboards[color];
        
        while(dest_bb){
            dest_bsf = bitScanForward(dest_bb);
            dest_bb &= ~SHIFT(dest_bsf);

            move_arr[m_i] = (move){src_bsf,dest_bsf,0};
            //make printf("king insert at %d: %d %d\n",m_i,src_bsf, dest_bsf);

            m_i++;
        }
    }
}

void gen_rook(uint64_t bb, move* move_arr, int color){
    uint64_t dest_bb;
    int src_bsf;//First bsf result (location of rook)
    int dest_bsf;//Second bsf result (destination)

    while(bb){
        src_bsf = bitScanForward(bb);
        bb &= ~SHIFT(src_bsf);

        uint64_t int_bb = rookDestTrunc[src_bsf] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
        int index = (int_bb * rookMN[src_bsf]) >> (64-rookMN_w[src_bsf]);
        dest_bb = rookDestInt[src_bsf][index];
        
        dest_bb &= ~board.bitboards[color];

        while(dest_bb){
            dest_bsf = bitScanForward(dest_bb);
            dest_bb &= ~SHIFT(dest_bsf);

            move_arr[m_i] = (move){src_bsf,dest_bsf,0};

            m_i++;
        }
    }
}

void gen_bishop(uint64_t bb, move* move_arr, int color){
    uint64_t dest_bb;
    int src_bsf;//First bsf result (location of bishop)
    int dest_bsf;//Second bsf result (destination)

    while(bb){
        src_bsf = bitScanForward(bb);
        bb &= ~SHIFT(src_bsf);

        uint64_t int_bb = bishopDestTrunc[src_bsf] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
        int index = (int_bb * bishopMN[src_bsf]) >> (64-bishopMN_w[src_bsf]);
        dest_bb = bishopDestInt[src_bsf][index];
        
        dest_bb &= ~board.bitboards[color];

        while(dest_bb){
            dest_bsf = bitScanForward(dest_bb);
            dest_bb &= ~SHIFT(dest_bsf);

            move_arr[m_i] = (move){src_bsf,dest_bsf,0};

            m_i++;
        }
    }
}

void gen_queen(uint64_t bb, move* move_arr, int color){
    uint64_t dest_bb;
    int src_bsf;//First bsf result (location of queen)
    int dest_bsf;//Second bsf result (destination)

    while(bb){
        src_bsf = bitScanForward(bb);
        bb &= ~SHIFT(src_bsf);

        uint64_t int_bb = rookDestTrunc[src_bsf] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
        int index = (int_bb * rookMN[src_bsf]) >> (64-rookMN_w[src_bsf]);
        dest_bb = rookDestInt[src_bsf][index];

        int_bb = bishopDestTrunc[src_bsf] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
        index = (int_bb * bishopMN[src_bsf]) >> (64-bishopMN_w[src_bsf]);
        dest_bb |= bishopDestInt[src_bsf][index];

        dest_bb &= ~board.bitboards[color];
        
        while(dest_bb){
            dest_bsf = bitScanForward(dest_bb);
            dest_bb &= ~SHIFT(dest_bsf);

            move_arr[m_i] = (move){src_bsf,dest_bsf,0};

            m_i++;
        }
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
        move_arr[m_i] = (move){bsf+9,bsf,1};
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
        move_arr[m_i] = (move){bsf+7,bsf,1};
        move_arr[m_i+1] = (move){bsf+7,bsf,2};
        move_arr[m_i+2] = (move){bsf+7,bsf,3};
        move_arr[m_i+3] = (move){bsf+7,bsf,4};
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
        move_arr[m_i] = (move){bsf+8,bsf,1};
        move_arr[m_i+1] = (move){bsf+8,bsf,2};
        move_arr[m_i+2] = (move){bsf+8,bsf,3};
        move_arr[m_i+3] = (move){bsf+8,bsf,4};
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
        move_arr[m_i+1] = (move){bsf-7,bsf,2};
        move_arr[m_i+2] = (move){bsf-7,bsf,3};
        move_arr[m_i+3] = (move){bsf-7,bsf,4};
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
        move_arr[m_i] = (move){bsf-9,bsf,0};
        m_i++;
    }
    while (TPB){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(TPB);
        TPB &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-9,bsf,1};
        move_arr[m_i+1] = (move){bsf-9,bsf,2};
        move_arr[m_i+2] = (move){bsf-9,bsf,3};
        move_arr[m_i+3] = (move){bsf-9,bsf,4};
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
        move_arr[m_i] = (move){bsf-8,bsf,1};
        move_arr[m_i+1] = (move){bsf-8,bsf,2};
        move_arr[m_i+2] = (move){bsf-8,bsf,3};
        move_arr[m_i+3] = (move){bsf-8,bsf,4};
        m_i+=4;
    }
    while (DPP){
        //Find least significant positive bit and remove it
        bsf = bitScanForward(DPP);
        DPP &= ~SHIFT(bsf);

        //Insert move struct
        move_arr[m_i] = (move){bsf-16,bsf,0};

        m_i++;
    }
}