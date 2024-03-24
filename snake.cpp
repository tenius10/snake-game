#include "position.h"
#include "snake.h"

Snake::Snake(){
    _front=_rear=-1;
    _length=1;
    push({(BOARD_SIZE-1) / 2, (BOARD_SIZE-1) / 2});
}

bool Snake::isEmpty(){
    return _front == _rear;    
}

void Snake::push(Position pos){
    _rear=(_rear+1)%Q_SIZE;
    body[_rear]=pos;
}

Position Snake::pop(){
    _front=(_front+1)%Q_SIZE;
    return body[_front];
}

Position Snake::front(){
    return body[(_front+1)%Q_SIZE];
}

Position Snake::rear(){
    return body[(_rear)%Q_SIZE];
}

Position Snake::peek(int idx){
    return body[(_front+1+idx)%Q_SIZE];
}
    
bool Snake::isBody(Position pos){
    for(int i=0;i<_length;i++){
        Position tmp=peek(i);
        if(tmp==pos) return true;
    }
    return false;
}
    
int Snake::length(){
    return _length;
}

int Snake::addLength(int amount){
    _length+=amount;
}