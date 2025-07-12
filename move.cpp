#include "move.h"

std::ostream& operator<<(std::ostream& stream, const Move& move){
    stream << notation(move.src) << " -> " << notation(move.dest);
    if(move.promo){
        std::stringView promotion;
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