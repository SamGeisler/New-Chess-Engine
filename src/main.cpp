#include "game.h"
#include "precomputed.h"
#include "lookup.h"
#include "interface.h"


int main(int argc, char* argv[]){
    Precomputed::loadPrecompute();
    Interface interface;
    

    interface.game.initBoard("4k2r/6K1/8/8/8/8/8/8 b k - 0 1");

    interface.renderInit();
    interface.renderBoard(-1, 0);

    std::array<Move, 220> moves;

    while(1){
        interface.game.printMetadata();
        int numMoves = interface.game.generateMoves(moves);
        std::cout << "\n" << numMoves << " MOVES: " << "\n";
        
        for(int i = 0; i<numMoves; i++){
            std::cout << moves[i] << "\n";
        }

        //interface.game.AICOLOR = 1-interface.game.metadata.toMove;

        Move pmove = interface.handlePlayerInput(0);
        interface.game.executeMove(pmove);
    }

    interface.renderQuit();
    return 0;
}

