#include "Game.h"
#include "Lookup.h"

void Game::executeMove(const Move& m){
    int color = boardArr[m.src]>8;
    int colorMod = (2*color-1);//-1 for WHITE, 1 for BLACK
    int piece = boardArr[m.src]%8;
    int captured = boardArr[m.dest]%8;

    MD_stack.push(metadata);

    //Update player to move
    metadata.toMove = 1-metadata.toMove;

    //Update fifty-move-rule count
    if(piece==PAWN || boardArr[m.dest]){
        metadata.fmrCount = 0;
    } else {
        metadata.fmrCount++;
    }

    //Bitboards: Remove moving piece from source
    bitboards[color] &= ~SHIFT(m.src);
    bitboards[piece] &= ~SHIFT(m.src);

    //Bitboards: Remove captured piece from destination
    bitboards[1-color] &= ~SHIFT(m.dest);
    if(captured) bitboards[captured] &= ~SHIFT(m.dest);

    //Check for promotion
    switch(m.promo){
        case 1: piece = KNIGHT; break;
        case 2: piece = BISHOP; break;
        case 3: piece = ROOK; break;
        case 4: piece = QUEEN; break;
        default: break;
    }

    //Update board array
    boardArr[m.dest] = static_cast<Piece>(piece + 8*color);
    boardArr[m.src] = EMPTY;

    //Bitboards: Add moving piece to destination
    bitboards[color] |= SHIFT(m.dest);
    bitboards[piece] |= SHIFT(m.dest);
    
    //Handle enpassant capture
    if(m.dest==metadata.epRight && piece==PAWN){
        int capturedSquare = metadata.epRight - 8*colorMod;
        bitboards[1-color] &= ~SHIFT(capturedSquare);
        bitboards[PAWN] &= ~SHIFT(capturedSquare);
        boardArr[capturedSquare] = EMPTY;
    }

    //Update enpassant metadata
    metadata.epRight = 0;
    if(piece==PAWN && m.dest == m.src + colorMod*16){
        metadata.epRight = m.dest - colorMod*8;
    }

    //Move rook for castling and update castling right for king moves
    if(piece == KING){
        metadata.castleFlags &= 0b0011 << (2*color);
        if(m.src == 60 && m.dest == 62){
            boardArr[61] = WHITE_ROOK;
            boardArr[63] = EMPTY;
            bitboards[ROOK] |= SHIFT(61);
            bitboards[WHITE] |= SHIFT(61);
            bitboards[ROOK] ^= SHIFT(63);
            bitboards[WHITE] ^= SHIFT(63);
        } else if(m.src == 60 && m.dest == 58){
            boardArr[59] = WHITE_ROOK;
            boardArr[56] = EMPTY;
            bitboards[ROOK] |= SHIFT(59);
            bitboards[WHITE] |= SHIFT(59);
            bitboards[ROOK] ^= SHIFT(56);
            bitboards[WHITE] ^= SHIFT(56);
        } else if(m.src == 4 && m.dest == 6){
            boardArr[5] = BLACK_ROOK;
            boardArr[7] = EMPTY;
            bitboards[ROOK] |= SHIFT(5);
            bitboards[BLACK] |= SHIFT(5);
            bitboards[ROOK] ^= SHIFT(7);
            bitboards[BLACK] ^= SHIFT(7);
        } else if(m.src == 4 && m.dest == 2){
            boardArr[3] = BLACK_ROOK;
            boardArr[0] = EMPTY;
            bitboards[ROOK] |= SHIFT(3);
            bitboards[BLACK] |= SHIFT(3);
            bitboards[ROOK] ^= SHIFT(0);
            bitboards[BLACK] ^= SHIFT(0);
        }
    }

    //Update castling metadata for rook moves/takes
    if(m.src==0 || m.dest==0) metadata.castleFlags &= ~1;
    if(m.src==7 || m.dest==7) metadata.castleFlags &= ~2;
    if(m.src==56 || m.dest==56) metadata.castleFlags &= ~4;
    if(m.src==63 || m.dest==63) metadata.castleFlags &= ~8;
}

//Reverts from executeMove - does not fix metadata
//int destSquare - piece located in destination square before move, if any (0=UNUSED1 if none). Bitboard index format (color is not included)
//wasEp - Flag denoting whether this move was to the en passant square
void Game::unexecuteMove(const Move& m, Piece destSquare){
    metadata = MD_stack.top();
    MD_stack.pop();

    int color = boardArr[m.dest]>8;
    int piece = boardArr[m.dest]%8;

    //Bitboards: Remove piece from destination square
    bitboards[color] &= ~SHIFT(m.dest);
    bitboards[piece] &= ~SHIFT(m.dest);

    //Bitboards: Return captured piece (if any) to destination square
    if(destSquare){
        bitboards[1-color] |= SHIFT(m.dest);
        bitboards[destSquare%8] |= SHIFT(m.dest);
    }
    
    if(m.promo){
        piece = PAWN;
    }

    //Bitboards: Return piece to starting square
    bitboards[color] |= SHIFT(m.src);
    bitboards[piece] |= SHIFT(m.src);

    //Board array: Restore
    boardArr[m.src] = static_cast<Piece>(piece + 8*color);
    boardArr[m.dest] = destSquare;

    //Undo en-passant capture
    if(metadata.epRight && metadata.epRight==m.dest && piece==PAWN){ 
        int repairSquare = m.dest + ( 8-16*color );
        boardArr[repairSquare] = static_cast<Piece>(PAWN + 8*(1-color));

        bitboards[1-color] |= SHIFT(repairSquare);
        bitboards[PAWN] |= SHIFT(repairSquare);
    }

    //Undo rook move for castling
    if(piece == KING){
        if(m.src == 60 && m.dest == 62){
            boardArr[63] = WHITE_ROOK;
            boardArr[61] = EMPTY;
            bitboards[ROOK] |= SHIFT(63);
            bitboards[WHITE] |= SHIFT(63);
            bitboards[ROOK] ^= SHIFT(61);
            bitboards[WHITE] ^= SHIFT(61);
        } else if(m.src == 60 && m.dest == 58){
            boardArr[56] = WHITE_ROOK;
            boardArr[59] = EMPTY;
            bitboards[ROOK] |= SHIFT(56);
            bitboards[WHITE] |= SHIFT(56);
            bitboards[ROOK] ^= SHIFT(59);
            bitboards[WHITE] ^= SHIFT(59);
        } else if(m.src == 4 && m.dest == 6){
            boardArr[7] = BLACK_ROOK;
            boardArr[5] = EMPTY;
            bitboards[ROOK] |= SHIFT(7);
            bitboards[BLACK] |= SHIFT(7);
            bitboards[ROOK] ^= SHIFT(5);
            bitboards[BLACK] ^= SHIFT(5);
        } else if(m.src == 4 && m.dest == 2){
            boardArr[0] = BLACK_ROOK;
            boardArr[3] = EMPTY;
            bitboards[ROOK] |= SHIFT(0);
            bitboards[BLACK] |= SHIFT(0);
            bitboards[ROOK] ^= SHIFT(3);
            bitboards[BLACK] ^= SHIFT(3);
        }
    }
}