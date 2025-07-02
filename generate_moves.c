#include "global.h"
#include "generate_moves.h"
#include "control.h"

//All of the following return a bitboard of possible destinations (disregarding same-color collisions), which is converted into move structs in generate_moves
uint64_t gen_pawn_white(uint64_t pawn_pos);
uint64_t gen_pawn_black(uint64_t pawn_pos);

uint64_t (*gen_pawn_ptr[2]) (uint64_t) = {&gen_pawn_white,&gen_pawn_black};
uint64_t gen_pawn(int src, int color);

//Get bitboard of possible destinations from bitboard of pieces to move
//These do not exclude same-color collisions
uint64_t gen_knight(int src, int color);
uint64_t gen_bishop(int src, int color);
uint64_t gen_rook(int src, int color);
uint64_t gen_queen(int src, int color);
uint64_t gen_king(int src, int color);

uint64_t (*get_piece_dest_bb[6]) (int, int) = {&gen_pawn, &gen_knight, &gen_bishop, &gen_rook, &gen_queen, &gen_king};

void append_moves(int src, uint64_t dest_bb, move** move_arr_head);
void append_pawn_moves(int src, int color, uint64_t dest_bb, move** move_arr_head);

uint64_t gen_xray_rook(uint64_t pieces, uint64_t blockers, int src);
uint64_t gen_xray_bishop(uint64_t pieces, uint64_t blockers, int src);

int generate_moves(move* move_arr, int color){
    if(is_in_check(color)){
        return generate_moves_check(move_arr,color);
    }

    move* move_arr_head = move_arr;

    int king_pos = bitScanForward(board.bitboards[KING] & board.bitboards[color]);
    uint64_t pinned = get_pinned(king_pos, color);

    for(int piece = PAWN; piece<=KING; piece++){

        uint64_t src_bb = board.bitboards[color] & board.bitboards[piece];
        
        uint64_t pinned_to_move = pinned & src_bb;
        src_bb ^= pinned_to_move;

        while(src_bb){
            int src_bsf = bitScanForward(src_bb);//Index of leftmost piece
            src_bb &= ~SHIFT(src_bsf);

            uint64_t dest_bb = get_piece_dest_bb[piece-2](src_bsf, color) & ~board.bitboards[color];
            if(piece==PAWN) append_pawn_moves(src_bsf, color, dest_bb, &move_arr_head);
            else append_moves(src_bsf, dest_bb, &move_arr_head);
        }

        while(pinned_to_move){
            int src_bsf = bitScanForward(pinned_to_move);
            pinned_to_move &= ~SHIFT(src_bsf);

            uint64_t dest_bb = get_piece_dest_bb[piece-2](src_bsf, color) & ~board.bitboards[color];

            if(src_bsf%8 == king_pos%8){
                dest_bb &= FILES[src_bsf%8];
            } else if(src_bsf/8 == king_pos/8){
                dest_bb &= RANKS[ 7 - src_bsf/8 ];
            } else if(diagLU_NE[src_bsf]==diagLU_NE[king_pos]){
                dest_bb &= DIAGONALS_NE[diagLU_NE[src_bsf]];
            } else if(diagLU_NW[src_bsf] == diagLU_NW[king_pos]){
                dest_bb &= DIAGONALS_NW[diagLU_NW[src_bsf]];
            }

            if(piece==PAWN) append_pawn_moves(src_bsf, color, dest_bb, &move_arr_head);
            else append_moves(src_bsf, dest_bb, &move_arr_head);

        }
    }

    return move_arr_head - move_arr;
}

