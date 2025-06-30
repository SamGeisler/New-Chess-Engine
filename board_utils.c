#include "global.h"

const int index64[64] = {
    0,  1, 48,  2, 57, 49, 28,  3,
   61, 58, 50, 42, 38, 29, 17,  4,
   62, 55, 59, 36, 53, 51, 43, 22,
   45, 39, 33, 30, 24, 18, 12,  5,
   63, 47, 56, 27, 60, 41, 37, 16,
   54, 35, 52, 21, 44, 32, 23, 11,
   46, 26, 40, 15, 34, 20, 31, 10,
   25, 14, 19,  9, 13,  8,  7,  6
};

int bitScanForward(uint64_t bb) {
   const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
   return index64[((bb & -bb) * debruijn64) >> 58];
}

void print_bb(uint64_t bb){
    printf("Bitboard: 0x%llX\n",bb);
    for(int i = 0; i<64; i++){
        printf("%c ", (SHIFT(i) & bb) ? 'X' : '.');
        if(i%8==7) printf("\n");
    }
}

int count_bits(uint64_t bb){
    int rv = 0;
    while(bb){
        bb &= ~SHIFT(bitScanForward(bb));
        rv++;
    }
    return rv;
}

void init_board(char* board_init){
    //Initialize metadata
    MD.castle_flags = 0xF;
    MD.ep_right = 0;

    for(int i = 0;i<64; i++) board_arr[i] = EMPTY;
    int i = 0;
    int pos = 0;
    while(board_init[i]!=' '){
        // piece
        if(board_init[i] <=122 && board_init[i] >=65){
            char p = board_init[i];
            switch (p){
                case 'K': 
                    board_arr[pos] = WHITE_KING; 
                    break;
                case 'Q': 
                    board_arr[pos] = WHITE_QUEEN; 
                    break;
                case 'R': 
                    board_arr[pos] = WHITE_ROOK; 
                    break;
                case 'B': 
                    board_arr[pos] = WHITE_BISHOP; 
                    break;
                case 'N': 
                    board_arr[pos] = WHITE_KNIGHT; 
                    break;
                case 'P': 
                    board_arr[pos] = WHITE_PAWN; 
                    break;
                case 'k': 
                    board_arr[pos] = BLACK_KING; 
                    break;
                case 'q': 
                    board_arr[pos] = BLACK_QUEEN; 
                    break;
                case 'r': 
                    board_arr[pos] = BLACK_ROOK; 
                    break;
                case 'b': 
                    board_arr[pos] = BLACK_BISHOP; 
                    break;
                case 'n': 
                    board_arr[pos] = BLACK_KNIGHT; 
                    break;
                case 'p': 
                    board_arr[pos] = BLACK_PAWN; 
                    break;
            }
            pos++;
        }   
        //number
        else if(board_init[i] <= 57 && board_init[i] >= 48){
            pos += board_init[i] - '0';
        }

        i++;
    }
    i++;
    //player to move
    MD.to_move = board_init[i]=='b';

    i+=2;
    //castle rights
    MD.castle_flags = 0;
    while(board_init[i]!=' '){
        if(board_init[i]=='K'){
            MD.castle_flags += 8;
        } else if(board_init[i]=='Q'){
            MD.castle_flags += 4;
        } else if(board_init[i]=='k'){
            MD.castle_flags += 2;
        } else if(board_init[i]=='q'){
            MD.castle_flags += 1;
        }
        i++;
    }
    i++;
    //en passant target square
    if(board_init[i]!='-'){
        MD.ep_right += board_init[i]-'a';
        i++;
        MD.ep_right += (8-board_init[i])*8;
    } else {
        MD.ep_right = 0;
    }

    update_bb();
}

void update_board_arr(){
    for(int i = 0;i<64; i++) board_arr[i] = EMPTY;
    uint64_t current_bb;
    int pos;
    for(int i = PAWN; i<=KING; i++){
        current_bb = board.bitboards[i] & board.bitboards[WHITE];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i;
            }
            current_bb >>=1;
            pos++;
        }

        current_bb = board.bitboards[i] & board.bitboards[BLACK];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i+8;
            }
            current_bb >>=1;
            pos++;
        }
    }
}

void update_bb(){
    for(int i = 0; i<8; i++) board.bitboards[i] = 0;
    for(int i = 0; i<64; i++){
        if(board_arr[i]){
            board.bitboards[ board_arr[i]>8 ] |= SHIFT(i);//Color
            board.bitboards[ board_arr[i]%8 ] |= SHIFT(i);//Piece
        }
    }
}

void print_board_arr(){
    char c;
    for(int i = 0; i<64; i++){
        switch(board_arr[i]){
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
        }

        printf("%c ",c);
        if(i%8==7) printf("\n");
    }
}

void print_metadata(){
    printf(MD.to_move ? "Black" : "White");
    printf(" to move.\n");
    printf("Castling rights - WKS: %d, WQS: %d, BKS: %d, BQS: %d\n",!!(MD.castle_flags&8),!!(MD.castle_flags&4),!!(MD.castle_flags&2),!!(MD.castle_flags&1));
    printf("En-passant target square: %d\n", MD.ep_right);
    printf("Fifty-move rule count (half moves): %d\n\n",MD.fmr_count);
}

char* notation(int pos){
    static char rv[3];
    rv[0] = 'a' + (63-pos)/8;
    rv[1] = '0' + pos%8;
    return rv;
}