#ifndef RENDER_H
#define RENDER_H

void renderInit();
void renderUpdate();
void renderQuit();

void renderBoard(int holding, uint64_t bb);//Value of held piece (-1 if no piece held). If so, this function gets the mouse location and renders the floating piece there

#endif