int generate_moves_check(move* move_arr, int color){
    move* move_arr_head = move_arr;

    uint64_t king_bb = board.bitboards[color] & board.bitboards[KING];
    int king_pos = bitScanForward(king_bb);

    uint64_t pinned = get_pinned(king_pos, color);

    uint64_t sliding_attackers = gen_rook(king_pos, color) & (board.bitboards[ROOK] | board.bitboards[QUEEN]) & board.bitboards[1-color];
    
    uint64_t slide_through_dest; //Full trajectory of sliding pieces, to ensure king leaves rank/file/diagonal
    uint64_t temp = sliding_attackers;
    while(temp){
        int src = bitScanForward(temp);
        temp ^= SHIFT(src);
        slide_through_dest |= rookDest[src];
    }

    temp = gen_bishop(king_pos, color) & (board.bitboards[BISHOP] | board.bitboards[QUEEN]) & board.bitboards[1-color];
    sliding_attackers |= temp;

    while(temp){
        int src = bitScanForward(temp);
        temp ^= SHIFT(src);
        slide_through_dest |= bishopDest[src];
    }

    int numSliding = count_bits(sliding_attackers);

    uint64_t pawn_knight_attackers = gen_knight(king_pos, color) & board.bitboards[1-color] & board.bitboards[KNIGHT];
    if(color==WHITE){
        pawn_knight_attackers |=  ( (king_bb >> 7) | (king_bb >> 9) ) & board.bitboards[BLACK] & board.bitboards[PAWN];
    } else {
        pawn_knight_attackers |=  ( (king_bb << 7) | (king_bb << 9) ) & board.bitboards[WHITE] & board.bitboards[PAWN];
    }
    int numPK = count_bits(pawn_knight_attackers);


    if(numSliding && numPK || numSliding > 2 || numPK >2){
        append_moves(king_pos, gen_king(king_pos, color) & ~board.bitboards[color] & ~slide_through_dest, &move_arr_head);
    } else {
        //Bitboard of legal destination squares: If single sliding attacker, inbetween or capture. If single other attacker, only capture
        uint64_t check_restriction_mask = numPK ? pawn_knight_attackers : inBetween[king_pos][bitScanForward(sliding_attackers)] | sliding_attackers;
        
        for(int piece = PAWN; piece<=QUEEN; piece++){
            uint64_t src_bb = board.bitboards[color] & board.bitboards[piece];
            
            //Absolutely pinned pieces cannot move at all
            src_bb &= ~pinned;

            while(src_bb){
                int src_bsf = bitScanForward(src_bb);//Index of leftmost piece
                src_bb &= ~SHIFT(src_bsf);

                uint64_t dest_bb = get_piece_dest_bb[piece-2](src_bsf, color) & ~board.bitboards[color];
                dest_bb &= check_restriction_mask;

                if(piece==PAWN) append_pawn_moves(src_bsf, color,dest_bb, &move_arr_head);
                else append_moves(src_bsf, dest_bb, &move_arr_head);
            }
        }  
        append_moves(king_pos, gen_king(king_pos, color) & ~board.bitboards[color] & ~slide_through_dest, &move_arr_head);
    } 

    return move_arr_head-move_arr;
}

int is_in_check(int color){
    uint64_t king = board.bitboards[color] & board.bitboards[KING];
    return (king & get_attacking_bitboard(1-color))!=0;
}

uint64_t gen_knight(int src, int color){
    return knightDest[src];
}

uint64_t gen_king(int src, int color){
    uint64_t attacking = get_attacking_bitboard(1-color);
    uint64_t occ =  (board.bitboards[color] & ~board.bitboards[KING]) | board.bitboards[1-color];
    uint64_t rv = kingDest[src] & ~attacking;

    if(color==WHITE){
        if( (MD.castle_flags & 8) && !((attacking | occ) & WHITE_KS_CASTLE_MASK) ){
            rv |= 0x4000000000000000;
        }
        if( (MD.castle_flags & 4) && !((attacking | occ) & WHITE_QS_CASTLE_MASK) ){
            rv |= 0x0400000000000000;
        }
    } else {
        if( (MD.castle_flags & 2) && !((attacking | occ) & BLACK_KS_CASTLE_MASK) ){
            rv |= 0x40;
        }
        if( (MD.castle_flags & 1) && !((attacking | occ) & BLACK_QS_CASTLE_MASK) ){
            rv |= 0x4;
        }
    }
    return rv;
}

uint64_t gen_rook(int src, int color){
    uint64_t int_bb = rookDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);
    return rookDestInt[src][index];
}

uint64_t gen_bishop(int src, int color){
    uint64_t int_bb = bishopDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);
    return bishopDestInt[src][index];
}

uint64_t gen_queen(int src, int color){
    uint64_t int_bb = rookDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);
    uint64_t dest_bb = rookDestInt[src][index];

    int_bb = bishopDestTrunc[src] & (board.bitboards[WHITE] | board.bitboards[BLACK]);//Intersection of attacking cross wtih other pieces
    index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);
    dest_bb |= bishopDestInt[src][index];

    return dest_bb;
}

void append_moves(int src, uint64_t dest_bb, move** move_arr_head){
    int dest_bsf;
    while(dest_bb){
        int dest_bsf = bitScanForward(dest_bb);
        dest_bb &= ~SHIFT(dest_bsf);

        *((*move_arr_head)++) = (move){src, dest_bsf, 0};
    }
}

void append_pawn_moves(int src, int color, uint64_t dest_bb, move** move_arr_head){
    int dest_bsf;
    while(dest_bb){
        int dest_bsf = bitScanForward(dest_bb);
        dest_bb &= ~SHIFT(dest_bsf);
        if(dest_bsf/8 == 7*color){//Moving to final rank  
            *((*move_arr_head)++) = (move){src, dest_bsf, 1};
            *((*move_arr_head)++) = (move){src, dest_bsf, 2};
            *((*move_arr_head)++) = (move){src, dest_bsf, 3};
            *((*move_arr_head)++) = (move){src, dest_bsf, 4};
        } else {
            *((*move_arr_head)++) = (move){src, dest_bsf, 0};
        }
    }
}

uint64_t gen_pawn(int src, int color){
    return gen_pawn_ptr[color](SHIFT(src));
}

