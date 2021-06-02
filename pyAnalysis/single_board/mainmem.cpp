#include "board.h"

#include <iostream>
#include <queue>
#include <cmath>
#include <time.h>

using namespace SlidePuzzle;
using namespace std;

int Board::activeBoardCnt = 0;
int Board::totalBoardCnt = 0;

queue<Board*> que;            // queue of boards
Board* pboard;                // points to current most optimal board state
Board* ptestBoard;            // points to 'explored' variation of most optimal board state
 
typedef long long Nanos;
const Nanos NanosInSecond = 1e9;

Nanos getNanos() {
    timespec now;
    
    clock_gettime(CLOCK_MONOTONIC, &now);
    return (now.tv_sec * NanosInSecond + now.tv_nsec);
}

void resetQue() {
    while(!que.empty()) {
        if (que.front() != pboard) delete(que.front());
        que.pop();
    }
    que.push(pboard);
}

int main()
{
    cout << "tile,activeBoards,time,moves" << endl;
    pboard = new Board(4);
    pboard->shuffle(1000);
    
    pboard->clearPath();
    Nanos start = getNanos();
    cout << "0" << ", " << Board::getActiveBoardCnt() << ", " << getNanos() - start << ", " << pboard->getPath().length() << endl;
    
    que.push(pboard);
    
    char dirs[]    = {'u','d','l','r'};
    char oppdirs[] = {'d','u','r','l'};
    bool pboardChanged;
    int recentActiveBoardCnt;
    
    for (int tileNum = 1; tileNum < pow(pboard->getN(), 2); tileNum++) {
        recentActiveBoardCnt = Board::getActiveBoardCnt();
        while(pboard->getTileDist(tileNum) > 0) 
        {
            for(int dirNum = 0; dirNum < 4; dirNum++) 
            {
                ptestBoard = new Board(que.front());
                pboardChanged = false;
                
                bool hitWall = !ptestBoard->move(dirs[dirNum]);
                bool revMove = que.front()->getLastMove() == oppdirs[dirNum];
                bool revProg = ( (0 < (tileNum-1)/pboard->getN() && (tileNum-1)/pboard->getN() < pboard->getN()-1 && ptestBoard->getTileDist((tileNum-1)/pboard->getN()*pboard->getN()) > 0) ||
                                 ((tileNum-1)/pboard->getN() == pboard->getN()-1 && ptestBoard->getTileDist((pboard->getN()-2)*pboard->getN()) > 0) );
                
                if (!hitWall && !revMove && !revProg) 
                {  
                    que.push(ptestBoard);
                    
                    if (ptestBoard->getTileDist(tileNum) < pboard->getTileDist(tileNum)) 
                    {
                        recentActiveBoardCnt = max(recentActiveBoardCnt, Board::getActiveBoardCnt());
                        
                        if (que.front() != pboard) delete(pboard);
                        pboard = ptestBoard;
                        pboardChanged = true;
                        
                        resetQue();
                        
                        dirNum = 3;
                    }
                } 
                else {
                    delete(ptestBoard);
                }
                
                ptestBoard = NULL;
            }
            
            if(!pboardChanged) {
                if (que.front() != pboard) delete(que.front());
                que.pop();
            }
            
        }
        
        cout << tileNum << ", " << recentActiveBoardCnt << ", " << getNanos() - start << ", " << pboard->getPath().length() << endl;
    }
    
    cout << pow(pboard->getN(), 2) << ", " << Board::getActiveBoardCnt() << ", " << getNanos() - start << ", " << pboard->getPath().length() << endl;
    
    delete(pboard);
    
    return 0;
}
