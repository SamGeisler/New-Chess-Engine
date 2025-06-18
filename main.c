#include "SDL.h"
#include "bb_utils.h"
#include "global.h"
#include "render.h"
#include "control.h"
#include "generate_moves.h"
//#include "search.h"

uint64_t knightDest[64] = {0x20400, 0x50800, 0xA1100, 0x142200, 0x284400, 0x508800, 0xA01000, 0x402000, 0x2040004, 0x5080008, 0xA110011, 0x14220022, 0x28440044, 0x50880088, 0xA0100010, 0x40200020, 0x204000402, 0x508000805, 0xA1100110A, 0x1422002214, 0x2844004428, 0x5088008850, 0xA0100010A0, 0x4020002040, 0x20400040200, 0x50800080500, 0xA1100110A00, 0x142200221400, 0x284400442800, 0x508800885000, 0xA0100010A000, 0x402000204000, 0x2040004020000, 0x5080008050000, 0xA1100110A0000, 0x14220022140000, 0x28440044280000, 0x50880088500000, 0xA0100010A00000, 0x40200020400000, 0x204000402000000, 0x508000805000000, 0xA1100110A000000, 0x1422002214000000, 0x2844004428000000, 0x5088008850000000, 0xA0100010A0000000, 0x4020002040000000, 0x400040200000000, 0x800080500000000, 0x1100110A00000000, 0x2200221400000000, 0x4400442800000000, 0x8800885000000000, 0x100010A000000000, 0x2000204000000000, 0x4020000000000, 0x8050000000000, 0x110A0000000000, 0x22140000000000, 0x44280000000000, 0x88500000000000, 0x10A00000000000, 0x20400000000000};
uint64_t kingDest[64] = {0x302, 0x705, 0xE0A, 0x1C14, 0x3828, 0x7050, 0xE0A0, 0xC040, 0x30203, 0x70507, 0xE0A0E, 0x1C141C, 0x382838, 0x705070, 0xE0A0E0, 0xC040C0, 0x3020300, 0x7050700, 0xE0A0E00, 0x1C141C00, 0x38283800, 0x70507000, 0xE0A0E000, 0xC040C000, 0x302030000, 0x705070000, 0xE0A0E0000, 0x1C141C0000, 0x3828380000, 0x7050700000, 0xE0A0E00000, 0xC040C00000, 0x30203000000, 0x70507000000, 0xE0A0E000000, 0x1C141C000000, 0x382838000000, 0x705070000000, 0xE0A0E0000000, 0xC040C0000000, 0x3020300000000, 0x7050700000000, 0xE0A0E00000000, 0x1C141C00000000, 0x38283800000000, 0x70507000000000, 0xE0A0E000000000, 0xC040C000000000, 0x302030000000000, 0x705070000000000, 0xE0A0E0000000000, 0x1C141C0000000000, 0x3828380000000000, 0x7050700000000000, 0xE0A0E00000000000, 0xC040C00000000000, 0x203000000000000, 0x507000000000000, 0xA0E000000000000, 0x141C000000000000, 0x2838000000000000, 0x5070000000000000, 0xA0E0000000000000, 0x40C0000000000000};
uint64_t rookDestTrunc[64] = {0x101010101017E, 0x202020202027C, 0x404040404047A, 0x8080808080876, 0x1010101010106E, 0x2020202020205E, 0x4040404040403E, 0x8080808080807E, 0x1010101017E00, 0x2020202027C00, 0x4040404047A00, 0x8080808087600, 0x10101010106E00, 0x20202020205E00, 0x40404040403E00, 0x80808080807E00, 0x10101017E0100, 0x20202027C0200, 0x40404047A0400, 0x8080808760800, 0x101010106E1000, 0x202020205E2000, 0x404040403E4000, 0x808080807E8000, 0x101017E010100, 0x202027C020200, 0x404047A040400, 0x8080876080800, 0x1010106E101000, 0x2020205E202000, 0x4040403E404000, 0x8080807E808000, 0x1017E01010100, 0x2027C02020200, 0x4047A04040400, 0x8087608080800, 0x10106E10101000, 0x20205E20202000, 0x40403E40404000, 0x80807E80808000, 0x17E0101010100, 0x27C0202020200, 0x47A0404040400, 0x8760808080800, 0x106E1010101000, 0x205E2020202000, 0x403E4040404000, 0x807E8080808000, 0x7E010101010100, 0x7C020202020200, 0x7A040404040400, 0x76080808080800, 0x6E101010101000, 0x5E202020202000, 0x3E404040404000, 0x7E808080808000, 0x7E01010101010100, 0x7C02020202020200, 0x7A04040404040400, 0x7608080808080800, 0x6E10101010101000, 0x5E20202020202000, 0x3E40404040404000, 0x7E80808080808000};
uint64_t bishopDestTrunc[64] = {0x40201008040200, 0x402010080400, 0x4020100A00, 0x40221400, 0x2442800, 0x204085000, 0x20408102000, 0x2040810204000, 0x20100804020000, 0x40201008040000, 0x4020100A0000, 0x4022140000, 0x244280000, 0x20408500000, 0x2040810200000, 0x4081020400000, 0x10080402000200, 0x20100804000400, 0x4020100A000A00, 0x402214001400, 0x24428002800, 0x2040850005000, 0x4081020002000, 0x8102040004000, 0x8040200020400, 0x10080400040800, 0x20100A000A1000, 0x40221400142200, 0x2442800284400, 0x4085000500800, 0x8102000201000, 0x10204000402000, 0x4020002040800, 0x8040004081000, 0x100A000A102000, 0x22140014224000, 0x44280028440200, 0x8500050080400, 0x10200020100800, 0x20400040201000, 0x2000204081000, 0x4000408102000, 0xA000A10204000, 0x14001422400000, 0x28002844020000, 0x50005008040200, 0x20002010080400, 0x40004020100800, 0x20408102000, 0x40810204000, 0xA1020400000, 0x142240000000, 0x284402000000, 0x500804020000, 0x201008040200, 0x402010080400, 0x2040810204000, 0x4081020400000, 0xA102040000000, 0x14224000000000, 0x28440200000000, 0x50080402000000, 0x20100804020000, 0x40201008040200};

