#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <stdlib.h>
#include <set>
#include <fstream>
#include <iostream>
#include <assert.h>
#include <math.h>

#define SHIFT(x) (((uint64_t)1)<<(x))

uint64_t rookDestTrunc[64] = {0x101010101017E, 0x202020202027C, 0x404040404047A, 0x8080808080876, 0x1010101010106E, 0x2020202020205E, 0x4040404040403E, 0x8080808080807E, 0x1010101017E00, 0x2020202027C00, 0x4040404047A00, 0x8080808087600, 0x10101010106E00, 0x20202020205E00, 0x40404040403E00, 0x80808080807E00, 0x10101017E0100, 0x20202027C0200, 0x40404047A0400, 0x8080808760800, 0x101010106E1000, 0x202020205E2000, 0x404040403E4000, 0x808080807E8000, 0x101017E010100, 0x202027C020200, 0x404047A040400, 0x8080876080800, 0x1010106E101000, 0x2020205E202000, 0x4040403E404000, 0x8080807E808000, 0x1017E01010100, 0x2027C02020200, 0x4047A04040400, 0x8087608080800, 0x10106E10101000, 0x20205E20202000, 0x40403E40404000, 0x80807E80808000, 0x17E0101010100, 0x27C0202020200, 0x47A0404040400, 0x8760808080800, 0x106E1010101000, 0x205E2020202000, 0x403E4040404000, 0x807E8080808000, 0x7E010101010100, 0x7C020202020200, 0x7A040404040400, 0x76080808080800, 0x6E101010101000, 0x5E202020202000, 0x3E404040404000, 0x7E808080808000, 0x7E01010101010100, 0x7C02020202020200, 0x7A04040404040400, 0x7608080808080800, 0x6E10101010101000, 0x5E20202020202000, 0x3E40404040404000, 0x7E80808080808000};
uint64_t* rookDestInt[64];
uint64_t bishopDestTrunc[64] = {0x40201008040200, 0x402010080400, 0x4020100A00, 0x40221400, 0x2442800, 0x204085000, 0x20408102000, 0x2040810204000, 0x20100804020000, 0x40201008040000, 0x4020100A0000, 0x4022140000, 0x244280000, 0x20408500000, 0x2040810200000, 0x4081020400000, 0x10080402000200, 0x20100804000400, 0x4020100A000A00, 0x402214001400, 0x24428002800, 0x2040850005000, 0x4081020002000, 0x8102040004000, 0x8040200020400, 0x10080400040800, 0x20100A000A1000, 0x40221400142200, 0x2442800284400, 0x4085000500800, 0x8102000201000, 0x10204000402000, 0x4020002040800, 0x8040004081000, 0x100A000A102000, 0x22140014224000, 0x44280028440200, 0x8500050080400, 0x10200020100800, 0x20400040201000, 0x2000204081000, 0x4000408102000, 0xA000A10204000, 0x14001422400000, 0x28002844020000, 0x50005008040200, 0x20002010080400, 0x40004020100800, 0x20408102000, 0x40810204000, 0xA1020400000, 0x142240000000, 0x284402000000, 0x500804020000, 0x201008040200, 0x402010080400, 0x2040810204000, 0x4081020400000, 0xA102040000000, 0x14224000000000, 0x28440200000000, 0x50080402000000, 0x20100804020000, 0x40201008040200};
uint64_t* bishopDestInt[64];

