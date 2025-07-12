#include "global.h"
#include "control.h"
#include "generateMoves.h"

void executeMove(const Move& m){
    int color = boardArr[m.src]>8;
    int colorMod = (2*color-1);//-1 for WHITE, 1 for BLACK
    int piece = boardArr[m.src]%8;
    int captured = boardArr[m.dest]%8;

    //Update player to move
    MD.toMove = 1-MD.toMove;

    //Update fifty-move-rule count
    if(piece==PAWN || boardArr[m.dest]){
        MD.fmrCount = 0;
    } else {
        MD.fmrCount++;
    }

    //Bitboards: Remove moving piece from source
    board.bitboards[color] &= ~SHIFT(m.src);
    board.bitboards[piece] &= ~SHIFT(m.src);

    //Bitboards: Remove captured piece from destination
    board.bitboards[1-color] &= ~SHIFT(m.dest);
    if(captured) board.bitboards[captured] &= ~SHIFT(m.dest);

    //Check for promotion
    if(m.promo){
        switch(m.promo){
            case 1: piece = KNIGHT; break;
            case 2: piece = BISHOP; break;
            case 3: piece = ROOK; break;
            case 4: piece = QUEEN; break;
        }
    }

    //Update board array
    boardArr[m.dest] = piece + 8*color;
    boardArr[m.src] = EMPTY;

    //Bitboards: Add moving piece to destination
    board.bitboards[color] |= SHIFT(m.dest);
    board.bitboards[piece] |= SHIFT(m.dest);
    
    //Handle enpassant capture
    if(m.dest==MD.epRight && piece==PAWN){
        int capturedSquare = MD.epRight - 8*colorMod;
        board.bitboards[1-color] &= ~SHIFT(capturedSquare);
        board.bitboards[PAWN] &= ~SHIFT(capturedSquare);
        boardArr[capturedSquare] = EMPTY;
    }

    //Update enpassant metadata
    MD.epRight = 0;
    if(piece==PAWN && m.dest == m.src + colorMod*16){
        MD.epRight = m.dest - colorMod*8;
    }

    //Move rook for castling and update castling right for king moves
    if(piece == KING){
        MD.castleFlags &= 0b0011 << (2*color);
        if(m.src == 60 && m.dest == 62){
            boardArr[61] = WHITE_ROOK;
            boardArr[63] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(61);
            board.bitboards[WHITE] |= SHIFT(61);
            board.bitboards[ROOK] ^= SHIFT(63);
            board.bitboards[WHITE] ^= SHIFT(63);
        } else if(m.src == 60 && m.dest == 58){
            boardArr[59] = WHITE_ROOK;
            boardArr[56] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(59);
            board.bitboards[WHITE] |= SHIFT(59);
            board.bitboards[ROOK] ^= SHIFT(56);
            board.bitboards[WHITE] ^= SHIFT(56);
        } else if(m.src == 4 && m.dest == 6){
            boardArr[5] = BLACK_ROOK;
            boardArr[7] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(5);
            board.bitboards[BLACK] |= SHIFT(5);
            board.bitboards[ROOK] ^= SHIFT(7);
            board.bitboards[BLACK] ^= SHIFT(7);
        } else if(m.src == 4 && m.dest == 2){
            boardArr[3] = BLACK_ROOK;
            boardArr[0] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(3);
            board.bitboards[BLACK] |= SHIFT(3);
            board.bitboards[ROOK] ^= SHIFT(0);
            board.bitboards[BLACK] ^= SHIFT(0);
        }
    }

    //Update castling metadata for rook moves/takes
    if(m.src==0 || m.dest==0) MD.castleFlags &= ~1;
    if(m.src==7 || m.dest==7) MD.castleFlags &= ~2;
    if(m.src==56 || m.dest==56) MD.castleFlags &= ~4;
    if(m.src==63 || m.dest==63) MD.castleFlags &= ~8;
}

//Reverts from executeMove - does not fix metadata
//int destSquare - piece located in destination square before move, if any (0=UNUSED1 if none). Bitboard index format (color is not included)
//wasEp - Flag denoting whether this move was to the en passant square
void unexecuteMove(const Move& m, int destSquare){
    int color = boardArr[m.dest]>8;
    int piece = boardArr[m.dest]%8;

    //Bitboards: Remove piece from destination square
    board.bitboards[color] &= ~SHIFT(m.dest);
    board.bitboards[piece] &= ~SHIFT(m.dest);

    //Bitboards: Return captured piece (if any) to destination square
    if(destSquare){
        board.bitboards[1-color] |= SHIFT(m.dest);
        board.bitboards[destSquare%8] |= SHIFT(m.dest);
    }
    
    if(m.promo){
        piece = PAWN;
    }

    //Bitboards: Return piece to starting square
    board.bitboards[color] |= SHIFT(m.src);
    board.bitboards[piece] |= SHIFT(m.src);

    //Board array: Restore
    boardArr[m.src] = piece + 8*color;
    boardArr[m.dest] = destSquare;

    //Undo en-passant capture
    if(MD.epRight && MD.epRight==m.dest && piece==PAWN){ 
        int repairSquare = m.dest + ( 8-16*color );
        boardArr[repairSquare] = PAWN + 8*(1-color);

        board.bitboards[1-color] |= SHIFT(repairSquare);
        board.bitboards[PAWN] |= SHIFT(repairSquare);
    }

    //Undo rook move for castling
    if(piece == KING){
        if(m.src == 60 && m.dest == 62){
            boardArr[63] = WHITE_ROOK;
            boardArr[61] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(63);
            board.bitboards[WHITE] |= SHIFT(63);
            board.bitboards[ROOK] ^= SHIFT(61);
            board.bitboards[WHITE] ^= SHIFT(61);
        } else if(m.src == 60 && m.dest == 58){
            boardArr[56] = WHITE_ROOK;
            boardArr[59] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(56);
            board.bitboards[WHITE] |= SHIFT(56);
            board.bitboards[ROOK] ^= SHIFT(59);
            board.bitboards[WHITE] ^= SHIFT(59);
        } else if(m.src == 4 && m.dest == 6){
            boardArr[7] = BLACK_ROOK;
            boardArr[5] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(7);
            board.bitboards[BLACK] |= SHIFT(7);
            board.bitboards[ROOK] ^= SHIFT(5);
            board.bitboards[BLACK] ^= SHIFT(5);
        } else if(m.src == 4 && m.dest == 2){
            boardArr[0] = BLACK_ROOK;
            boardArr[3] = EMPTY;
            board.bitboards[ROOK] |= SHIFT(0);
            board.bitboards[BLACK] |= SHIFT(0);
            board.bitboards[ROOK] ^= SHIFT(3);
            board.bitboards[BLACK] ^= SHIFT(3);
        }
    }
}

Move getEngineMove(){
    return (Move){0,0,0};
}

//The color passed is the color to move
int gameEnd(int color, int isInCheck, int numMoves){
    if(numMoves == 0){
        if(isInCheck){
            return 1-2*color;
        } else {
            return 0;
        }
    }
    if(MD.fmrCount==100) return 0;
    
    return 2;
}