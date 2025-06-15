#include "SDL.h"

#include "global.h"
#include "control.h"
#include "render.h"

void execute_move(move m){
    board_arr[m.dest] = board_arr[m.src];
    board_arr[m.src] = 0;
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