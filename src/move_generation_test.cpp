#include "game.h"
#include "move_generation_test.h"
#include "precomputed.h"

#include <cmath>
#include <iostream>
#include <ctime>
#include <cstring>
#include <iomanip>


int main(int argc, char *argv[]){
    Precomputed::loadPrecompute();

    MoveGenerationTest moveGenerationTest;

    moveGenerationTest.perftDebug("4k2r/6K1/8/8/8/8/8/8 b k - 0 1",2);

    moveGenerationTest.runTestSuite();

    return 0;
}

void MoveGenerationTest::perftDebug(std::string_view fen, int depth){
    game.initBoard(fen);
    std::array<Move, 220> moves;
    int numMoves = game.generateMoves(moves);
    
    std::cout << numMoves << " moves.\n";

    int total = 0;
    Game::Metadata backupMD;

    for(int i = 0; i<numMoves; i++){
        backupMD = game.metadata;
        Game::Piece destSquare = game.boardArr[moves[i].dest];
        game.executeMove(moves[i]);

  
        std::cout << moves[i] << " -> " << perft(1,depth-1) << " moves.\n";       

        game.metadata = backupMD;
        game.unexecuteMove(moves[i], destSquare);
    }
}

void MoveGenerationTest::runTestSuite(){

    std::cout << std::setprecision(3);

    int testSuccesful = 1;
    FILE* fh = fopen("perftsuite.txt","r");
    if(fh==NULL){
        std::cout << "Test aborted: Could not open perftsuite.txt\n";
        return;
    }

    clock_t startClock = clock();
    for(int i = 0; i<125; i++){
        clock_t thisStart = clock();
        char fen[85];
        int fenPos = 0;
        char c;
        //read FEN notation
        while((c=fgetc(fh)) != ';'){
            fen[fenPos] = c;
            fenPos++;
        }
        
        char evalsText[6][12];
        int evals[6] = {0,0,0,0,0,0};
        //read each given perft evaluation
        for(int j = 0; j<6; j++){
            fgetc(fh); fgetc(fh); fgetc(fh);

            int k = 0;
            while(1){
                c = fgetc(fh);
                if(c=='\n' || c==' ' || c==EOF){
                    evalsText[j][k] = '\0';
                    break;
                }
                evalsText[j][k] = c;
                k++;
            }
            if(c==' ') fgetc(fh);
        }

        //convert to int
        for(int j = 0; j<6; j++){
            
            int n = strlen(evalsText[j]);

            for(int k = 0; k<n; k++){
                int x = evalsText[j][n-1-k] - '0';
                evals[j] += ( (int)(pow(10,k)+0.5) ) * x;
            }
        }

        game.initBoard(fen);
        std::cout << "board " << i+1 << ":";
        for(int j = 1; j<= 6; j++){
            int p = perft(1,j);
            if(evals[j-1]!=p) testSuccesful = 0;
            std::cout << " " << static_cast<int>((evals[j-1]==p));
        }

        double elapsed = (double)(clock() - thisStart)/CLOCKS_PER_SEC;
        std::cout << ". Time = " << elapsed << "s\n";
    }
    fclose(fh);


    if(testSuccesful){
        std::cout << "\nAll positions correct!\n";
    } else {
        std::cout << "\nTest failed.\n";
    }

    double elapsed = (double)(clock() - startClock)/CLOCKS_PER_SEC;
    int min = (int)elapsed / 60;

    std::cout << "Total time elapsed: " << min << "m " << elapsed-60.0*min << "s\n";
}



int MoveGenerationTest::perft(int depth, int endingDepth){
    std::array<Move, 220> moves;
    int numMoves;

    numMoves = game.generateMoves(moves);

    if(depth==endingDepth){
        return numMoves;
    } else {
        int total = 0;
        Game::Metadata backupMD;
        for(int i = 0; i<numMoves; i++){
            
            backupMD = game.metadata;
            Game::Piece destSquare = game.boardArr[moves[i].dest];
            game.executeMove(moves[i]);

            int perftResult = perft(depth+1, endingDepth);
            total += perftResult;            

            game.metadata = backupMD;
            game.unexecuteMove(moves[i], destSquare);
        }
        return total;
    }
}