int rookMN_w[64] = {13, 12, 13, 13, 13, 13, 12, 13, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 10, 11, 12, 11, 11, 11, 11, 11, 11, 11, 12, 11, 11, 11, 11, 11, 10, 12, 12, 11, 11, 11, 11, 11, 10, 11, 12, 11, 11, 11, 11, 11, 11, 11, 13, 11, 11, 11, 12, 12, 11, 12};
int bishopMN_w[64]={6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 8, 8, 8, 5, 5, 5, 5, 8, 10, 11, 8, 5, 5, 5, 5, 7, 11, 11, 7, 5, 5, 5, 5, 8, 8, 8, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};
uint64_t rookMN[64] = {0xC9873C62385CB89F, 0x10C31FAD8CEAA30D, 0xCF5E04D70671F0A3, 0x89B92A9D723B3B55, 0xC6E64BC5D919CBB3, 0xA514179558BC361D, 0x39C013037F09CD08, 0xDC11AB87F9E635EE, 0x33FCAF149167CBB1, 0xF0215351FA4EC13B, 0x3C44E943A79118D2, 0xCF128B254EED659, 0x63F09D52A0509D44, 0xABFC00C31FE1314, 0x38E0900BA684979F, 0x3EDE5454B04943B4, 0x9659334D6162F60, 0x5AF94E9FFD7A217E, 0x7F7D05ED060861C, 0xFB4B3E0020060310, 0x3C6FAF0F0EC909B7, 0x347362A006C981FD, 0x9FB084004B1E2850, 0x4FB2CA001C0DC58D, 0x5CDCAFEA46A19FDE, 0x3B70668273DBE7B6, 0x3082A6E8778A81DD, 0x4E7625655222EBBF, 0x5D49263BDF92B3B, 0x4B34083FEF84EE24, 0xF0606F5D83103C07, 0x80BA1062000B1F6C, 0xFEA44F41CA00125F, 0xD3D59728BFC03E5A, 0xDF4DEAA4B06AACAD, 0x78BF37ACC851F48D, 0x13DCE28A85A5A5BE, 0x56BB0A640011EA52, 0xB95428160C00D005, 0x69E48ECA21937837, 0xB03CF520F600A3D7, 0x6F8A5D37EC9F3322, 0x896B24E807605401, 0xFA475610D9F0DA1C, 0xAA650F8B20FF8B2D, 0x26A2EFC4E588008D, 0x931C232238140050, 0x55E5E50369860004, 0x71771395C22470D0, 0x3BE6B33D60BD7A78, 0xF46155ECEADFBA2D, 0xB4120CC4D6002200, 0x7BA585BBD17876F1, 0xBC6CB45A2DB5A600, 0x5E31AF411D2F5210, 0x52B9741295094A00, 0x7737816369C2E06E, 0x8F9C400103F08321, 0x7431A0008A704301, 0x8B82A0C01AEA0012, 0xC75982DF3C2C1D2, 0xE5AE01920F078582, 0xADDB860488103104, 0xA8B31F0A8B426BFA};
uint64_t bishopMN[64] = {0x3D40145521470103, 0x606D029090D0048, 0x28D01B0A0F240137, 0x5A07EA7CF43E9D4F, 0x4624042260BCCDF3, 0x2C60304602EEDFE, 0xA043EC039587147D, 0xE863220910051055, 0xF43CBC84500C4D02, 0x803E48010803A905, 0xBF32C1DE02510B20, 0x509B5820D4C485C2, 0x1FAFD41C20709799, 0x6D96C2080E1952A3, 0x547306C606304007, 0x2B6CB60B04063E86, 0xA1F1FC750FD00720, 0x652422D04508450B, 0x4E5814100BC31460, 0x3B27B8AF68E01031, 0x8C0BFE72836BEF99, 0x24719D233B0B73C3, 0x89FD8EC7541057F7, 0xE9EB05DE2C060E04, 0x82604587B8086818, 0x327B709A600A0E06, 0x69791C9646F632DF, 0xCAA10210E5F0803A, 0xA51846AE650F6F41, 0x53C4398F9EEF70F2, 0x294A508EE08062D, 0xC6EB02C4E1041F92, 0x9CB22020CFFD082E, 0x937D1C2084F2581A, 0xB44E4128145007C0, 0x5E384DB8E5A5A2, 0xD86D7DB9A8F6E8EB, 0x1C3A5BAB004E0097, 0x59838150323D701, 0x6F711AC903B6011E, 0xFEE91420E04074A1, 0x4D440B0818616651, 0xAEAB8EF2EFF9B3DC, 0x210F2E0972561333, 0xF8B57A9685A15280, 0x5C403410C4106B80, 0x5E065C8D020A8C01, 0x791C388601421206, 0xA043EC039587147D, 0x18C0A40744300FE4, 0x21E28AEDA8D00698, 0x22723A2E42021C25, 0x441A2151B20E0043, 0x5DC3E0B4694A0187, 0x34D1F09A27E0D2FE, 0x606D029090D0048, 0xE863220910051055, 0x2B6CB60B04063E86, 0x643054DB6C16180D, 0x791C388601421206, 0x69DFDD4D701E1601, 0x330C10C8F11B2200, 0xF43CBC84500C4D02, 0x3D40145521470103};