uint64_t gen_pawn_white(uint64_t pawn_pos){
    uint64_t rv = 0;
    rv |= (pawn_pos >> 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);//Single push
    rv |= ( (rv&RANKS[2]) >> 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);//Double push
    rv |= ((pawn_pos >> 7) | (pawn_pos >> 9)) & board.bitboards[BLACK];//Attacks
    if(MD.ep_right) rv |= ((pawn_pos >> 7) | (pawn_pos >> 9)) & SHIFT(MD.ep_right);//en passant
    return rv;
}

uint64_t gen_pawn_black(uint64_t pawn_pos){
    uint64_t rv = 0;
    rv |= (pawn_pos << 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);//Single push
    rv |= ( (rv&RANKS[5]) << 8) & ~(board.bitboards[BLACK] | board.bitboards[WHITE]);//Double push
    rv |= ((pawn_pos << 7) | (pawn_pos << 9)) & board.bitboards[WHITE];//Attacks
    if(MD.ep_right) rv |= ((pawn_pos << 7) | (pawn_pos << 9)) & SHIFT(MD.ep_right);//en passant
    return rv;
}

//Used for king move generation (returns the attacks of the passed color)
uint64_t get_attacking_bitboard(int color){
    uint64_t rv = 0;
    uint64_t pawns = board.bitboards[color] & board.bitboards[PAWN];
    if(color==WHITE){
        rv |= (pawns >> 9) & NOT_H_FILE;
        rv |= (pawns >> 7) & NOT_A_FILE;
    } else {
        rv |= (pawns << 9) & NOT_A_FILE;
        rv |= (pawns << 7) & NOT_H_FILE;
    }

    uint64_t knights = board.bitboards[color] & board.bitboards[KNIGHT];
    while(knights){
        int src = bitScanForward(knights);
        knights &= ~SHIFT(src);
        rv |= knightDest[src];
    }

    rv |= kingDest[bitScanForward(board.bitboards[color] & board.bitboards[KING])];

    uint64_t rooks = board.bitboards[color] & board.bitboards[ROOK];
    while(rooks){
        int src = bitScanForward(rooks);
        rooks &= ~SHIFT(src);
        rv |= gen_rook(src, color);
    }
    uint64_t bishops = board.bitboards[color] & board.bitboards[BISHOP];
    while(bishops){
        int src = bitScanForward(bishops);
        bishops &= ~SHIFT(src);
        rv |= gen_bishop(src, color);
    }
    uint64_t queens = board.bitboards[color] & board.bitboards[QUEEN];
    while(queens){
        int src = bitScanForward(queens);
        queens &= ~SHIFT(src);
        rv |= gen_queen(src, color);
    }

    return rv;
}

uint64_t get_pinned(int king_pos, int color){
    uint64_t pinned = 0 ;

    uint64_t enemy_sliders = gen_xray_rook(board.bitboards[WHITE] | board.bitboards[BLACK], board.bitboards[color], king_pos) & (board.bitboards[1-color] & (board.bitboards[QUEEN] | board.bitboards[ROOK]));
    while(enemy_sliders){
        int src = bitScanForward(enemy_sliders);
        enemy_sliders &= ~SHIFT(src);

        uint64_t int_bb = rookDestTrunc[src] & ((board.bitboards[WHITE] | board.bitboards[BLACK]));
        int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);
        pinned |= inBetween[src][king_pos] & board.bitboards[color];
    }

    enemy_sliders = gen_xray_bishop(board.bitboards[WHITE] | board.bitboards[BLACK], board.bitboards[color], king_pos) & (board.bitboards[1-color] & (board.bitboards[QUEEN] | board.bitboards[BISHOP]));
    while(enemy_sliders){
        int src = bitScanForward(enemy_sliders);
        enemy_sliders &= ~SHIFT(src);

        uint64_t int_bb = bishopDestTrunc[src] & ((board.bitboards[WHITE] | board.bitboards[BLACK]));
        int index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);
        pinned |= inBetween[src][king_pos] & board.bitboards[color];
    }
    return pinned;
}

uint64_t gen_xray_rook(uint64_t pieces, uint64_t blockers, int src){
    uint64_t int_bb = rookDestTrunc[src] & (pieces);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);

    uint64_t dest_bb = rookDestInt[src][index];
    blockers &= dest_bb;

    int_bb = rookDestTrunc[src] & (pieces ^ blockers);//Intersection of attacking cross wtih other pieces
    index = (int_bb * rookMN[src]) >> (64-rookMN_w[src]);

    return dest_bb ^ rookDestInt[src][index];

}

uint64_t gen_xray_bishop(uint64_t pieces, uint64_t blockers, int src){
    uint64_t int_bb = bishopDestTrunc[src] & (pieces);//Intersection of attacking cross wtih other pieces
    int index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);

    uint64_t dest_bb = bishopDestInt[src][index];
    blockers &= dest_bb;

    int_bb = bishopDestTrunc[src] & (pieces ^ blockers);//Intersection of attacking cross wtih other pieces
    index = (int_bb * bishopMN[src]) >> (64-bishopMN_w[src]);

    return dest_bb ^ bishopDestInt[src][index];
}
