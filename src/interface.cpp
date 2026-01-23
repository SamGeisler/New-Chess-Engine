#include "interface.h"
#include "game.h"
#include "lookup.h"
#include "move.h"

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currentSurface = NULL;
SDL_Surface* numberSurface = NULL;

SDL_Surface* pieceSurfaces[16];

Interface::Interface()
    : game(Game::WHITE, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1")
{
    renderInit();
}

Interface::Interface(int _playerColor, std::string_view _initBoard)
    : game(_playerColor, _initBoard)
{
    renderInit();
}

SDL_Surface* Interface::loadSurface(const char * path){
    SDL_Surface* loaded = SDL_LoadBMP(path);
    SDL_Surface* optimized = SDL_ConvertSurface(loaded, screenSurface -> format, 0);
    SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0x00, 0xFF, 0xFF));
    SDL_FreeSurface(loaded);
    return optimized;
}

void Interface::drawBackground(uint64_t bb){
    SDL_FillRect(currentSurface, NULL, BGCOLOR);
    SDL_Rect square = {0,0,SQUARE_WIDTH,SQUARE_WIDTH};
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            square.x = XMARG() + j*SQUARE_WIDTH;
            square.y = YMARG() + i*SQUARE_WIDTH;

            int color;
            if(bb & SHIFT(8*i +j))
                color = ((i+j)%2==0 ? LIGHT_ATTACKING : DARK_ATTACKING);
            else 
                color = ((i+j)%2==0 ? LIGHT_SPACE_COLOR : DARK_SPACE_COLOR);

            SDL_FillRect(currentSurface, &square, color);
        }
    }
    SDL_BlitScaled(numberSurface, NULL, currentSurface, NULL);

}


void Interface::loadPieceSurfaces(){
    pieceSurfaces[Game::Piece::WHITE_PAWN] = loadSurface("img/white_pawn.bmp");
    pieceSurfaces[Game::Piece::WHITE_KNIGHT] = loadSurface("img/white_knight.bmp");
    pieceSurfaces[Game::Piece::WHITE_BISHOP] = loadSurface("img/white_bishop.bmp");
    pieceSurfaces[Game::Piece::WHITE_ROOK] = loadSurface("img/white_rook.bmp");
    pieceSurfaces[Game::Piece::WHITE_QUEEN] = loadSurface("img/white_queen.bmp");
    pieceSurfaces[Game::Piece::WHITE_KING] = loadSurface("img/white_king.bmp");
    pieceSurfaces[Game::Piece::BLACK_PAWN] = loadSurface("img/black_pawn.bmp");
    pieceSurfaces[Game::Piece::BLACK_KNIGHT] = loadSurface("img/black_knight.bmp");
    pieceSurfaces[Game::Piece::BLACK_BISHOP] = loadSurface("img/black_bishop.bmp");
    pieceSurfaces[Game::Piece::BLACK_ROOK] = loadSurface("img/black_rook.bmp");
    pieceSurfaces[Game::Piece::BLACK_QUEEN] = loadSurface("img/black_queen.bmp");
    pieceSurfaces[Game::Piece::BLACK_KING] = loadSurface("img/black_king.bmp");
}

void Interface::renderBoard(int holding, uint64_t bb){

    drawBackground(bb);
    if(1){
        for(int i = 0; i<64; i++){
            if(game.boardArr[i] && (holding==-1 || i!=holding)){
                SDL_Rect locRect = {XMARG() + SQUARE_WIDTH*(i%8), YMARG() + SQUARE_WIDTH*(i/8), SQUARE_WIDTH, SQUARE_WIDTH};
                SDL_BlitScaled(pieceSurfaces[game.boardArr[i]], NULL, currentSurface, &locRect);
            }
        }
    } else {//Later board flipping for black player
        for(int i = 0; i<64; i++){
            if(game.boardArr[63-i] && (holding==-1 || 63-i!=holding)){
                SDL_Rect locRect = {XMARG() + SQUARE_WIDTH*(i%8), YMARG() + SQUARE_WIDTH*(i/8), SQUARE_WIDTH, SQUARE_WIDTH};
                SDL_BlitScaled(pieceSurfaces[game.boardArr[63-i]], NULL, currentSurface, &locRect);
            }
        }
    }
    if(holding != -1){
        int hx, hy; SDL_GetMouseState(&hx, &hy);
        SDL_Rect holdingLocRect = {hx-SQUARE_WIDTH/2, hy-SQUARE_WIDTH/2, SQUARE_WIDTH, SQUARE_WIDTH};
        SDL_BlitScaled(pieceSurfaces[game.boardArr[holding]], NULL, currentSurface, &holdingLocRect);
    }

    SDL_BlitScaled(currentSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);

}

