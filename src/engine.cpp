#include <array>
#include <limits>

#include "engine.h"
#include "lookup.h"

#define ENGINE_DEBUG 0
//#define VERIFY_HASHING

#define INF std::numeric_limits<double>::infinity()

Move Engine::getMove(){
    minimaxEval(maxSearchDepth, 1-gameRef.playerColor, -INF, INF);
    return bestMove;
}

/*
Negamax implementation - Each player wants to maximize their score, assumes other player wants to minimize
The engine maintains a range of possible scores:
    Lower bound (alpha) - Current player has an alternative move resulting in this score.
    Upper bound (beta) - Opponent player has an alternative move that they will prefer (scores lower)
*/
double Engine::minimaxEval(int depth, int color, double alpha, double beta){
    if(depth == 0)
        return staticEval(color);

    std::array<Move, 220> moves;
    int numMoves = gameRef.generateMoves(moves);

    Game::EndFlag gameEndFlag = gameRef.gameEnd(color, gameRef.isInCheck(color), numMoves);
    if(gameEndFlag != 2){
        if(gameEndFlag){
            return -100000 - depth;
        } else {
            return 0;
        }
    }

    double maxEval = -INF;
    auto bestMove = moves.begin();

    for(auto m = moves.begin(); m<moves.begin() + numMoves; m++){
        Game::Piece destSquare = gameRef.boardArr[m->dest];
        gameRef.executeMove(*m);

        double newEval;
        /*
        TTable::Entry* entry = ttable.getEntry(gameRef.ttableKey, depth-1);

        if(entry != nullptr){
            switch (entry->nodeType){
                case TTable::PV_NODE:
                    std::cout << "PV-node found.\n";
                    newEval = entry->score;
                    break;
                case TTable::CUT_NODE:
                    std::cout << "Cut-node found.\n";

                    alpha = std::max(entry->score, alpha);
                    newEval = -minimaxEval(depth-1, 1-color, -beta, -alpha);
                    break;
                case TTable::ALL_NODE:
                    std::cout << "All-node found.\n";

                    beta = std::min(entry->score, beta);
                    newEval = -minimaxEval(depth-1, 1-color, -beta, -alpha);
                    break;
            }
        } else {
        */
        
        newEval = -minimaxEval(depth-1, 1-color, -beta, -alpha);
        
        gameRef.unexecuteMove(*m, destSquare);  

        if(newEval > maxEval){
            maxEval = newEval;
            bestMove = m;
        }

        if(newEval >= beta){
            //ttable.makeEntry(gameRef.ttableKey, depth, TTable::CUT_NODE, newEval);

            this->bestMove = *bestMove;
            return maxEval;
        }
    }

    //ttable.makeEntry(gameRef.ttableKey, depth, maxEval == -INF ? TTable::ALL_NODE : TTable::PV_NODE, alpha);
    this->bestMove = *bestMove;
    return maxEval;
}



double Engine::staticEval(int color){
    double eval = 0;
    for(int piece = Game::PAWN; piece <= Game::QUEEN; piece++){
        uint64_t whitePieces = gameRef.bitboards[piece] & gameRef.bitboards[Game::WHITE];
        while(whitePieces){
            int src = BitboardOps::bitScanForward(whitePieces);
            whitePieces ^= SHIFT(src);
            eval += PIECE_VALUES[piece];
        }

        uint64_t blackPieces = gameRef.bitboards[piece] & gameRef.bitboards[Game::BLACK];
        while(blackPieces){
            int src = BitboardOps::bitScanForward(blackPieces);
            blackPieces ^= SHIFT(src);
            eval -= PIECE_VALUES[piece];
        }

    }
    
    return eval * (1-2*color);
}

