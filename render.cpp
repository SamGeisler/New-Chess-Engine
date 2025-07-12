#include "SDL.h"
#include <stringView>

#include "global.h"
#include "render.h"

SDL_Window* window = NULL;
SDL_Surface* screenSurface = NULL;
SDL_Surface* currentSurface = NULL;
SDL_Surface* numberSurface = NULL;

SDL_Surface* pieceSurfaces[16];

SDL_Surface* loadSurface(const char * path){
    SDL_Surface* loaded = SDL_LoadBMP(path);
    SDL_Surface* optimized = SDL_ConvertSurface(loaded, screenSurface -> format, 0);
    SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0x00, 0xFF, 0xFF));
    SDL_FreeSurface(loaded);
    return optimized;
}

void drawBackground(uint64_t bb){
    SDL_FillRect(currentSurface, NULL, BGCOLOR);
    SDL_Rect square = {0,0,PS,PS};
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            square.x = XMARG + j*PS;
            square.y = YMARG + i*PS;

            int color;
            if(bb & SHIFT(8*i +j))
                color =  ((i+j)%2==0 ? LIGHTATTACKING : DARKATTACKING);
            else 
                color = ((i+j)%2==0 ? LIGHTSPACECOLOR : DARKSPACECOLOR);

            SDL_FillRect(currentSurface, &square, color);
        }
    }
    SDL_BlitScaled(numberSurface, NULL, currentSurface, NULL);

}


void loadPieceSurfaces(){
    pieceSurfaces[WHITE_PAWN] = loadSurface("img/whitePawn.bmp");
    pieceSurfaces[WHITE_KNIGHT] = loadSurface("img/whiteKnight.bmp");
    pieceSurfaces[WHITE_BISHOP] = loadSurface("img/whiteBishop.bmp");
    pieceSurfaces[WHITE_ROOK] = loadSurface("img/whiteRook.bmp");
    pieceSurfaces[WHITE_QUEEN] = loadSurface("img/whiteQueen.bmp");
    pieceSurfaces[WHITE_KING] = loadSurface("img/whiteKing.bmp");
    pieceSurfaces[BLACK_PAWN] = loadSurface("img/blackPawn.bmp");
    pieceSurfaces[BLACK_KNIGHT] = loadSurface("img/blackKnight.bmp");
    pieceSurfaces[BLACK_BISHOP] = loadSurface("img/blackBishop.bmp");
    pieceSurfaces[BLACK_ROOK] = loadSurface("img/blackRook.bmp");
    pieceSurfaces[BLACK_QUEEN] = loadSurface("img/blackQueen.bmp");
    pieceSurfaces[BLACK_KING] = loadSurface("img/blackKing.bmp");
}

void renderBoard(int holding, uint64_t bb){

    drawBackground(bb);
    if(1){
        for(int i = 0; i<64; i++){
            if(boardArr[i] && (holding==-1 || i!=holding)){
                SDL_Rect locRect = {XMARG + PS*(i%8), YMARG + PS*(i/8), PS, PS};
                SDL_BlitScaled(pieceSurfaces[boardArr[i]], NULL, currentSurface, &locRect);
            }
        }
    } else {//Later board flipping for black player
        for(int i = 0; i<64; i++){
            if(boardArr[63-i] && (holding==-1 || 63-i!=holding)){
                SDL_Rect locRect = {XMARG + PS*(i%8), YMARG + PS*(i/8), PS, PS};
                SDL_BlitScaled(pieceSurfaces[boardArr[63-i]], NULL, currentSurface, &locRect);
            }
        }
    }
    if(holding != -1){
        int hx, hy; SDL_GetMouseState(&hx, &hy);
        SDL_Rect holdingLocRect = {hx-PS/2, hy-PS/2, PS, PS};
        SDL_BlitScaled(pieceSurfaces[boardArr[holding]], NULL, currentSurface, &holdingLocRect);
    }
    SDL_BlitScaled(currentSurface, NULL, screenSurface, NULL);
    SDL_UpdateWindowSurface(window);

}

void renderInit(){
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
    numberSurface = loadSurface("img/boardSs.bmp");
}

void renderQuit(){
    SDL_DestroyWindow(window);

    SDL_FreeSurface(currentSurface);
    currentSurface = NULL;

    //Free piece surfaces
    for(int i = WHITE_PAWN; i<=BLACK_KING; i++){
        if(i==NULL_PIECE2 || i==NULL_PIECE3) continue;
        SDL_FreeSurface(pieceSurfaces[i]);
    }

    SDL_Quit();
}