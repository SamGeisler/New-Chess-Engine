#include "global.h"
#include "search.h"
#include "generateMoves.h"
#include "control.h"

/*
//White - positive, black - negative
double eval(int color, Move* bestMove, int depth){
    std::array<Move, 220> moveArr;
    int numMoves = generateMoves(moveArr, color);

    Move tempMove;
    double tempEval;
    Metadata backup_MD;

    
    double bestEval;
    if(color==WHITE){
        for(int i = 0;i<numMoves; i++){
            backup_MD = MD;
            executeMove(moveArr[i]);

            tempEval = eval(1-color, &tempMove, depth-1);
            if(bestEval > tempEval){
                *bestMove = moveArr[i];
                bestEval = tempEval;
            }

            unexecuteMove(moveArr[i]);
            MD = backup_MD;
        }
    } else if (color==BLACK){
        for(int i = 0;i<numMoves; i++){
            backup_MD = MD;
            executeMove(moveArr[i]);

            tempEval = eval(1-color, &tempMove, depth-1);
            if(bestEval < tempEval){
                *bestMove = moveArr[i];
                bestEval = tempEval;
            }

            unexecuteMove(moveArr[i]);
            MD = backup_MD;
        }
    }

    return bestEval;
}

move quiescenceEval(){

}

double staticEval(){

}


*/