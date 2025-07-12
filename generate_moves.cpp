#include "game.h"
#include "lookup.h"
#include "magic_numbers.h"
#include "precomputed.h"

void appendMoves(int src, uint64_t destBB, std::array<Move,220>::iterator& next);
void appendPawnMoves(int src, int color, uint64_t destBB, std::array<Move,220>::iterator& next);


int Game::generateMoves(std::array<Move, 220>& moveArr, int color){
    if(isInCheck(color)){
        return generateMovesCheck(moveArr,color);
    }

    auto moveArrNext = moveArr.begin();

    int kingPos = BitboardOps::bitScanForward(bitboards[KING] & bitboards[color]);
    uint64_t pinned = getPinned(kingPos, color);

    for(int piece = PAWN; piece<=KING; piece++){

        uint64_t srcBB = bitboards[color] & bitboards[piece];
        
        uint64_t pinnedToMove = pinned & srcBB;
        srcBB ^= pinnedToMove;

        while(srcBB){
            int srcBSF = BitboardOps::bitScanForward(srcBB);//Index of leftmost piece
            srcBB &= ~SHIFT(srcBSF);

            uint64_t destBB = (this->*getPieceDest[piece-2])(srcBSF, color) & ~bitboards[color];
            if(piece==PAWN) appendPawnMoves(srcBSF, color, destBB, moveArrNext);
            else appendMoves(srcBSF, destBB, moveArrNext);
        }

        while(pinnedToMove){
            int srcBSF = BitboardOps::bitScanForward(pinnedToMove);
            pinnedToMove &= ~SHIFT(srcBSF);

            uint64_t destBB = (this->*getPieceDest[piece-2])(srcBSF, color) & ~bitboards[color];

            if(srcBSF%8 == kingPos%8){
                destBB &= BitboardLookup::FILES[srcBSF%8];
            } else if(srcBSF/8 == kingPos/8){
                destBB &= BitboardLookup::RANKS[ 7 - srcBSF/8 ];
            } else if(BoardLookup::DIAGS_NE[srcBSF]==BoardLookup::DIAGS_NE[kingPos]){
                destBB &= BitboardLookup::DIAGONALS_NE[BoardLookup::DIAGS_NE[srcBSF]];
            } else if(BoardLookup::DIAGS_NW[srcBSF] == BoardLookup::DIAGS_NW[kingPos]){
                destBB &= BitboardLookup::DIAGONALS_NW[BoardLookup::DIAGS_NW[srcBSF]];
            }

            if(piece==PAWN) appendPawnMoves(srcBSF, color, destBB, moveArrNext);
            else appendMoves(srcBSF, destBB, moveArrNext);

        }
    }

    return moveArrNext - moveArr.begin();
}

int Game::generateMovesCheck(std::array<Move,220>& moveArr, int color){
    auto moveArrNext = moveArr.begin();

    uint64_t kingBb = bitboards[color] & bitboards[KING];
    int kingPos = BitboardOps::bitScanForward(kingBb);

    uint64_t pinned = getPinned(kingPos, color);

    uint64_t slidingAttackers = genRook(kingPos, color) & (bitboards[ROOK] | bitboards[QUEEN]) & bitboards[1-color];
    
    uint64_t slideThroughDest; //Full trajectory of sliding pieces, to ensure king leaves rank/file/diagonal
    uint64_t temp = slidingAttackers;
    while(temp){
        int src = BitboardOps::bitScanForward(temp);
        temp ^= SHIFT(src);
        slideThroughDest |= BitboardLookup::ROOK_DEST[src];
    }

    temp = genBishop(kingPos, color) & (bitboards[BISHOP] | bitboards[QUEEN]) & bitboards[1-color];
    slidingAttackers |= temp;

    while(temp){
        int src = BitboardOps::bitScanForward(temp);
        temp ^= SHIFT(src);
        slideThroughDest |= BitboardLookup::BISHOP_DEST[src];
    }

    int numSliding = BitboardOps::countBits(slidingAttackers);

    uint64_t pawnKnightAttackers = genKnight(kingPos, color) & bitboards[1-color] & bitboards[KNIGHT];
    if(color==WHITE){
        pawnKnightAttackers |=  ( (kingBb >> 7) | (kingBb >> 9) ) & bitboards[Game::BLACK] & bitboards[PAWN];
    } else {
        pawnKnightAttackers |=  ( (kingBb << 7) | (kingBb << 9) ) & bitboards[Game::WHITE] & bitboards[PAWN];
    }
    int numPK = BitboardOps::countBits(pawnKnightAttackers);


    if(numSliding && numPK || numSliding > 1 || numPK >1){
        appendMoves(kingPos, genKing(kingPos, color) & ~bitboards[color] & ~slideThroughDest, moveArrNext);
    } else {
        //Bitboard of legal destination squares: If single sliding attacker, inbetween or capture. If single other attacker, only capture
        uint64_t checkRestrictionMask = numPK ? pawnKnightAttackers : Precomputed::inbetweenBB[kingPos][BitboardOps::bitScanForward(slidingAttackers)] | slidingAttackers;
        
        for(int piece = PAWN; piece<=QUEEN; piece++){
            uint64_t srcBB = bitboards[color] & bitboards[piece];
            
            //Absolutely pinned pieces cannot move at all
            srcBB &= ~pinned;

            while(srcBB){
                int srcBSF = BitboardOps::bitScanForward(srcBB);//Index of leftmost piece
                srcBB &= ~SHIFT(srcBSF);

                uint64_t destBB = (this->*getPieceDest[piece-2])(srcBSF, color) & ~bitboards[color];
                destBB &= checkRestrictionMask;

                if(piece==PAWN) appendPawnMoves(srcBSF, color,destBB, moveArrNext);
                else appendMoves(srcBSF, destBB, moveArrNext);
            }
        }  
        appendMoves(kingPos, genKing(kingPos, color) & ~bitboards[color] & ~slideThroughDest, moveArrNext);
    } 

    return moveArrNext - moveArr.begin();
}

