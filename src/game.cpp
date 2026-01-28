#include "game.h"
#include "lookup.h"

#include <iostream>

Game::Game(int _playerColor, std::string_view _initBoard) : 
    playerColor(_playerColor) {
    initBoard(_initBoard);
}

//The color passed is the color to move
Game::EndFlag Game::gameEnd(int color, int isInCheck, int numMoves){
    if(numMoves == 0){
        if(isInCheck){
            return (color == WHITE) ? BLACK_WINS : WHITE_WINS;
        } else {
            return DRAW;
        }
    }
    if(metadata.fmrCount==100) return DRAW;
    
    return NO_END;
}

void Game::initBoard(std::string_view boardInit){
    //Initialize metadata
    metadata.castleFlags = 0xF;
    metadata.epRight = 0;

    for(int i = 0;i<64; i++) boardArr[i] = EMPTY;
    for(int i = WHITE; i<=KING; ++i) bitboards[i] = 0;

    int i = 0;
    int pos = 0;
    while(boardInit[i]!=' '){
        // piece
        if(boardInit[i] <=122 && boardInit[i] >=65){
            char p = boardInit[i];
            bitboards[ (p>'a') ? BLACK : WHITE ] |= SHIFT(pos);
            switch (p){
                case 'K': 
                    boardArr[pos] = WHITE_KING; 
                    bitboards[KING] |= SHIFT(pos);
                    break;
                case 'Q': 
                    boardArr[pos] = WHITE_QUEEN; 
                    bitboards[QUEEN] |= SHIFT(pos);
                    break;
                case 'R': 
                    boardArr[pos] = WHITE_ROOK; 
                    bitboards[ROOK] |= SHIFT(pos);
                    break;
                case 'B': 
                    boardArr[pos] = WHITE_BISHOP; 
                    bitboards[BISHOP] |= SHIFT(pos);
                    break;
                case 'N': 
                    boardArr[pos] = WHITE_KNIGHT; 
                    bitboards[KNIGHT] |= SHIFT(pos);
                    break;
                case 'P': 
                    boardArr[pos] = WHITE_PAWN; 
                    bitboards[PAWN] |= SHIFT(pos);
                    break;
                case 'k': 
                    boardArr[pos] = BLACK_KING; 
                    bitboards[KING] |= SHIFT(pos);
                    break;
                case 'q': 
                    boardArr[pos] = BLACK_QUEEN; 
                    bitboards[QUEEN] |= SHIFT(pos);
                    break;
                case 'r': 
                    boardArr[pos] = BLACK_ROOK; 
                    bitboards[ROOK] |= SHIFT(pos);
                    break;
                case 'b': 
                    boardArr[pos] = BLACK_BISHOP; 
                    bitboards[BISHOP] |= SHIFT(pos);
                    break;
                case 'n': 
                    boardArr[pos] = BLACK_KNIGHT; 
                    bitboards[KNIGHT] |= SHIFT(pos);
                    break;
                case 'p': 
                    boardArr[pos] = BLACK_PAWN; 
                    bitboards[PAWN] |= SHIFT(pos);
                    break;
            }
            
            
            pos++;
        }   
        //number
        else if(boardInit[i] <= 57 && boardInit[i] >= 48){
            pos += boardInit[i] - '0';
        }

        i++;
    }
    i++;
    //player to move
    metadata.toMove = boardInit[i]=='b';

    i+=2;
    //castle rights
    metadata.castleFlags = 0;
    while(boardInit[i]!=' '){
        if(boardInit[i]=='K'){
            metadata.castleFlags += 8;
        } else if(boardInit[i]=='Q'){
            metadata.castleFlags += 4;
        } else if(boardInit[i]=='k'){
            metadata.castleFlags += 2;
        } else if(boardInit[i]=='q'){
            metadata.castleFlags += 1;
        }
        i++;
    }
    i++;

    //en passant target square
    if(boardInit[i]!='-'){
        metadata.epRight += boardInit[i]-'a';
        i++;
        metadata.epRight += (8-(boardInit[i]-'0'))*8;
    } else {
        metadata.epRight = 0;
    }

}

void Game::printBoardArr(){
    char c;
    for(int i = 0; i<64; i++){
        switch(boardArr[i]){
            case EMPTY: c = '.'; break;
            case WHITE_PAWN: c='P';  break;
            case WHITE_KNIGHT: c='N'; break;
            case WHITE_BISHOP: c='B'; break;
            case WHITE_ROOK: c='R'; break;
            case WHITE_QUEEN: c='Q'; break;
            case WHITE_KING: c='K'; break;
            case BLACK_PAWN: c='p'; break;
            case BLACK_KNIGHT: c='n'; break;
            case BLACK_BISHOP: c='b'; break;
            case BLACK_ROOK: c='r'; break;
            case BLACK_QUEEN: c='q'; break;
            case BLACK_KING: c='k'; break;
            default: c='?'; break;
        }

        std::cout << c << ' ';
        if(i%8==7) std::cout << "\n";
    }
}

void Game::printBB(Game::BBIndex bb){
    std::cout << "Bitboard: 0x" << std::hex << bitboards[bb] << std::dec << ":\n";
    for(int i = 0; i<64; i++){
        std::cout << ((SHIFT(i) & bitboards[bb]) ? 'X' : '.') << " ";
        if(i%8==7) std::cout << "\n";
    }
}

void Game::printBB(uint64_t bb){
    std::cout << "Bitboard: 0x" << std::hex << bb << std::dec << ":\n";
    for(int i = 0; i<64; i++){
        std::cout << ((SHIFT(i) & bb) ? 'X' : '.') << " ";
        if(i%8==7) std::cout << "\n";
    }
}

void Game::printMetadata(){
    std::cout << (metadata.toMove ? "Black" : "White") << " to move.\n";

    std::cout << "Castling rights:";
    std::cout << (!!(metadata.castleFlags&8) ? " WKS " : " - ");
    std::cout << (!!(metadata.castleFlags&4) ? " WQS " : " - ");
    std::cout << (!!(metadata.castleFlags&2) ? " BKS " : " - ");
    std::cout << (!!(metadata.castleFlags&1) ? " BQS " : " - ") << "\n";

    std::cout << "En-passant target square: " << metadata.epRight << "\n";

    std::cout << "Fifty-move rule count (half moves): " << metadata.fmrCount << "\n\n";
}