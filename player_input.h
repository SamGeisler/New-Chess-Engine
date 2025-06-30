#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

//Called in main after AI moves - stalls entire program until input is received (handles quitting window and eventually other inputs as well)
move handle_player_input(uint64_t render_bb);

#endif