bool Game::isInCheck(int color){
    uint64_t king = bitboards[color] & bitboards[KING];
    return (king & getAttacking(1-color))!=0;
}

uint64_t Game::genKnight(int src, int color){
    return BitboardLookup::KNIGHT_DEST[src];
}

uint64_t Game::genKing(int src, int color){
    uint64_t attacking = getAttacking(1-color);
    uint64_t occ =  (bitboards[color] & ~bitboards[BBIndex::KING]) | bitboards[1-color];
    uint64_t rv = BitboardLookup::KING_DEST[src] & ~attacking;

    if(color==WHITE){
        if( (metadata.castleFlags & 8) && !((attacking | occ) & BitboardConst::WHITE_KS_CASTLE_MASK) ){
            rv |= 0x4000000000000000;
        }
        if( (metadata.castleFlags & 4) && !((attacking | occ) & BitboardConst::WHITE_QS_CASTLE_MASK) ){
            rv |= 0x0400000000000000;
        }
    } else {
        if( (metadata.castleFlags & 2) && !((attacking | occ) & BitboardConst::BLACK_KS_CASTLE_MASK) ){
            rv |= 0x40;
        }
        if( (metadata.castleFlags & 1) && !((attacking | occ) & BitboardConst::BLACK_QS_CASTLE_MASK) ){
            rv |= 0x4;
        }
    }
    return rv;
}

uint64_t Game::genRook(int src, int color){
    return MagicNumbers::genDestRook(src, bitboards[WHITE] | bitboards[BLACK]);
}

uint64_t Game::genBishop(int src, int color){
    return MagicNumbers::genDestBishop(src, bitboards[WHITE] | bitboards[BLACK]);
}

uint64_t Game::genQueen(int src, int color){
    return MagicNumbers::genDestRook(src, bitboards[WHITE] | bitboards[BLACK]) | MagicNumbers::genDestBishop(src, bitboards[WHITE] | bitboards[BLACK]);
}

void appendMoves(int src, uint64_t destBB, std::array<Move, 220>::iterator& moveArrNext){
    int destBsf;
    while(destBB){
        int destBsf = BitboardOps::bitScanForward(destBB);
        destBB &= ~SHIFT(destBsf);

        *(moveArrNext++) = Move{src, destBsf, Move::Promo::NONE};
    }
}

void appendPawnMoves(int src, int color, uint64_t destBB, std::array<Move, 220>::iterator& moveArrNext){
    int destBsf;
    while(destBB){
        int destBsf = BitboardOps::bitScanForward(destBB);
        destBB &= ~SHIFT(destBsf);
        if(destBsf/8 == 7*color){//Moving to final rank  
            *(moveArrNext++) = Move {src, destBsf, Move::Promo::KNIGHT};
            *(moveArrNext++) = Move {src, destBsf, Move::Promo::BISHOP};
            *(moveArrNext++) = Move {src, destBsf, Move::Promo::ROOK};
            *(moveArrNext++) = Move {src, destBsf, Move::Promo::QUEEN};
        } else {
            *(moveArrNext++) = Move {src, destBsf, Move::Promo::NONE};
        }
    }
}

uint64_t Game::genPawn(int src, int color){
    return color == WHITE ? genPawnWhite(src) : genPawnBlack(src);
}

