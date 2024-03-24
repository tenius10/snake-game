#include "position.h"

bool Position::operator==(Position other){
    return x==other.x && y==other.y;
}
bool Position::operator!=(Position other){
    return x!=other.x || y!=other.y;
}