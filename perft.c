/*
TODO:
Figure out a better move generation debugging workflow/implementation. 
Maybe something like a perft_debug function that take in a target depth and FEN string
and gives the same output as stockfish. I also need to verify the notation() method
in board_utils, and should probably just brush up on C strings in general.
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "load_precompute.h"
#include "generate_moves.h"
#include "control.h"
#include "perft.h"

int AICOLOR;
double PIECE_VALUES[5] = {1.0, 3.0, 3.0, 5.0, 9.0};

unsigned int board_arr[64];
board_t board;
metadata_t MD;

void perft_debug(char* fen, int depth);

int main(int argc, char *argv[]){
    load_precompute();
    perft_debug("r3k2r/8/8/8/8/8/8/1R2K2R b Kkq - 0 1", 2);

    //run_test_suite();

    return 0;
}

void perft_debug(char* fen, int depth){
    init_board(fen);
    move* moves = malloc(220*sizeof(move));
    int num_moves = generate_moves(moves, MD.to_move);
    printf("%d moves:\n",num_moves);
    int total = 0;
    metadata_t backup_md;
    for(int i = 0; i<num_moves; i++){
        backup_md = MD;
        int dest_square = board_arr[moves[i].dest];
        execute_move(moves[i]);

        char out1[3] = "  \0"; 
        char out2[3] = "  \0";
        notation(moves[i].src, out1);
        notation(moves[i].dest, out2);
        
        printf("%s -> %s: %d moves\n", out1, out2, perft(1,depth-1,MD.to_move));        

        MD = backup_md;
        unexecute_move(moves[i], dest_square, moves[i].dest == MD.ep_right);
    }
}

void run_test_suite(){
    int test_succesful = 1;
    FILE* fh = fopen("perftsuite.txt","r");
    if(fh==NULL){
        printf("Could not open perftsuite.txt.\n");
        return;
    }

    clock_t start_clock = clock();
    for(int i = 0; i<125; i++){
        clock_t this_start = clock();
        char fen[85];
        int fen_pos = 0;
        char c;
        //read FEN notation
        while((c=fgetc(fh)) != ';'){
            fen[fen_pos] = c;
            fen_pos++;
        }
        
        char evals_text[6][12];
        int evals[6] = {0,0,0,0,0,0};
        //read each given perft evaluation
        for(int j = 0; j<6; j++){
            fgetc(fh); fgetc(fh); fgetc(fh);

            int k = 0;
            while(1){
                c = fgetc(fh);
                if(c=='\n' || c==' ' || c==EOF){
                    evals_text[j][k] = '\0';
                    break;
                }
                evals_text[j][k] = c;
                k++;
            }
            if(c==' ') fgetc(fh);
        }
        //convert to int
        for(int j = 0; j<6; j++){
            int n = strlen(evals_text[j]);
            for(int k = 0; k<n; k++){
                int x = evals_text[j][n-1-k] - '0';
                evals[j] += ( (int)(pow(10,k)+0.5) ) * x;
            }
        }

        init_board(fen);
        printf("board %d:",i+1);
        for(int j = 1; j<= 6; j++){
            int p = perft(1,j,MD.to_move);
            if(evals[j-1]!=p) test_succesful = 0;
            printf(" %d",evals[j-1]==p);
        }
        double elapsed = (double)(clock() - this_start)/CLOCKS_PER_SEC;
        printf(". Time = %.3fs\n",elapsed);
    }
    fclose(fh);


    if(test_succesful){
        printf("\nAll positions correct!\n");
    } else {
        printf("\n Test failed.\n");
    }
    double elapsed = (double)(clock() - start_clock)/CLOCKS_PER_SEC;
    int min = (int)elapsed / 60;
    printf("Total time elapsed: %dm %.3f seconds\n",min, elapsed-min);
}


int perft(int depth, int ending_depth, int color){
    move* moves = malloc(220*sizeof(move)); int num_moves;
    num_moves = generate_moves(moves, color);
    if(depth==ending_depth){
        free(moves);
        return num_moves;
    } else {
        int total = 0;
        metadata_t backup_md;
        for(int i = 0; i<num_moves; i++){
            
            backup_md = MD;
            int dest_square = board_arr[moves[i].dest];
            execute_move(moves[i]);

            int perft_result = perft(depth+1, ending_depth, 1-color);
            total += perft_result;            

            MD = backup_md;
            unexecute_move(moves[i], dest_square, moves[i].dest == MD.ep_right);
        }
        free(moves);
        return total;
    }
}
