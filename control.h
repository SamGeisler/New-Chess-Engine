#ifndef CONTROL_H
#define CONTROL_H

#include "global.h"
#include "render.h"


//Called in main after AI moves - stalls entire program until input is received (handles quitting window and eventually other inputs as well)
move handle_player_input();

move get_engine_move();

void execute_move(move m);//update global game state, assumes valid move.
void unexecute_move(move m);//Restore board state but not metadata


#endif