int rookMN_w[64] = {13, 12, 13, 13, 13, 13, 12, 13, 12, 11, 11, 11, 11, 11, 11, 12, 12, 11, 11, 11, 11, 11, 10, 11, 12, 11, 11, 11, 11, 11, 11, 11, 12, 11, 11, 11, 11, 11, 10, 12, 12, 11, 11, 11, 11, 11, 10, 11, 12, 11, 11, 11, 11, 11, 11, 11, 13, 11, 11, 11, 12, 12, 11, 12};
int bishopMN_w[64]={6, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 7, 8, 8, 8, 5, 5, 5, 5, 8, 10, 11, 8, 5, 5, 5, 5, 7, 11, 11, 7, 5, 5, 5, 5, 8, 8, 8, 7, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 6, 5, 5, 5, 5, 5, 5, 6};
uint64_t rookMN[64] = {0xC9873C62385CB89F, 0x10C31FAD8CEAA30D, 0xCF5E04D70671F0A3, 0x89B92A9D723B3B55, 0xC6E64BC5D919CBB3, 0xA514179558BC361D, 0x39C013037F09CD08, 0xDC11AB87F9E635EE, 0x33FCAF149167CBB1, 0xF0215351FA4EC13B, 0x3C44E943A79118D2, 0xCF128B254EED659, 0x63F09D52A0509D44, 0xABFC00C31FE1314, 0x38E0900BA684979F, 0x3EDE5454B04943B4, 0x9659334D6162F60, 0x5AF94E9FFD7A217E, 0x7F7D05ED060861C, 0xFB4B3E0020060310, 0x3C6FAF0F0EC909B7, 0x347362A006C981FD, 0x9FB084004B1E2850, 0x4FB2CA001C0DC58D, 0x5CDCAFEA46A19FDE, 0x3B70668273DBE7B6, 0x3082A6E8778A81DD, 0x4E7625655222EBBF, 0x5D49263BDF92B3B, 0x4B34083FEF84EE24, 0xF0606F5D83103C07, 0x80BA1062000B1F6C, 0xFEA44F41CA00125F, 0xD3D59728BFC03E5A, 0xDF4DEAA4B06AACAD, 0x78BF37ACC851F48D, 0x13DCE28A85A5A5BE, 0x56BB0A640011EA52, 0xB95428160C00D005, 0x69E48ECA21937837, 0xB03CF520F600A3D7, 0x6F8A5D37EC9F3322, 0x896B24E807605401, 0xFA475610D9F0DA1C, 0xAA650F8B20FF8B2D, 0x26A2EFC4E588008D, 0x931C232238140050, 0x55E5E50369860004, 0x71771395C22470D0, 0x3BE6B33D60BD7A78, 0xF46155ECEADFBA2D, 0xB4120CC4D6002200, 0x7BA585BBD17876F1, 0xBC6CB45A2DB5A600, 0x5E31AF411D2F5210, 0x52B9741295094A00, 0x7737816369C2E06E, 0x8F9C400103F08321, 0x7431A0008A704301, 0x8B82A0C01AEA0012, 0xC75982DF3C2C1D2, 0xE5AE01920F078582, 0xADDB860488103104, 0xA8B31F0A8B426BFA};
uint64_t bishopMN[64] = {0x3D40145521470103, 0x606D029090D0048, 0x28D01B0A0F240137, 0x5A07EA7CF43E9D4F, 0x4624042260BCCDF3, 0x2C60304602EEDFE, 0xA043EC039587147D, 0xE863220910051055, 0xF43CBC84500C4D02, 0x803E48010803A905, 0xBF32C1DE02510B20, 0x509B5820D4C485C2, 0x1FAFD41C20709799, 0x6D96C2080E1952A3, 0x547306C606304007, 0x2B6CB60B04063E86, 0xA1F1FC750FD00720, 0x652422D04508450B, 0x4E5814100BC31460, 0x3B27B8AF68E01031, 0x8C0BFE72836BEF99, 0x24719D233B0B73C3, 0x89FD8EC7541057F7, 0xE9EB05DE2C060E04, 0x82604587B8086818, 0x327B709A600A0E06, 0x69791C9646F632DF, 0xCAA10210E5F0803A, 0xA51846AE650F6F41, 0x53C4398F9EEF70F2, 0x294A508EE08062D, 0xC6EB02C4E1041F92, 0x9CB22020CFFD082E, 0x937D1C2084F2581A, 0xB44E4128145007C0, 0x5E384DB8E5A5A2, 0xD86D7DB9A8F6E8EB, 0x1C3A5BAB004E0097, 0x59838150323D701, 0x6F711AC903B6011E, 0xFEE91420E04074A1, 0x4D440B0818616651, 0xAEAB8EF2EFF9B3DC, 0x210F2E0972561333, 0xF8B57A9685A15280, 0x5C403410C4106B80, 0x5E065C8D020A8C01, 0x791C388601421206, 0xA043EC039587147D, 0x18C0A40744300FE4, 0x21E28AEDA8D00698, 0x22723A2E42021C25, 0x441A2151B20E0043, 0x5DC3E0B4694A0187, 0x34D1F09A27E0D2FE, 0x606D029090D0048, 0xE863220910051055, 0x2B6CB60B04063E86, 0x643054DB6C16180D, 0x791C388601421206, 0x69DFDD4D701E1601, 0x330C10C8F11B2200, 0xF43CBC84500C4D02, 0x3D40145521470103};

void genMagicNumbers();
void genBishopDestBB();
void genBishopDestIntersections();
void genRookDestIntersections();
void loadIntersections();
void calculateDestBB();//From intersections
void genIB();
void genLineInfo();
void genSlideDest();

