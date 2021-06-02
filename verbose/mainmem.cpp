#include "board.h"

#include <iostream>
#include <queue>
#include <cmath>
#include <time.h>

using namespace SlidePuzzle;
using namespace std;

int Board::activeBoardCnt = 0;
int Board::totalBoardCnt = 0;
time_t lastNow = 0; 

queue<Board*> que;            // queue of boards
Board* pboard;                // points to current most optimal board state
Board* ptestBoard;            // points to 'explored' variation of most optimal board state
 
void printStats(bool force = false) {
    time_t now = time(0);
    if (force || now != lastNow) {
        lastNow = now;
        cout << "activeBoardCnt = " << Board::getActiveBoardCnt() << " totalBoardCnt = " << Board::getTotalBoardCnt() << endl; 
    }
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
    printStats(true);
    
    pboard = new Board(4);
    pboard->shuffle(1000);
    //pboard->move("ululurdruldddlur");
    pboard->clearPath();
    
    printStats(true);
    cout << "SHUFFLED BOARD" << endl;
    pboard->printBoard();
    
    que.push(pboard);
    
    char dirs[]    = {'u','d','l','r'};
    char oppdirs[] = {'d','u','r','l'};
    bool pboardChanged;
    
    for (int tileNum = 1; tileNum < pow(pboard->getN(), 2); tileNum++) {
        while(pboard->getTileDist(tileNum) > 0) 
        {
            for(int dirNum = 0; dirNum < 4; dirNum++) 
            {
                ptestBoard = new Board(que.front());
                pboardChanged = false;
                printStats();
                
                bool hitWall = !ptestBoard->move(dirs[dirNum]);
                bool revMove = que.front()->getLastMove() == oppdirs[dirNum];
                bool revProg = ( (0 < (tileNum-1)/pboard->getN() && (tileNum-1)/pboard->getN() < pboard->getN()-1 && ptestBoard->getTileDist((tileNum-1)/pboard->getN()*pboard->getN()) > 0) ||
                                 ((tileNum-1)/pboard->getN() == pboard->getN()-1 && ptestBoard->getTileDist((pboard->getN()-2)*pboard->getN()) > 0) );
                
                if (!hitWall && !revMove && !revProg) 
                {  
                    que.push(ptestBoard);
//                    cout << dirs[dirNum] << " tile:" << tileNum  << " tileDist: " << ptestBoard->getTileDist(tileNum) << endl;
//                    printStats(true);
//                    ptestBoard->printBoard();
                    
                    if (ptestBoard->getTileDist(tileNum) < pboard->getTileDist(tileNum)) 
                    {
                        printStats(true);
                        cout << "new moves: " << ptestBoard->getPath().substr(pboard->getPath().length(), ptestBoard->getPath().length()-pboard->getPath().length()) << endl;
                        cout << "tile: " << tileNum  << " tileDist: " << ptestBoard->getTileDist(tileNum) << endl;
                        ptestBoard->printBoard();
                        
                        if (que.front() != pboard) delete(pboard);
                        pboard = ptestBoard;
                        pboardChanged = true;
                        resetQue();
                        
//                        printStats(true);
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
    }

    printStats(true);
    cout << "SOLVED BOARD in " << pboard->getPath().length() << " moves." << endl;
    //pboard->printBoard();
    
    delete(pboard);
    printStats(true);
     
    return 0;
}
