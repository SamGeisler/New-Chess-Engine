#include "move.h"
#include "Lookup.h"

std::ostream& operator<<(std::ostream& stream, const Move& move){
    stream << DebugTools::notation(move.src) << " -> " << DebugTools::notation(move.dest);
    if(move.promo){
        std::string_view promotion;
        switch(move.promo){
            case 1: promotion = "knight"; break;
            case 2: promotion = "bishop"; break;
            case 3: promotion = "rook"; break;
            case 4: promotion = "queen"; break;
            default: promotion = "INVALID";
        }
        stream << " - "<< promotion << ".";
    }

    return stream;
}