#ifndef APPLE_SUPPORT_H
#define APPLE_SUPPORT_H

#include "position.h"
#define BOARD_SIZE 20

//사과와 관련된 처리 모음
struct AppleSupport{
    static const int ARR_SIZE=(BOARD_SIZE - 2) * (BOARD_SIZE - 2);
    Position arr[ARR_SIZE];
    int size;

    AppleSupport();
    void insert(Position pos);
    void erase(Position pos);
    Position getRandomApple();
};

#endif