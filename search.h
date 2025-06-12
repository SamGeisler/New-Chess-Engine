#ifndef SEARCH_H
#define SEARCH_H

move eval(int color, move* best_move, int depth);

move quiescence_eval();

double static_eval();


#endif