void init_board(char* board_init);//Initialize board_arr and board struct from FEN string

void update_board_arr();//Update board_arr from bitboards
void update_bb();//Update borad.bitboards from board_arr
void loadDestBB();//Load sliding piece intersections destination bitboards (rookDestInt & bishopDestInt)


void print_board_arr();


int main(int argc, char* argv[]){
    loadDestBB();
    init_board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    render_init();
    render_board(-1);

    move* moves = (move*)malloc(sizeof(move)*220);

    while(1){
        printf("\nWHITE MOVES:\n");
        for(int i = 0; i<generate_moves(moves,WHITE); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }
        printf("\nBLACK MOVES:\n");
        for(int i = 0; i<generate_moves(moves,BLACK); i++){
            printf("%d -> %d, promo %d\n", moves[i].src, moves[i].dest, moves[i].promo);
        }

        execute_move(handle_player_input());
        update_bb();
    } 

    render_quit();
    return 0;
}

void loadDestBB(){
    printf("Loading precomputed data...\n");

    FILE* dest_fptr = fopen("rookDestReal.txt","r");//Destination bb based on intersections with other pieces
    FILE* int_fptr = fopen("rookIntersections.txt","r");//Possible intersections

    for(int src = 0; src<64; src++){
        int hash_capacity = SHIFT(rookMN_w[src]);
        int num_entries = SHIFT(count_bits(rookDestTrunc[src]));

        rookDestInt[src] = (uint64_t*)malloc(hash_capacity * sizeof(uint64_t));

        for(int i = 0; i<num_entries; i++){
            uint64_t dest_bb;
            uint64_t int_bb;
            fscanf(dest_fptr,"%llX",&dest_bb);
            fscanf(int_fptr,"%llX",&int_bb);

            int index = (rookMN[src] * int_bb)>>(64-rookMN_w[src]);
            rookDestInt[src][index] = dest_bb;
        }
    }

    fclose(dest_fptr);
    fclose(int_fptr);
    dest_fptr = fopen("bishopDestReal.txt","r");
    int_fptr = fopen("bishopIntersections.txt","r");

    for(int src = 0; src<64; src++){
        int hash_capacity = SHIFT(bishopMN_w[src]);
        int num_entries = SHIFT(count_bits(bishopDestTrunc[src]));

        bishopDestInt[src] = (uint64_t*)malloc(hash_capacity * sizeof(uint64_t));

        for(int i = 0; i<num_entries; i++){
            uint64_t dest_bb;
            uint64_t int_bb;
            fscanf(dest_fptr,"%llX",&dest_bb);
            fscanf(int_fptr,"%llX",&int_bb);

            int index = (bishopMN[src] * int_bb)>>(64-bishopMN_w[src]);
            bishopDestInt[src][index] = dest_bb;
        }
    }

    fclose(dest_fptr);
    fclose(int_fptr);
}


void init_board(char* board_init){
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
                board_arr[pos] = i-1;
            }
            current_bb >>=1;
            pos++;
        }

        current_bb = board.bitboards[i] & board.bitboards[BLACK];
        pos = 0;
        while(current_bb){
            if(current_bb & 1){
                board_arr[pos] = i+5;
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
            board.bitboards[board_arr[i]>=7] |= SHIFT(i);//Color
            board.bitboards[((board_arr[i]-1)%6) +2] |= SHIFT(i);//Piece
        }
    }
}

void print_board_arr(){
    char c;
    for(int i = 0; i<64; i++){
        switch(i){
            case WHITE_PAWN: c='P';
            case WHITE_KNIGHT: c='N';
            case WHITE_BISHOP: c='B';
            case WHITE_ROOK: c='R';
            case WHITE_QUEEN: c='Q';
            case WHITE_KING: c='K';
            case BLACK_PAWN: c='p';
            case BLACK_KNIGHT: c='n';
            case BLACK_BISHOP: c='b';
            case BLACK_ROOK: c='r';
            case BLACK_QUEEN: c='q';
            case BLACK_KING: c='k';
        }

        printf("%c ");
        if(i%8==7) printf("\n");
    }
}