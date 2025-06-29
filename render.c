#include "SDL.h"
#include "global.h"
#include "render.h"

SDL_Window* window = NULL;
SDL_Surface* screen_surface = NULL;
SDL_Surface* current_surface = NULL;
SDL_Surface* number_surface = NULL;

SDL_Surface* piece_surfaces[16];

SDL_Surface* load_surface(char* path){
    SDL_Surface* loaded = SDL_LoadBMP(path);
    SDL_Surface* optimized = SDL_ConvertSurface(loaded, screen_surface -> format, 0);
    SDL_SetColorKey(optimized, SDL_TRUE, SDL_MapRGB(optimized->format, 0x00, 0xFF, 0xFF));
    SDL_FreeSurface(loaded);
    return optimized;
}

void draw_background(uint64_t bb){
    SDL_FillRect(current_surface, NULL, BGCOLOR);
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

            SDL_FillRect(current_surface, &square, color);
        }
    }
    SDL_BlitScaled(number_surface, NULL, current_surface, NULL);

}


void load_piece_surfaces(){
    piece_surfaces[WHITE_PAWN] = load_surface("img/white_pawn.bmp");
    piece_surfaces[WHITE_KNIGHT] = load_surface("img/white_knight.bmp");
    piece_surfaces[WHITE_BISHOP] = load_surface("img/white_bishop.bmp");
    piece_surfaces[WHITE_ROOK] = load_surface("img/white_rook.bmp");
    piece_surfaces[WHITE_QUEEN] = load_surface("img/white_queen.bmp");
    piece_surfaces[WHITE_KING] = load_surface("img/white_king.bmp");
    piece_surfaces[BLACK_PAWN] = load_surface("img/black_pawn.bmp");
    piece_surfaces[BLACK_KNIGHT] = load_surface("img/black_knight.bmp");
    piece_surfaces[BLACK_BISHOP] = load_surface("img/black_bishop.bmp");
    piece_surfaces[BLACK_ROOK] = load_surface("img/black_rook.bmp");
    piece_surfaces[BLACK_QUEEN] = load_surface("img/black_queen.bmp");
    piece_surfaces[BLACK_KING] = load_surface("img/black_king.bmp");
}

void render_board(int holding, uint64_t bb){

    draw_background(bb);
    if(1){
        for(int i = 0; i<64; i++){
            if(board_arr[i] && (holding==-1 || i!=holding)){
                SDL_Rect loc_rect = {XMARG + PS*(i%8), YMARG + PS*(i/8), PS, PS};
                SDL_BlitScaled(piece_surfaces[board_arr[i]], NULL, current_surface, &loc_rect);
            }
        }
    } else {//Later board flipping for black player
        for(int i = 0; i<64; i++){
            if(board_arr[63-i] && (holding==-1 || 63-i!=holding)){
                SDL_Rect loc_rect = {XMARG + PS*(i%8), YMARG + PS*(i/8), PS, PS};
                SDL_BlitScaled(piece_surfaces[board_arr[63-i]], NULL, current_surface, &loc_rect);
            }
        }
    }
    if(holding != -1){
        int hx, hy; SDL_GetMouseState(&hx, &hy);
        SDL_Rect holding_loc_rect = {hx-PS/2, hy-PS/2, PS, PS};
        SDL_BlitScaled(piece_surfaces[board_arr[holding]], NULL, current_surface, &holding_loc_rect);
    }
    SDL_BlitScaled(current_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);

}

void render_init(){
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
	}
    
    window = SDL_CreateWindow("Chess Display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SWIDTH, SHEIGHT, SDL_WINDOW_SHOWN);
    if(FULLSCREEN) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    
    screen_surface = SDL_GetWindowSurface(window);


    current_surface = SDL_ConvertSurface(screen_surface, screen_surface->format, SDL_SWSURFACE);
    load_piece_surfaces();
    SDL_BlitSurface( current_surface, NULL, screen_surface, NULL);
    SDL_UpdateWindowSurface(window);
    number_surface = load_surface("img/board_ss.bmp");
}

void render_quit(){
    SDL_DestroyWindow(window);

    SDL_FreeSurface(current_surface);
    current_surface = NULL;

    //Free piece surfaces
    for(int i = WHITE_PAWN; i<=BLACK_KING; i++){
        if(i==NULL_PIECE2 || i==NULL_PIECE3) continue;
        SDL_FreeSurface(piece_surfaces[i]);
    }

    SDL_Quit();
}