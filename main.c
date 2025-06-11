#include <stdio.h>
#include <inttypes.h>
#include "SDL.h"
#include "global.h"
#include "render.h"
#include "control.h"

void init_board(char* board_init);//Initialize board_arr and board struct from FEN string

void update_board_arr();//Update board_arr from bitboards
void update_bb();//Update borad.bitboards from board_arr

void print_bb(uint64_t bb);
void print_board_arr();


int main(int argc, char* argv[]){
    init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    render_init();
    render_board(-1);

    while(1) execute_move(handle_player_input());

    render_quit();
    return 0;
}

void init_board(char* board_init){
    for(int i = 0;i<64; i++) board_arr[i] = EMPTY;
    int i = 0;
    int pos = 0;
    while(board_init[i]!=' '){
        // piece
        if(board_init[i] <=122 && board_init[i] >=65){
            char p = board_init[i];
            switch (p){
                case 'K': 
                    board_arr[pos] = WHITE_KING; 
                    break;
                case 'Q': 
                    board_arr[pos] = WHITE_QUEEN; 
                    break;
                case 'R': 
                    board_arr[pos] = WHITE_ROOK; 
                    break;
                case 'B': 
                    board_arr[pos] = WHITE_BISHOP; 
                    break;
                case 'N': 
                    board_arr[pos] = WHITE_KNIGHT; 
                    break;
                case 'P': 
                    board_arr[pos] = WHITE_PAWN; 
                    break;
                case 'k': 
                    board_arr[pos] = BLACK_KING; 
                    break;
                case 'q': 
                    board_arr[pos] = BLACK_QUEEN; 
                    break;
                case 'r': 
                    board_arr[pos] = BLACK_ROOK; 
                    break;
                case 'b': 
                    board_arr[pos] = BLACK_BISHOP; 
                    break;
                case 'n': 
                    board_arr[pos] = BLACK_KNIGHT; 
                    break;
                case 'p': 
                    board_arr[pos] = BLACK_PAWN; 
                    break;
            }
            pos++;
        }   
        //number
        else if(board_init[i] <= 57 && board_init[i] >= 48){
            pos += board_init[i] - '0';
        }

        i++;
    }



    update_bb();
}

void update_board_arr(){
    for(int i = 0;i<64; i++) board_arr[i] = EMPTY;
    uint64_t current_bb;
    int pos;
    for(int i = PAWN; i<=KING; i++){
        current_bb = board.bitboards[i] & board.bitboards[WHITE];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i-1;
            }
            current_bb >>=1;
            pos++;
        }

        current_bb = board.bitboards[i] & board.bitboards[BLACK];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i+5;
            }
            current_bb >>=1;
            pos++;
        }
    }
}

void update_bb(){
    for(int i = 0; i<8; i++) board.bitboards[i] = 0;
    for(int i = 0; i<64; i++){
        if(board_arr[i]){
            //printf("i=%d, b[i]=%d, bb1=%d, bb2=%d\n",i,board_arr[i],board_arr[i]>=7,((board_arr[i]-1)%6)+2);
            board.bitboards[board_arr[i]>=7] |= SHIFT(i);//Color
            board.bitboards[((board_arr[i]-1)%6) +2] |= SHIFT(i);//Piece
        }
    }
}

void print_bb(uint64_t bb){
    printf("Bitboard: %X\n",bb);
    for(int i = 0; i<64; i++){
        printf("%c ", (SHIFT(i) & bb) ? 'X' : '.');
        if(i%8==7) printf("\n");
    }
}

void print_board_arr(){
    char c;
    for(int i = 0; i<64; i++){
        switch(i){
            case WHITE_PAWN: c='P';
            case WHITE_KNIGHT: c='N';
            case WHITE_BISHOP: c='B';
            case WHITE_ROOK: c='R';
            case WHITE_QUEEN: c='Q';
            case WHITE_KING: c='K';
            case BLACK_PAWN: c='p';
            case BLACK_KNIGHT: c='n';
            case BLACK_BISHOP: c='b';
            case BLACK_ROOK: c='r';
            case BLACK_QUEEN: c='q';
            case BLACK_KING: c='k';
        }

        printf("%c ");
        if(i%8==7) printf("\n");
    }
}