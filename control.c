#include "SDL.h"

#include "global.h"
#include "control.h"
#include "render.h"

void execute_move(move m){
    int color = board_arr[m.src]>8;
    int color_mod = (2*color-1);//-1 for WHITE, 1 for BLACK
    int piece = board_arr[m.src]%8;
    int captured = board_arr[m.dest]%8;
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
}

move handle_player_input(){
    SDL_Event e;
    int holding = -1;
    int mx, my;
    move move_found = (move){64,64};

    while(move_found.src==64){
    
        render_board(holding);
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
                    move_found.src = holding;
                    move_found.dest = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                    holding = -1;
                }
            }
        }
    }
    return move_found;
}

move get_engine_move(){

}