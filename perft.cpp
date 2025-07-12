#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include <array>

#include "global.h"
#include "loadPrecompute.h"
#include "generateMoves.h"
#include "control.h"
#include "perft.h"

int AICOLOR;
double PIECE_VALUES[5] = {1.0, 3.0, 3.0, 5.0, 9.0};

unsigned int boardArr[64];
Board board;
Metadata MD;

void perftDebug(std::stringView fen, int depth);

int main(int argc, char *argv[]){
    loadPrecompute();
    //perftDebug("1r2k2r/8/8/8/8/8/8/R3K2R w KQk - 0 1", 2);

    runTestSuite();

    return 0;
}

void perftDebug(std::stringView fen, int depth){
    initBoard(fen);
    std::array<Move, 220> moves;
    int numMoves = generateMoves(moves, MD.toMove);
    
    std::cout << numMoves << " moves.\n";

    int total = 0;
    Metadata backupMd;

    for(int i = 0; i<numMoves; i++){
        backupMd = MD;
        int destSquare = boardArr[moves[i].dest];
        executeMove(moves[i]);

  
        std::cout << moves[i] << " -> " << perft(1,depth-1) << " moves.\n";       

        MD = backupMd;
        unexecuteMove(moves[i], destSquare);
    }
}

void runTestSuite(){

    std::cout << std::setprecision(3);

    int testSuccesful = 1;
    FILE* fh = fopen("perftsuite.txt","r");
    if(fh==NULL){
        std::cout << "Test aborted: Could not open perftsuite.txt\n";
        return;
    }

    clockT startClock = clock();
    for(int i = 0; i<125; i++){
        clockT thisStart = clock();
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

        initBoard(fen);
        std::cout << "board " << i+1 << ":";
        for(int j = 1; j<= 6; j++){
            int p = perft(1,j);
            if(evals[j-1]!=p) testSuccesful = 0;
            std::cout << " " << staticCast<int>((evals[j-1]==p));
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


int perft(int depth, int endingDepth){
    std::array<Move, 220> moves;
    int numMoves;

    numMoves = generateMoves(moves, MD.toMove);

    if(depth==endingDepth){
        return numMoves;
    } else {
        int total = 0;
        Metadata backupMd;
        for(int i = 0; i<numMoves; i++){
            
            backupMd = MD;
            int destSquare = boardArr[moves[i].dest];
            executeMove(moves[i]);

            int perftResult = perft(depth+1, endingDepth);
            total += perftResult;            

            MD = backupMd;
            unexecuteMove(moves[i], destSquare);
        }
        return total;
    }
}