void Interface::renderInit(){
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
        std::cout << "SDL could not be initialized. Error: " << SDL_GetError() << "\n";
	}
    
    window = SDL_CreateWindow("Chess Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_SHOWN);
    if(FULLSCREEN) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    screenSurface = SDL_GetWindowSurface(window);


    currentSurface = SDL_ConvertSurface(screenSurface, screenSurface->format, SDL_SWSURFACE);
    loadPieceSurfaces();
    SDL_BlitSurface( currentSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);
    numberSurface = loadSurface("img/board_ss.bmp");
}

void Interface::renderQuit(){
    SDL_DestroyWindow(window);

    SDL_FreeSurface(currentSurface);
    currentSurface = NULL;

    //Free piece surfaces
    for(int i = Game::Piece::WHITE_PAWN; i<=Game::Piece::WHITE_KING; i++){
        SDL_FreeSurface(pieceSurfaces[i]);
    }
    for(int i = Game::Piece::BLACK_PAWN; i<=Game::Piece::BLACK_KING; i++){
        SDL_FreeSurface(pieceSurfaces[i]);
    }

    SDL_Quit();
}

Move Interface::handlePlayerInput(uint64_t renderBB){
    std::array<Move, 220> moves;
    int numMoves = game.generateMoves(moves);

    SDL_Event e;
    int holding = -1;
    int mx, my;
    Move moveFound = Move {64,64, Move::Promo::NONE };

    while(moveFound.src==64){



    
        renderBoard(holding, renderBB);
        while(SDL_PollEvent(&e)){
            SDL_GetMouseState(&mx,&my);

            if(e.type==SDL_QUIT){
                renderQuit();
                exit(0);
            } else if(e.type==SDL_MOUSEBUTTONDOWN && holding == -1){
                if(mx < XMARG() || my < YMARG() || mx > SWIDTH-XMARG() || my > SHEIGHT-YMARG() ) continue;

                holding = ((mx-XMARG()) / SQUARE_WIDTH) + 8* ( (my-YMARG()) / SQUARE_WIDTH);
                if(game.boardArr[holding]==0) holding = -1;

            } else if(e.type==SDL_MOUSEBUTTONUP && holding != -1 ){
                if(mx < XMARG() || my < YMARG() || mx > SWIDTH-XMARG() || my > SHEIGHT-YMARG() || ((mx-XMARG()) / SQUARE_WIDTH) + 8* ( (my-YMARG()) / SQUARE_WIDTH)==holding){
                    holding = -1;
                } else {
                    int trySrc = holding;
                    int tryDest = ((mx-XMARG()) / SQUARE_WIDTH) + 8* ( (my-YMARG()) / SQUARE_WIDTH);
                    
                    for(int i = 0; i<numMoves; i++){
                        if(moves[i].src == trySrc && moves[i].dest == tryDest){
                            moveFound.src = holding;
                            moveFound.dest = ((mx-XMARG()) / SQUARE_WIDTH) + 8* ( (my-YMARG()) / SQUARE_WIDTH);

                            if((moveFound.dest < 8 || moveFound.dest >=56) && (game.boardArr[moveFound.src] == Game::WHITE_PAWN || game.boardArr[moveFound.src] == Game::BLACK_PAWN)){
                                int promoInp;
                                std::cout << "Enter piece to promote to, 1-Knight, 2-Bishop, 3-Rook, 4-Queen: ";

                                int promoInput{0};
                                std::cin >> promoInput;
                                while(promoInput > 4 || promoInput < 1){
                                    std::cout << "Invalid input\n";
                                    std::cin >> promoInput;
                                }
                                moveFound.promo = static_cast<Move::Promo>(promoInput);
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