using namespace std;
static uint16_t randResetSeedCounter = 0;

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
        rv += (bb & 1);
        bb>>=1;
    }
    return rv;
}

uint64_t rand64(){
    if(( (randResetSeedCounter++)%0x8000 )== 0){
        srand(time(NULL));
        //printf("resetting seed\n");
    }
    return (uint64_t)rand() | (uint64_t)rand()<<15 | (uint64_t)rand()<<30 | (uint64_t)rand()<<45 | (uint64_t)rand()<<60;
}

int main() {
    genSlideDest();
    return 0;
}

void genSlideDest(){
    printf("Rook full dest bbs:\n");
    for(int src = 0; src<64; src++){
        uint64_t dest_bb = 0;
        for(int i = src+8; i<64; i+=8) dest_bb |= SHIFT(i);
        for(int i = src-8; i>=0; i-=8) dest_bb |= SHIFT(i);
        for(int i = src+1; i%8>0; i++) dest_bb |= SHIFT(i);
        for(int i = src-1; i%8<7 && i>=0; i--) dest_bb |= SHIFT(i);
        printf("0x%llX, ",dest_bb);
    }

    printf("Bishop full dest bbs:\n");
    for(int src = 0; src < 64; src++){
        uint64_t dest_bb = 0;
        for(int i = src+7; i<64 && i%8>=0 && i%8<7; i+=7) dest_bb |= SHIFT(i);
        for(int i = src+9; i<64 && i%8<=7 && i%8>0; i+=9) dest_bb |= SHIFT(i);
        for(int i = src-7; i>=0 && i%8<=7 && i%8>0; i-=7) dest_bb |= SHIFT(i);
        for(int i = src-9; i>=0 && i%8<7 && i%8>=0; i-=9) dest_bb |= SHIFT(i);
        printf("0x%llX, ",dest_bb);
    }
}

void genLineInfo(){
    printf("RANKS\n");
    for(int i = 0; i<8; i++){
        printf("0x%llx, ",0x0101010101010101 << i);
    }
    printf("\n\nFILES\n");
    for(int i = 0; i<8; i++){
        printf("0x%llx, ",0xFF << i*8);
    }

    printf("\n\nDIAGONALS NE\n");
    uint64_t bb;
    for(int i = 0; i<8; i++){
        bb = SHIFT(i);
        for(int j = i+7; j%8<7 && j < 64; j+= 7) bb |= SHIFT(j);
        printf("0x%llX, ",bb);
    }
    for(int i = 15; i<64; i+=8){
        bb = SHIFT(i);
        for(int j = i+7; j%8<7 && j < 64; j+= 7) bb |= SHIFT(j);
        printf("0x%llX, ",bb);
    }
    printf("\n\nDIAGONALS NW\n");
    for(int i = 7; i>=0; i--){
        bb = SHIFT(i);
        for(int j = i+9; j%8>0 && j < 64; j+= 9) bb |= SHIFT(j);
        printf("0x%llX, ",bb);
    }
    for(int i = 8; i<64; i+=8){
        bb = SHIFT(i);
        for(int j = i+9; j%8>0 && j < 64; j+= 9) bb |= SHIFT(j);
        printf("0x%llX, ",bb);
    }

    printf("\n\nDIAGONALS NE LOOKUP\n");
    for(int i = 0; i<8; i++){
        for(int j = i; j<i+8; j++) printf("%d, ",j);
    }

    printf("\nDIAGONALS NW LOOKUP\n");
    for(int i = 0; i<8; i++){
        for(int j = i+7; j>=i; j--) printf("%d, ", j);
    }


}
void genIB(){
    FILE* fptr =fopen("ib.txt","w");

    for(int m = 0; m<64; m++){
        for(int n = 0; n<64; n++){
            int src = min(m,n);
            int dest = max(m,n);

            uint64_t ib = 0;
            if(src==dest){
                ib = 0;
            } else if(src%8==dest%8){
                for(int i = src+8; i<dest; i+=8) ib |= SHIFT(i);
            } else if(src/8 == dest/8){
                for(int i = src+1; i<dest; i++) ib |= SHIFT(i);
            } else {
                uint64_t prov_bb = 0;
                for(int i = src + 9; i%8>0 && i<64; i+= 9){
                    if(i==dest){
                        ib = prov_bb;
                    }
                    prov_bb |= SHIFT(i);
                }
                prov_bb = 0;
                for(int i = src + 7; i%8<7 && i<64; i+= 7){
                    if(i==dest){
                        ib = prov_bb;
                    }
                    prov_bb |= SHIFT(i);
                }
            }

            fprintf(fptr, "0x%llX\n",ib);
        }
    }

    fclose(fptr);
}

