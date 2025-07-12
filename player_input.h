#ifndef PLAYER_INPUT_H
#define PLAYER_INPUT_H

//Called in main after AI moves - stalls entire program until input is received (handles quitting window and eventually other inputs as well)
Move handlePlayerInput(uint64_t renderBb);

#endif