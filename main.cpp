/*
Tentative approach to draws:
- Ignore repetitions until a transposition table is implemented, see if it is easy/feasible then
- No way to offer a draw/resign
- Implement stalemate and FMR.
*/

/*C -> C++ todo:
- Redo file input
- Replace printf in loadPrecompute.cpp
- Refactor into OOP design
- Redo macros (most likely into namespaced constants)
- Redo enums (most likely into scoped enums)
*/

#include "SDL.h"

#include "global.h"
#include "loadPrecompute.h"
#include "render.h"
#include "control.h"
#include "generateMoves.h"
#include "playerInput.h"
//#include "search.h"

int AICOLOR;
double PIECE_VALUES[5] = {1.0, 3.0, 3.0, 5.0, 9.0};

unsigned int boardArr[64];
Board board;
Metadata MD;

int main(int argc, char* argv[]){
    loadPrecompute();
    //Starting board: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    initBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    renderInit();
    renderBoard(-1,0);

    std::array<Move, 220> moves;

    while(1){
        printMetadata();
        int numMoves = generateMoves(moves,MD.toMove);
        std::cout << "\n" << numMoves << " MOVES: " << "\n";
        
        for(int i = 0; i<numMoves; i++){
            std::cout << moves[i] << "\n";
        }
        AICOLOR = 1-MD.toMove;

        Move pmove = handlePlayerInput(0);
        executeMove(pmove);
    
    } 

    renderQuit();
    return 0;
}

