#include "global.h"
#include "control.h"
#include "generate_moves.h"

void execute_move(move m){
    int color = board_arr[m.src]>8;
    int color_mod = (2*color-1);//-1 for WHITE, 1 for BLACK
    int piece = board_arr[m.src]%8;
    int captured = board_arr[m.dest]%8;

    //Update player to move
    MD.to_move = 1-MD.to_move;

    //Update fifty-move-rule count
    if(piece==PAWN || board_arr[m.dest]){
        MD.fmr_count = 0;
    } else {
        MD.fmr_count++;
    }

    //Bitboards: Remove moving piece from source
    board.bitboards[color] &= ~SHIFT(m.src);
    board.bitboards[piece] &= ~SHIFT(m.src);

    //Bitboards: Remove captured piece from destination
    board.bitboards[1-color] &= ~SHIFT(m.dest);
    if(captured) board.bitboards[captured] &= ~SHIFT(m.dest);

    //Check for promotion
    if(m.promo){
        switch(m.promo){
            case 1: piece = KNIGHT; break;
            case 2: piece = BISHOP; break;
            case 3: piece = ROOK; break;
            case 4: piece = QUEEN; break;
        }
    }

    //Update board array
    board_arr[m.dest] = piece + 8*color;
    board_arr[m.src] = EMPTY;

    //Bitboards: Add moving piece to destination
    board.bitboards[color] |= SHIFT(m.dest);
    board.bitboards[piece] |= SHIFT(m.dest);
    
    //Handle enpassant capture
    if(m.dest==MD.ep_right && piece==PAWN){
        int captured_square = MD.ep_right - 8*color_mod;
        board.bitboards[1-color] &= ~SHIFT(captured_square);
        board.bitboards[PAWN] &= ~SHIFT(captured_square);
        board_arr[captured_square] = EMPTY;
    }

    //Update enpassant metadata
    MD.ep_right = 0;
    if(piece==PAWN && m.dest == m.src + color_mod*16){
        MD.ep_right = m.dest - color_mod*8;
    }

    //Move rook for castling and update castling right for king moves
    if(piece == KING){
        MD.castle_flags &= 0b0011 << (2*color);
        if(m.src == 60 && m.dest == 62){
            board_arr[61] = WHITE_ROOK;
            board_arr[63] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(61);
            board.bitboards[WHITE] |= SHIFT(61);
            board.bitboards[ROOK] ^= SHIFT(63);
            board.bitboards[WHITE] ^= SHIFT(63);
        } else if(m.src == 60 && m.dest == 58){
            board_arr[59] = WHITE_ROOK;
            board_arr[56] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(59);
            board.bitboards[WHITE] |= SHIFT(59);
            board.bitboards[ROOK] ^= SHIFT(56);
            board.bitboards[WHITE] ^= SHIFT(56);
        } else if(m.src == 4 && m.dest == 6){
            board_arr[5] = BLACK_ROOK;
            board_arr[7] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(5);
            board.bitboards[BLACK] |= SHIFT(5);
            board.bitboards[ROOK] ^= SHIFT(7);
            board.bitboards[BLACK] ^= SHIFT(7);
        } else if(m.src == 4 && m.dest == 2){
            board_arr[3] = BLACK_ROOK;
            board_arr[0] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(3);
            board.bitboards[BLACK] |= SHIFT(3);
            board.bitboards[ROOK] ^= SHIFT(0);
            board.bitboards[BLACK] ^= SHIFT(0);
        }
    }

    //Update castling metadata for rook moves/takes
    if(m.src==0 || m.dest==0) MD.castle_flags &= ~1;
    if(m.src==7 || m.dest==7) MD.castle_flags &= ~2;
    if(m.src==56 || m.dest==56) MD.castle_flags &= ~4;
    if(m.src==63 || m.dest==63) MD.castle_flags &= ~8;
}

//Reverts from execute_move - does not fix metadata
//int dest_square - piece located in destination square before move, if any (0=UNUSED1 if none). Bitboard index format (color is not included)
void unexecute_move(move m, int dest_square, int was_ep){
    int color = board_arr[m.dest]>8;
    int piece = board_arr[m.dest]%8;

    //Bitboards: Remove piece from destination square
    board.bitboards[color] &= ~SHIFT(m.dest);
    board.bitboards[piece] &= ~SHIFT(m.dest);

    //Bitboards: Return captured piece (if any) to destination square
    if(dest_square){
        board.bitboards[1-color] |= SHIFT(m.dest);
        board.bitboards[dest_square] |= SHIFT(m.dest);
    }
    
    if(m.promo){
        piece = PAWN;
    }

    //Bitboards: Return piece to starting square
    board.bitboards[color] |= SHIFT(m.src);
    board.bitboards[piece] |= SHIFT(m.src);

    //Board array: Fix
    board_arr[m.src] = piece + 8*color;
    board_arr[m.dest] = dest_square;

    //Undo en-passant capture
    if(was_ep){ 
        int repair_square = m.dest + (16*color-8);
        board_arr[repair_square] = PAWN + 8*(1-color);

        board.bitboards[1-color] |= SHIFT(repair_square);
        board.bitboards[PAWN] |= SHIFT(repair_square);
    }

    //Undo rook move for castling
    if(piece == KING){
        if(m.src == 60 && m.dest == 62){
            board_arr[63] = WHITE_ROOK;
            board_arr[61] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(63);
            board.bitboards[WHITE] |= SHIFT(63);
            board.bitboards[ROOK] ^= SHIFT(61);
            board.bitboards[WHITE] ^= SHIFT(61);
        } else if(m.src == 60 && m.dest == 58){
            board_arr[56] = WHITE_ROOK;
            board_arr[59] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(56);
            board.bitboards[WHITE] |= SHIFT(56);
            board.bitboards[ROOK] ^= SHIFT(59);
            board.bitboards[WHITE] ^= SHIFT(59);
        } else if(m.src == 4 && m.dest == 6){
            board_arr[7] = BLACK_ROOK;
            board_arr[5] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(7);
            board.bitboards[BLACK] |= SHIFT(7);
            board.bitboards[ROOK] ^= SHIFT(5);
            board.bitboards[BLACK] ^= SHIFT(5);
        } else if(m.src == 4 && m.dest == 2){
            board_arr[0] = BLACK_ROOK;
            board_arr[3] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(0);
            board.bitboards[BLACK] |= SHIFT(0);
            board.bitboards[ROOK] ^= SHIFT(3);
            board.bitboards[BLACK] ^= SHIFT(3);
        }
    }
}

move get_engine_move(){

}

//The color passed is the color to move
int game_end(int color, int is_in_check, int num_moves){
    if(num_moves == 0){
        if(is_in_check){
            return 1-2*color;
        } else {
            return 0;
        }
    }
    if(MD.fmr_count==100) return 0;
    
    return 2;
}