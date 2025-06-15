#include "SDL.h"
#include "bb_utils.h"
#include "global.h"
#include "render.h"
#include "control.h"
#include "generate_moves.h"
//#include "search.h"

uint64_t knightDestLookup[64] = {0x20400, 0x50800, 0xA1100, 0x142200, 0x284400, 0x508800, 0xA01000, 0x402000, 0x2040004, 0x5080008, 0xA110011, 0x14220022, 0x28440044, 0x50880088, 0xA0100010, 0x40200020, 0x204000402, 0x508000805, 0xA1100110A, 0x1422002214, 0x2844004428, 0x5088008850, 0xA0100010A0, 0x4020002040, 0x20400040200, 0x50800080500, 0xA1100110A00, 0x142200221400, 0x284400442800, 0x508800885000, 0xA0100010A000, 0x402000204000, 0x2040004020000, 0x5080008050000, 0xA1100110A0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xA0100010A00000, 0x40200020400000, 0x204000402000000, 0x508000805000000, 0xA1100110A000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000, 0x400040200000000, 0x800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000, 0x4020000000000, 0x8050000000000, 0x110A0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10A00000000000, 0x20400000000000};
uint64_t kingDestLookup[64] = {0x302, 0x705, 0xE0A, 0x1C14, 0x3828, 0x7050, 0xE0A0, 0xC040, 0x30203, 0x70507, 0xE0A0E, 0x1C141C, 0x382838, 0x705070, 0xE0A0E0, 0xC040C0, 0x3020300, 0x7050700, 0xE0A0E00, 0x1C141C00, 0x38283800, 0x70507000, 0xE0A0E000, 0xC040C000, 0x302030000, 0x705070000, 0xE0A0E0000, 0x1C141C0000, 0x3828380000, 0x7050700000, 0xE0A0E00000, 0xC040C00000, 0x30203000000, 0x70507000000, 0xE0A0E000000, 0x1C141C000000, 0x382838000000, 0x705070000000, 0xE0A0E0000000, 0xC040C0000000, 0x3020300000000, 0x7050700000000, 0xE0A0E00000000, 0x1C141C00000000, 0x38283800000000, 0x70507000000000, 0xE0A0E000000000, 0xC040C000000000, 0x302030000000000, 0x705070000000000, 0xE0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000, 0x203000000000000, 0x507000000000000, 0xA0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000};
void init_board(char* board_init);//Initialize board_arr and board struct from FEN string

void update_board_arr();//Update board_arr from bitboards
void update_bb();//Update borad.bitboards from board_arr


void print_board_arr();


int main(int argc, char* argv[]){

    init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    render_init();
    render_board(-1);


    move* moves = (move*)malloc(sizeof(move)*220);

    while(1){
        //print_bb(board.bitboards[PAWN] & board.bitboards[BLACK]);
        printf("WHITE MOVES\n");
        for(int i = 0; i<generate_moves(moves, WHITE); i++) printf("%d -> %d, promo %d\n",moves[i].src,moves[i].dest,moves[i].promo);
        printf("\n\nBLACK MOVES\n");
        for(int i = 0; i<generate_moves(moves, BLACK); i++) printf("%d -> %d, promo %d\n",moves[i].src,moves[i].dest,moves[i].promo);
        printf("\n\n");
        

        execute_move(handle_player_input());
        update_bb();
    } 

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
            board.bitboards[board_arr[i]>=7] |= SHIFT(i);//Color
            board.bitboards[((board_arr[i]-1)%6) +2] |= SHIFT(i);//Piece
        }
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