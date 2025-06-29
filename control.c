#include "SDL.h"

#include "global.h"
#include "control.h"
#include "render.h"
#include "generate_moves.h"

void execute_move(move m){
    int color = board_arr[m.src]>8;
    int color_mod = (2*color-1);//-1 for WHITE, 1 for BLACK
    int piece = board_arr[m.src]%8;
    int captured = board_arr[m.dest]%8;

    if(piece==PAWN || board_arr[m.dest]){
        MD.fmr_count = 0;
    } else {
        MD.fmr_count++;
    }

    board_arr[m.dest] = board_arr[m.src];
    board_arr[m.src] = EMPTY;

    //Remove moving piece from source
    board.bitboards[color] &= ~SHIFT(m.src);
    board.bitboards[piece] &= ~SHIFT(m.src);

    //Remove captured piece from destination
    board.bitboards[1-color] &= ~SHIFT(m.dest);
    if(captured) board.bitboards[captured] &= ~SHIFT(m.dest);

    //Add moving piece to destination
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

move handle_player_input(uint64_t render_bb){
    move* moves = (move*)malloc(220*sizeof(move));
    int num_moves = generate_moves(moves, 1-AICOLOR);

    SDL_Event e;
    int holding = -1;
    int mx, my;
    move move_found = (move){64,64};

    while(move_found.src==64){
    
        render_board(holding, render_bb);
        while(SDL_PollEvent(&e)){
            SDL_GetMouseState(&mx,&my);

            if(e.type==SDL_QUIT){
                render_quit();
                exit(0);
            } else if(e.type==SDL_MOUSEBUTTONDOWN && holding == -1){
                if(mx < XMARG || my < YMARG || mx > SWIDTH-XMARG || my > SHEIGHT-YMARG ) continue;

                holding = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                if(board_arr[holding]==0) holding = -1;

            } else if(e.type==SDL_MOUSEBUTTONUP && holding != -1 ){
                if(mx < XMARG || my < YMARG || mx > SWIDTH-XMARG || my > SHEIGHT-YMARG || ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS)==holding){
                    holding = -1;
                } else {
                    int try_src = holding;
                    int try_dest = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                    for(int i = 0; i<num_moves; i++){
                        if(moves[i].src == try_src && moves[i].dest == try_dest){
                            move_found.src = holding;
                            move_found.dest = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                            break;
                        }
                        if(i==num_moves-1) printf("Invalid move! - %d -> %d\n",try_src, try_dest);
                    }
                    holding = -1;
                }
            }
        }
    }
    return move_found;
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