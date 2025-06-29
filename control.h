#ifndef CONTROL_H
#define CONTROL_H

#include "global.h"
#include "render.h"


//Called in main after AI moves - stalls entire program until input is received (handles quitting window and eventually other inputs as well)
move handle_player_input(uint64_t render_bb);

move get_engine_move();

//Update bitboards and board_arr
void execute_move(move m);//update global game state, assumes valid move.
void unexecute_move(move m);//Restore board state but not metadata

/*Takes is_in_check to avoid repeated work, the color passed is the color to move
Return values:
- 2: Game continuing
- 1: White wins
- 0: Draw
- -1: Black wins*/
int game_end(int color, int is_in_check, int num_moves);

#endif