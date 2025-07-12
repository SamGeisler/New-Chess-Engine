#include "SDL.h"
#include "global.h"
#include "control.h"
#include "render.h"
#include "playerInput.h"
#include "generateMoves.h"

Move handlePlayerInput(uint64_t renderBb){
    std::array<Move, 220> moves;
    int numMoves = generateMoves(moves, 1-AICOLOR);

    SDL_Event e;
    int holding = -1;
    int mx, my;
    Move moveFound = (Move){64,64,0};

    while(moveFound.src==64){
    
        renderBoard(holding, renderBb);
        while(SDL_PollEvent(&e)){
            SDL_GetMouseState(&mx,&my);

            if(e.type==SDL_QUIT){
                renderQuit();
                exit(0);
            } else if(e.type==SDL_MOUSEBUTTONDOWN && holding == -1){
                if(mx < XMARG || my < YMARG || mx > SWIDTH-XMARG || my > SHEIGHT-YMARG ) continue;

                holding = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                if(boardArr[holding]==0) holding = -1;

            } else if(e.type==SDL_MOUSEBUTTONUP && holding != -1 ){
                if(mx < XMARG || my < YMARG || mx > SWIDTH-XMARG || my > SHEIGHT-YMARG || ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS)==holding){
                    holding = -1;
                } else {
                    int trySrc = holding;
                    int tryDest = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);
                    for(int i = 0; i<numMoves; i++){
                        if(moves[i].src == trySrc && moves[i].dest == tryDest){
                            moveFound.src = holding;
                            moveFound.dest = ((mx-XMARG) / PS) + 8* ( (my-YMARG) / PS);

                            if((moveFound.dest < 8 || moveFound.dest >=56) && (boardArr[moveFound.src] == WHITE_PAWN || boardArr[moveFound.src] == BLACK_PAWN)){
                                int promoInp;
                                std::cout << "Enter piece to promote to, 1-Knight, 2-Bishop, 3-Rook, 4-Queen: ";

                                int promoInp{0};
                                std::cin >> promoInp;
                                while(promoInp > 4 || promoInp < 1){
                                    std::cout << "Invalid input\n";
                                    std::cin >> promoInp;
                                }
                                moveFound.promo = promoInp;
                            }

                            break;
                        }
                        if(i==numMoves-1) 
                            std::cout << "Invalid move! - " <<  trySrc << " -> " << tryDest << "\n";
                    }
                    holding = -1;
                }
            }
        }
    }
    return moveFound;
}