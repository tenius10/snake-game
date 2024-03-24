#include "console.h"
#include "position.h"
#include "snake.h"
#include "appleSupport.h"

#define BOARD_SIZE 20
#define APPLE_SCORE 10
#define MOVE_DELAY 15
using namespace std;
using namespace console;

int dx[4] = {0, 0, -1, 1};
int dy[4] = {-1, 1, 0, 0};

enum Direction{ UP=0, DOWN=1, LEFT=2, RIGHT=3, NONE=4 };


//BOARD_SIZE를 기반으로 string이 중앙 정렬되도록 y행에 출력한다.
void printCenter(string s, int y){
    int bm=(BOARD_SIZE) / 2;
    int sm=s.size() / 2;
    if(s.size()>BOARD_SIZE){
        string st=s.substr(sm-bm, BOARD_SIZE);
        draw(0, y, st);
    }
    else{
        int idx = bm - sm;
        draw(idx, y, s);
    }   
}

// 게임 오버 UI 출력
void gameOver(){
    int y=(BOARD_SIZE-1)/2;
    printCenter("YOU LOSE!", y);
    printCenter("Try again? (Enter)", y+1);
}

// 게임 클리어 UI 출력
void gameClear(){
    int y=(BOARD_SIZE-1)/2;
    printCenter("You Win!", y);
    printCenter("Congraturation!", y+1);
}

// 회전하려는 방향이 현재 방향과 수직이면 이동 가능
bool isValidDirection(Direction curDir, Direction inputDir){
    switch(curDir){
        case UP:
        case DOWN:
            return (inputDir!=UP && inputDir!=DOWN);
        case LEFT:
        case RIGHT:
            return (inputDir!=LEFT && inputDir!=RIGHT);
        default:
            return false;
    }
}

// 뱀의 머리가 몸통이나 벽에 부딪히지 않았는지 검사
bool isValidPosition(Snake snake, Position nextPos, Position applePos){
    bool isWall=nextPos.x<1 || nextPos.x>BOARD_SIZE-2 || nextPos.y<1 || nextPos.y>BOARD_SIZE-2;
    bool isBody=snake.isBody(nextPos);
    //만약 겹친 부분이 body이기는 한데 tail이고 apple 위치가 아니라면 body에 부딪힌 거 아님
    bool isTail=(nextPos!=applePos) && (nextPos==snake.front());
    return (!isWall && !isBody) || isTail;
}

void drawMap(string map[][BOARD_SIZE]){
    for(int i=0;i<BOARD_SIZE;i++){
        for(int j=0;j<BOARD_SIZE;j++){
            draw(j, i, map[i][j]);
        }
    }
}

/**
 * snake 게임 시작 함수
 * 반환 : esc를 눌러서 종료했는지 여부
*/
bool startSnake(){
    // 화면 초기화
    clear();
    
    // 게임 상태 초기화
    int score = 0;
    Snake snake;
    Direction curDir=RIGHT;
    Direction inputDir;

    //맵 초기화
    string map[BOARD_SIZE][BOARD_SIZE];
    map[0][0]="┏";
    map[0][BOARD_SIZE-1]="┓";
    map[BOARD_SIZE-1][0]="┗";
    map[BOARD_SIZE-1][BOARD_SIZE-1]="┛";
    for(int i=1;i<BOARD_SIZE-1;i++){
        map[0][i]=map[BOARD_SIZE-1][i]="━";
        map[i][0]=map[i][BOARD_SIZE-1]="┃";
    }
    
    //사과 생성
    AppleSupport appleSupport;
    appleSupport.erase(snake.rear());
    Position applePos=appleSupport.getRandomApple();

    map[applePos.y][applePos.x]="●";

    //화면 그리기
    drawMap(map);

    int waitCnt=0;  //루프를 몇 번 기다릴 건지
    while(true){
        // 0. 대기
        wait();
        waitCnt++;
        
        if(waitCnt<MOVE_DELAY) continue;
        else waitCnt=0;

        // 1. 입력 받기
        
        //ESC 누르면 즉시 게임 종료
        if(key(K_ESC)) return true;

        //화살표 입력
        inputDir=NONE;
        if(key(K_UP)) inputDir=UP;
        if(key(K_DOWN)) inputDir=DOWN;
        if(key(K_LEFT)) inputDir=LEFT;
        if(key(K_RIGHT)) inputDir=RIGHT;

        // 2. 게임 상태 수정

        // 2-1. 방향 틀기
        
        //올바른 방향이면, 방향 틀기
        if(inputDir!=NONE && (snake.length() == 1 || isValidDirection(curDir, inputDir))){
            curDir = inputDir;
        }
    
        // 2-2. 뱀 위치 수정

        //뱀의 머리 좌표 이동
        Position nextHead={
            (snake.rear().x)+dx[curDir],
            (snake.rear().y)+dy[curDir]
        };

        //뱀이 몸통이나 벽에 부딪혔다면, 게임 오버
        if(!isValidPosition(snake, nextHead, applePos)){
            map[applePos.y][applePos.x]=" ";
            gameOver();
            break;
        }

        snake.push(nextHead);  //뱀 머리 삽입
        appleSupport.erase(nextHead);

        if(snake.rear() != applePos){
            //사과를 먹은게 아니라면 뱀 꼬리 삭제
            Position tail=snake.pop();
            appleSupport.insert(tail);
            map[tail.y][tail.x]=" ";
        }

        // 2-3. 사과 판정
        if(snake.rear()==applePos){
            snake.addLength(1);
            score+=APPLE_SCORE;
            if(snake.length()>=(BOARD_SIZE-1)*(BOARD_SIZE-1)){
                map[applePos.y][applePos.x]=" ";
                gameClear();
                break;
            }
            else{
                //사과 새로 생성
                applePos=appleSupport.getRandomApple();
            }
        }

        // 3. 화면 그리기
        
        //뱀 표시
        for(int i=0;i<snake.length();i++){
            Position pos=snake.peek(i);
            map[pos.y][pos.x]="■";
        }

        //사과 표시
        map[applePos.y][applePos.x]="●";

        drawMap(map);

        //점수 UI 표시
        printCenter("Score: "+to_string(score), BOARD_SIZE);
    }

    return false;
}

void startGame(){
    // console 초기화
    init();

    // 처음 들어왔을 때는 바로 시작
    bool isEsc = startSnake();
    if(isEsc) return;

    // 입력 기다리기
    // Enter 누르면 재시작, Esc 누르면 종료
    while(true){
        if(key(K_ENTER)) {
            isEsc = startSnake();
            if(isEsc) return;
        }
        if(key(K_ESC)) break;
        // 대기
        wait();  
    }
}