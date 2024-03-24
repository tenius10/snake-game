#ifndef SNAKE_H
#define SNAKE_H

#include "position.h"
#define BOARD_SIZE 20

struct Snake {
    static const int Q_SIZE = (BOARD_SIZE - 1) * (BOARD_SIZE - 1);
    int _front, _rear;
    int _length;
    Position body[Q_SIZE];
    
    Snake();
    bool isEmpty();
    void push(Position pos);
    Position pop();
    Position front();
    Position rear();
    Position peek(int idx);
    bool isBody(Position pos);
    int length();
    int addLength(int amount);
};

#endif