uint64_t Game::genPawnWhite(uint64_t pawnPos){
    uint64_t rv = 0;
    rv |= (pawnPos >> 8) & ~(bitboards[Game::BLACK] | bitboards[Game::WHITE]);//Single push
    rv |= ( (rv & BitboardLookup::RANKS[2]) >> 8) & ~(bitboards[Game::BLACK] | bitboards[Game::WHITE]);//Double push

    rv |= (pawnPos >> 7) & bitboards[Game::BLACK] & BitboardConst::NOT_A_FILE;//Right attacks
    rv |= (pawnPos >> 9) & bitboards[Game::BLACK] & BitboardConst::NOT_H_FILE;//Left attacks


    if(metadata.epRight){
        rv |= (pawnPos >> 7) & SHIFT(metadata.epRight) & BitboardConst::NOT_A_FILE;//Right en passant
        rv |= (pawnPos >> 9) & SHIFT(metadata.epRight) & BitboardConst::NOT_H_FILE;//Left en passant
    }
    return rv;
}

uint64_t Game::genPawnBlack(uint64_t pawnPos){
    uint64_t rv = 0;
    rv |= (pawnPos << 8) & ~(bitboards[Game::BLACK] | bitboards[Game::WHITE]);//Single push
    rv |= ( (rv & BitboardLookup::RANKS[5]) << 8) & ~(bitboards[Game::BLACK] | bitboards[Game::WHITE]);//Double push

    rv |= (pawnPos << 7) & bitboards[Game::WHITE] & BitboardConst::NOT_H_FILE;//Left attacks (white perspective)
    rv |=  (pawnPos << 9) & bitboards[Game::WHITE] & BitboardConst::NOT_A_FILE;//Right attacks

    if(metadata.epRight){
        rv |= (pawnPos << 7) & SHIFT(metadata.epRight) & BitboardConst::NOT_H_FILE;//Left en passant
        rv |= (pawnPos << 9) & SHIFT(metadata.epRight) & BitboardConst::NOT_A_FILE;//Right en passant
    }
    return rv;
}

//Used for king move generation (returns the attacks of the passed color)
uint64_t Game::getAttacking(int color){
    uint64_t rv = 0;
    uint64_t pawns = bitboards[color] & bitboards[PAWN];
    if(color==WHITE){
        rv |= (pawns >> 9) & BitboardConst::NOT_H_FILE;
        rv |= (pawns >> 7) & BitboardConst::NOT_A_FILE;
    } else {
        rv |= (pawns << 9) & BitboardConst::NOT_A_FILE;
        rv |= (pawns << 7) & BitboardConst::NOT_H_FILE;
    }

    uint64_t knights = bitboards[color] & bitboards[KNIGHT];
    while(knights){
        int src = BitboardOps::bitScanForward(knights);
        knights &= ~SHIFT(src);
        rv |= BitboardLookup::KNIGHT_DEST[src];
    }

    rv |= BitboardLookup::KING_DEST[BitboardOps::bitScanForward(bitboards[color] & bitboards[KING])];

    uint64_t rooks = bitboards[color] & bitboards[ROOK];
    while(rooks){
        int src = BitboardOps::bitScanForward(rooks);
        rooks &= ~SHIFT(src);
        rv |= genRook(src, color);
    }
    uint64_t bishops = bitboards[color] & bitboards[BISHOP];
    while(bishops){
        int src = BitboardOps::bitScanForward(bishops);
        bishops &= ~SHIFT(src);
        rv |= genBishop(src, color);
    }
    uint64_t queens = bitboards[color] & bitboards[QUEEN];
    while(queens){
        int src = BitboardOps::bitScanForward(queens);
        queens &= ~SHIFT(src);
        rv |= genQueen(src, color);
    }

    return rv;
}

uint64_t Game::getPinned(int kingPos, int color){
    uint64_t pinned = 0 ;

    uint64_t enemySliders = genXrayRook(bitboards[Game::WHITE] | bitboards[Game::BLACK], bitboards[color], kingPos) & (bitboards[1-color] & (bitboards[QUEEN] | bitboards[ROOK]));
    while(enemySliders){
        int src = BitboardOps::bitScanForward(enemySliders);
        enemySliders &= ~SHIFT(src);

        pinned |= Precomputed::inbetweenBB[src][kingPos] & bitboards[color];
    }

    enemySliders = genXrayBishop(bitboards[Game::WHITE] | bitboards[Game::BLACK], bitboards[color], kingPos) & (bitboards[1-color] & (bitboards[QUEEN] | bitboards[BISHOP]));
    while(enemySliders){
        int src = BitboardOps::bitScanForward(enemySliders);
        enemySliders &= ~SHIFT(src);

        pinned |= Precomputed::inbetweenBB[src][kingPos] & bitboards[color];
    }
    return pinned;
}

uint64_t Game::genXrayRook(uint64_t pieces, uint64_t blockers, int src){
    uint64_t destBB = MagicNumbers::genDestRook(src, pieces);
    blockers &= destBB;

    return destBB ^ MagicNumbers::genDestRook(src, pieces ^ blockers);

}

uint64_t Game::genXrayBishop(uint64_t pieces, uint64_t blockers, int src){
    uint64_t destBB = MagicNumbers::genDestBishop(src, pieces);
    blockers &= destBB;

    return destBB ^ MagicNumbers::genDestBishop(src, pieces ^ blockers);

}
