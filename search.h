#ifndef SEARCH_H
#define SEARCH_H

double eval(int color, Move* bestMove, int depth);

Move quiescenceEval();

double staticEval();


#endif