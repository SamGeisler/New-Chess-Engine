#include "precomputed.h"
#include "lookup.h"
#include "magic_numbers.h"

#include "stdio.h"

bool loadRBDestinations();
bool loadInbetween();

namespace Precomputed {
    uint64_t inbetweenBB[64][64];
    std::vector<std::vector<uint64_t>> rookDestIntersected(64);
    std::vector<std::vector<uint64_t>> bishopDestIntersected(64);

    bool loadPrecompute(){
        return loadInbetween() && loadRBDestinations();
    }
}

bool loadRBDestinations(){
    FILE* destFptr = fopen("precomputed-data/rookDestReal.txt","r");//Destination bb based on intersections with other pieces
    FILE* intFptr = fopen("precomputed-data/rookIntersections.txt","r");//Possible intersections

    for(int src = 0; src<64; src++){
        int hashCapacity = SHIFT(MagicNumbers::rookWidths[src]);
        int numEntries = SHIFT(BitboardOps::countBits(BitboardLookup::ROOK_DEST_TRUNC[src]));

        Precomputed::rookDestIntersected[src].resize(hashCapacity);

        for(int i = 0; i<numEntries; i++){
            uint64_t destBB;
            uint64_t intBB;
            fscanf(destFptr,"%llX", &destBB);
            fscanf(intFptr,"%llX", &intBB);

            int index = (MagicNumbers::rookNums[src] * intBB)>>(64-MagicNumbers::rookWidths[src]);
            Precomputed::rookDestIntersected[src][index] = destBB;
        }
    }

    fclose(destFptr);
    fclose(intFptr);
    destFptr = fopen("precomputed-data/bishopDestReal.txt","r");
    intFptr = fopen("precomputed-data/bishopIntersections.txt","r");

    for(int src = 0; src<64; src++){
        int hashCapacity = SHIFT(MagicNumbers::bishopWidths[src]);
        int numEntries = SHIFT(BitboardOps::countBits(BitboardLookup::BISHOP_DEST_TRUNC[src]));

        Precomputed::bishopDestIntersected[src].resize(hashCapacity);

        for(int i = 0; i<numEntries; i++){
            uint64_t destBB;
            uint64_t intBB;
            fscanf(destFptr,"%llX",&destBB);
            fscanf(intFptr,"%llX",&intBB);

            int index = (MagicNumbers::bishopNums[src] * intBB)>>(64-MagicNumbers::bishopWidths[src]);
            Precomputed::bishopDestIntersected[src][index] = destBB;
        }
    }

    fclose(destFptr);
    fclose(intFptr);

    return true;
}

bool loadInbetween(){
    FILE* fptr = fopen("precomputed-data/ib.txt","r");
    for(int i = 0; i<64; i++){
        for(int j = 0; j<64; j++){
            fscanf(fptr, "%llX",&(Precomputed::inbetweenBB[i][j]));
        }
    }
    
    fclose(fptr);

    return true;
}
