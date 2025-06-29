/*
TODO: Complete move generation
    X Ensure legality
    X Implement en passant
    X Implement castling
    X Look into and decide about drawing
    - Game end
    - Test move generation
*/
/*
Tentative approach to draws:
- Ignore repetitions until a transposition table is implemented, see if it is easy/feasible then
- No way to offer a draw/resign
- Implement stalemate and FMR.
*/
#include "SDL.h"
#include "global.h"
#include "precompute.h"
#include "render.h"
#include "control.h"
#include "generate_moves.h"
//#include "search.h"

int AICOLOR;
double PIECE_VALUES[5] = {1.0, 3.0, 3.0, 5.0, 9.0};

unsigned int board_arr[64];
board_t board;
metadata_t MD;

void init_board(char* board_init);//Initialize board_arr and board struct from FEN string
void update_board_arr();//Restores board_arr from bitboards
void update_bb();//Restore bitboards from board_rr
void print_board_arr();

int main(int argc, char* argv[]){
    load_precompute();
    init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    render_init();
    render_board(-1,0);

    move* moves = (move*)malloc(sizeof(move)*220);

    while(1){
        //printf("Castling rights: %d %d %d %d\n",!!(MD.castle_flags&8),!!(MD.castle_flags&4),!!(MD.castle_flags&2),!!(MD.castle_flags&1));
        /*printf("\nWHITE MOVES:\n");
        for(int i = 0; i<generate_moves(moves,WHITE); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }*/
        AICOLOR = BLACK;
        move pmove = handle_player_input(get_pinned(bitScanForward(board.bitboards[WHITE] & board.bitboards[KING]), WHITE));
        execute_move(pmove);

        /*printf("\nBLACK MOVES:\n");
        for(int i = 0; i<generate_moves(moves,BLACK); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }*/
        //print_bb(board.bitboards[KING]);
        //print_bb(board.bitboards[PAWN]);
        AICOLOR = WHITE;
        pmove = handle_player_input(get_pinned(bitScanForward(board.bitboards[BLACK] & board.bitboards[KING]), BLACK));
        execute_move(pmove);
    
    } 

    free(moves);

    render_quit();
    return 0;
}


void init_board(char* board_init){
    //Initialize metadata
    MD.castle_flags = 0xF;
    MD.ep_right = 0;

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
                board_arr[pos] = i;
            }
            current_bb >>=1;
            pos++;
        }

        current_bb = board.bitboards[i] & board.bitboards[BLACK];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i+8;
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
            board.bitboards[ board_arr[i]>8 ] |= SHIFT(i);//Color
            board.bitboards[ board_arr[i]%8 ] |= SHIFT(i);//Piece
        }
    }
}

void print_board_arr(){
    char c;
    for(int i = 0; i<64; i++){
        switch(board_arr[i]){
            case EMPTY: c = '.'; break;
            case WHITE_PAWN: c='P';  break;
            case WHITE_KNIGHT: c='N'; break;
            case WHITE_BISHOP: c='B'; break;
            case WHITE_ROOK: c='R'; break;
            case WHITE_QUEEN: c='Q'; break;
            case WHITE_KING: c='K'; break;
            case BLACK_PAWN: c='p'; break;
            case BLACK_KNIGHT: c='n'; break;
            case BLACK_BISHOP: c='b'; break;
            case BLACK_ROOK: c='r'; break;
            case BLACK_QUEEN: c='q'; break;
            case BLACK_KING: c='k'; break;
        }

        printf("%c ",c);
        if(i%8==7) printf("\n");
    }
}