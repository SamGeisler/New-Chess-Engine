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
    init_board("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1");

    render_init();
    render_board(-1,0);

    move* moves = (move*)malloc(sizeof(move)*220);

    while(1){
        print_metadata();
        int num_moves = generate_moves(moves,MD.to_move);
        printf("\n%d MOVES:\n", num_moves);
        for(int i = 0; i<num_moves; i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }
        AICOLOR = 1-MD.to_move;
        move pmove = handle_player_input(0);
        execute_move(pmove);
    
    } 

    free(moves);

    render_quit();
    return 0;
}

