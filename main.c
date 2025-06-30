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
#include "load_precompute.h"
#include "render.h"
#include "control.h"
#include "generate_moves.h"
#include "player_input.h"
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
    //Starting board: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    init_board("4k3/8/8/8/8/8/8/4K2R w K - 0 1 ;D1 15 ;D2 66 ;D3 1197 ;D4 7059 ;D5 133987 ;D6 764643");

    render_init();
    render_board(-1,0);

    move* moves = (move*)malloc(sizeof(move)*220);

    while(1){
        print_metadata();
        printf("\nWHITE MOVES:\n");
        for(int i = 0; i<generate_moves(moves,WHITE); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }
        AICOLOR = BLACK;
        move pmove = handle_player_input(get_pinned(bitScanForward(board.bitboards[WHITE] & board.bitboards[KING]), WHITE));
        execute_move(pmove);

        print_metadata();
        printf("\nBLACK MOVES:\n");
        for(int i = 0; i<generate_moves(moves,BLACK); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }
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