void calculateDestBB(){
    FILE* fptr = fopen("rookDestReal.txt","w");
    for(int src = 0; src<64; src++){
        int num = SHIFT(count_bits(rookDestTrunc[src]));

        uint64_t output_bb;
        for(int i = 0; i<num; i++){
            output_bb = 0;
            for(int d = src-8;d>=0;d-=8){
                output_bb |= SHIFT(d);
                if(rookDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src+8;d<64;d+=8){
                output_bb |= SHIFT(d);
                if(rookDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src-1;d%8<7 && d>=0 ;d--){
                output_bb |= SHIFT(d);
                if(rookDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src+1;d%8>0;d++){
                output_bb |= SHIFT(d);
                if(rookDestInt[src][i] & SHIFT(d)) break;
            }
            fprintf(fptr,"0x%llX\n",output_bb);
        }
    }

    fclose(fptr);
    fptr = fopen("bishopDestReal.txt","w");

    for(int src = 0; src<64; src++){
        int num = SHIFT(count_bits(bishopDestTrunc[src]));

        uint64_t output_bb;
        for(int i = 0; i<num; i++){
            output_bb = 0;
            for(int d = src+9;d<64 && d%8>0;d+=9){
                output_bb |= SHIFT(d);
                if(bishopDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src-9;d>=0 && d%8<7;d-=9){
                output_bb |= SHIFT(d);
                if(bishopDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src+7;d<64 && d%8<7;d+=7){
                output_bb |= SHIFT(d);
                if(bishopDestInt[src][i] & SHIFT(d)) break;
            }
            for(int d = src-7;d>=0 && d%8>0;d-=7){
                output_bb |= SHIFT(d);
                if(bishopDestInt[src][i] & SHIFT(d)) break;
            }
            fprintf(fptr,"0x%llX\n",output_bb);
        }
    }

    fclose(fptr);
}

void genRookDestIntersections(){
    FILE* fptr = fopen("rookIntersections.txt","w");
    for(int src = 0; src<64; src++){
        uint64_t dest_bb = rookDestTrunc[src];
        //print_bb(dest_bb);
        int num_bits = count_bits(dest_bb);
        //printf("CB:%d\n",num_bits);

        for(uint16_t i = 0; i<SHIFT(num_bits); i++){
            uint16_t num_to_shift = i;
            int pos = 0;
            uint64_t int_bb = 0;
            while(num_to_shift){
                while(!(dest_bb & SHIFT(pos))){
                    pos++;
                }
                int_bb |= ((uint64_t)(num_to_shift&1))<<pos;
                num_to_shift>>=1;
                pos++;
            }
            fprintf(fptr, "0x%llX\n",int_bb);
        }
    }

    fclose(fptr);
}

void genBishopDestBB(){
    for(int src = 0; src < 64; src++){
        uint64_t dest_bb = 0;
        for(int i = src+7; i<56 && i%8>0 && i%8<7; i+=7) dest_bb |= SHIFT(i);
        for(int i = src+9; i<56 && i%8<7 && i%8>0; i+=9) dest_bb |= SHIFT(i);
        for(int i = src-7; i>7 && i%8<7 && i%8>0; i-=7) dest_bb |= SHIFT(i);
        for(int i = src-9; i>7 && i%8<7 && i%8>0; i-=9) dest_bb |= SHIFT(i);
        printf("0x%llX, ",dest_bb);
    }
}

void genBishopDestIntersections(){
    FILE* fptr = fopen("bishopIntersections.txt","w");
    for(int src = 0; src<64; src++){
        uint64_t dest_bb = bishopDestTrunc[src];
        //print_bb(dest_bb);
        int num_bits = count_bits(dest_bb);
        //printf("CB:%d\n",num_bits);

        for(uint16_t i = 0; i<SHIFT(num_bits); i++){
            uint16_t num_to_shift = i;
            int pos = 0;
            uint64_t int_bb = 0;
            while(num_to_shift){
                while(!(dest_bb & SHIFT(pos))){
                    pos++;
                }
                int_bb |= ((uint64_t)(num_to_shift&1))<<pos;
                num_to_shift>>=1;
                pos++;
            }
            fprintf(fptr, "0x%llX\n",int_bb);
        }
    }

    fclose(fptr);
}

uint64_t best[128];
int best_w[128];
int num_perfectR;//Number of minimal width rook magic numbers
int num_perfectB;//Number of minimal width bishop magic numbers
FILE* MNlogptr;

void logMN(){
    fprintf(MNlogptr,"Number of minimal rook MNs: %d\n",num_perfectR);
    fprintf(MNlogptr,"Number of minimal bishop MNs: %d\n",num_perfectB);
    fprintf(MNlogptr,"Rook MN widths:\n");
    for(int i = 0 ;i<64; i++) fprintf(MNlogptr,"%d, ",best_w[i]);
    fprintf(MNlogptr,"\n\nRook magic numbers:\n");
    for(int i = 0; i<64; i++){
        fprintf(MNlogptr,"0x%llX, ",best[i]);
    }

    fprintf(MNlogptr,"\n\nBishop MN widths:\n");
    for(int i = 64 ;i<128; i++) fprintf(MNlogptr,"%d, ",best_w[i]);
    fprintf(MNlogptr,"\n\nBishop magic numbers:\n");
    for(int i = 64; i<128; i++){
        fprintf(MNlogptr,"0x%llX, ",best[i]);
    }
    fprintf(MNlogptr,"\n\n\n");
}

void genMagicNumbers(){
    for(int i = 0; i<64; i++){
        best[i] = rookMN[i];
        best[i+64] = bishopMN[i];
        best_w[i] = rookMN_w[i];
        best_w[i+64] = bishopMN_w[i];
    }

    loadIntersections();
    MNlogptr = fopen("MN_gen_log.txt","w");
    uint64_t test_magic;
    set<uint16_t> used_indices;

    num_perfectR = 11;
    num_perfectB = 52;

    while(1){
        test_magic = rand64();
        //Rooks
        for(int src = 0; src<64; src++){
            if(best_w[src]==count_bits(rookDestTrunc[src])) continue;
            used_indices.clear();
            int num_entries = SHIFT(count_bits(rookDestTrunc[src]));


            for(int i = 0; i<num_entries; i++){
                uint16_t prod = (test_magic * rookDestInt[src][i]) >>(64-(best_w[src]-1));

                if(used_indices.find(prod) != used_indices.end()) break;
                used_indices.insert(prod);
                if(i==num_entries-1){
                    best_w[src]--;
                    best[src] = test_magic;
                    printf("New rook MN for %d: Width=%d MN=%llX\n",src,best_w[src], test_magic);
                    logMN();
                    if(best_w[src]==count_bits(rookDestTrunc[src])) num_perfectR++;
                }
            }

        }
        //Bishops
        for(int src = 0; src<64; src++){
            if(best_w[src+64]==count_bits(bishopDestTrunc[src])) continue;
            used_indices.clear();
            int num_entries = SHIFT(count_bits(bishopDestTrunc[src]));


            for(int i = 0; i<num_entries; i++){
                uint16_t prod = (test_magic * bishopDestInt[src][i]) >>(64-(best_w[src+64]-1));

                if(used_indices.find(prod) != used_indices.end()) break;
                used_indices.insert(prod);
                if(i==num_entries-1){
                    best_w[src+64]--;
                    best[src+64] = test_magic;
                    printf("New bishop MN for %d: Width=%d MN=%llX\n",src,best_w[src+64], test_magic);
                    logMN();
                    if(best_w[src+64]==count_bits(bishopDestTrunc[src])) num_perfectB++;
                }
            }

        }

    }
    fclose(MNlogptr);
}

void loadIntersections(){
    
    FILE* fptr = fopen("bishopIntersections.txt","r");
    uint64_t file_input;

    if(fptr == NULL){
        cerr << "Failed to open bishopIntersections.txt" << endl;
        exit(1);
    }

    for(int src = 0; src<64; src++){
        int num_entries = SHIFT(count_bits(bishopDestTrunc[src]));
        bishopDestInt[src] = new uint64_t[num_entries];

        for(int i = 0; i<num_entries; i++){
            fscanf(fptr,"%llX",&file_input);
            bishopDestInt[src][i] = file_input;
        }
    }

    fclose(fptr);
    fptr = fopen("rookIntersections.txt","r");

    if(fptr == NULL){
        cerr << "Failed to open rookIntersections.txt" << endl;
        exit(1);
    }

    for(int src = 0; src<64; src++){
        int num_entries = SHIFT(count_bits(rookDestTrunc[src]));
        rookDestInt[src] = new uint64_t[num_entries];

        for(int i = 0; i<num_entries; i++){
            fscanf(fptr,"%llX",&file_input);
            rookDestInt[src][i] = file_input;
        }
    }

    fclose(fptr);  
}
