#include "SDL.h"
#include "global.h"
#include "control.h"
#include "render.h"
#include "player_input.h"
#include "generate_moves.h"

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

                            if((move_found.dest < 8 || move_found.dest >=56) && (board_arr[move_found.src] == WHITE_PAWN || board_arr[move_found.src] == BLACK_PAWN)){
                                int promo_inp;
                                printf("Enter piece to promote to, 1-Knight, 2-Bishop, 3-Rook, 4-Queen:");
                                while(!scanf("%d",&promo_inp) || promo_inp > 4 || promo_inp < 1){
                                    printf("Invalid input\n");
                                }
                                move_found.promo = promo_inp;
                            }

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