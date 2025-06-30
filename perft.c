#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "global.h"
#include "generate_moves.h"
#include "control.h"
#include "perft.h"


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
        int bup_board[64];
        metadata_t bup_md;
        for(int i = 0; i<num_moves; i++){
            
            metadata_t backup_md = MD;
            int dest_square = board_arr[moves[i].dest];
            execute_move(moves[i]);
            
            total += perft(depth+1, ending_depth, 1-color);

            MD = backup_md;
            unexecute_move(moves[i], dest_square, moves[i].dest == MD.ep_right);
        }
        free(moves);
        return total;
    }
}
