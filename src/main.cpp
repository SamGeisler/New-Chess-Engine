#include "game.h"
#include "engine.h"
#include "precomputed.h"
#include "lookup.h"
#include "interface.h"


int main(int argc, char* argv[]){
    Precomputed::loadPrecompute();
    Interface interface;
    Engine engine(interface.game);

    interface.renderBoard(-1, 0);

    while(1){
        Move move = interface.handlePlayerInput(0);
        interface.game.executeMove(move);

        move = engine.getMove();
        interface.game.executeMove(move);
    }

    interface.renderQuit();
    return 0;
}

