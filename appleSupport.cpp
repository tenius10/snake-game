#include <cstdlib>
#include <ctime>
#include "position.h"
#include "appleSupport.h"

AppleSupport::AppleSupport(){
    size=0;
    for(int i=1;i<BOARD_SIZE-1;i++){
        for(int j=1;j<BOARD_SIZE-1;j++){
            arr[size++]={i, j};
        }
    }
}

void AppleSupport::insert(Position pos){
    if(size<ARR_SIZE){
        arr[size++]=pos;
    }
}

void AppleSupport::erase(Position pos){
    for(int i=0;i<size;i++){
        if(arr[i]==pos){
            arr[i]=arr[size-1];
            size--;
        }
    }
}

Position AppleSupport::getRandomApple(){
    srand(time(nullptr));
    int randIdx=rand()%size;
    return arr[randIdx];
}