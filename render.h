#ifndef RENDER_H
#define RENDER_H

void render_init();
void render_update();
void render_quit();

void render_board(int holding);//Value of held piece (-1 if no piece held). If so, this function gets the mouse location and renders the floating piece there

#endif