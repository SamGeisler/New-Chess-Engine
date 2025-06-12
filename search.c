#include "global.h"
#include "search.h"
#include "generate_moves.h"

//White - positive, black - negative
double eval(int color, move* best_move, int depth){
    move* move_arr = (move*)malloc(200*sizeof(move));
    int num_moves = generate_moves(move_arr, color);

    move temp_move;
    double temp_eval;
    metadata_t backup_MD;

    
    double best_eval;
    if(color==WHITE){
        for(int i = 0;i<num_moves; i++){
            backup_MD = MD;
            execute_move(move_arr[i]);

            temp_eval = eval(1-color, &temp_move, depth-1);
            if(best_eval > temp_eval){
                *best_move = move_arr[i];
                best_eval = temp_eval;
            }

            unexecute_move(move_arr[i]);
            MD = backup_MD;
        }
    } else if (color==BLACK){
        for(int i = 0;i<num_moves; i++){
            backup_MD = MD;
            execute_move(move_arr[i]);

            temp_eval = eval(1-color, &temp_move, depth-1);
            if(best_eval < temp_eval){
                *best_move = move_arr[i];
                best_eval = temp_eval;
            }

            unexecute_move(move_arr[i]);
            MD = backup_MD;
        }
    }

    return best_eval;
}

move quiescence_eval(){

}

double static_eval(){

}

