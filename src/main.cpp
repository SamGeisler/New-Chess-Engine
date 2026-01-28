#include "game.h"
#include "engine.h"
#include "precomputed.h"
#include "interface.h"


int main(int argc, char* argv[]){
    Precomputed::loadPrecompute();
    Interface interface;
    Engine engine(interface.game);

    interface.renderBoard(-1, 0);
    std::cout << "THIS IS A TEST." << std::endl;
    while(1){
        Move move = interface.handlePlayerInput(0);
        interface.game.executeMove(move);
        interface.renderBoard(-1, 0);

        move = engine.getMove();
        interface.game.executeMove(move);
    }

    interface.renderQuit();
    return 0;
}