#ifndef CONTROL_H
#define CONTROL_H

move get_engine_move();

//Update bitboards and board_arr
void execute_move(move m);//update global game state, assumes valid move.
void unexecute_move(move m, int dest_square);//Restore board state but not metadata
/*Move execution/unexcution procedure:

metadata_t backup_md = MD;
int dest_square = board_arr[m.dest];
execute_move(m);
//process
MD = backup_md;
unexecute_move(m, dest_square);

*/

/*Takes is_in_check to avoid repeated work, the color passed is the color to move
Return values:
- 2: Game continuing
- 1: White wins
- 0: Draw
- -1: Black wins*/
int game_end(int color, int is_in_check, int num_moves);

#endif