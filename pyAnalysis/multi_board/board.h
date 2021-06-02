#include <string>
#include <stdio.h>      
#include <stdlib.h>    
#include <cmath>
#include <time.h>
using namespace std;

namespace SlidePuzzle {
    
    
class Board {
public:
    Board() 
    {
        ++activeBoardCnt;
        ++totalBoardCnt;
    } // default empty constructor 
    
    Board(int n) : n(n) // constructor for solved n by n board
    {
        ++activeBoardCnt;
        ++totalBoardCnt;
        if (n > 0) {
            position = new int*[n];

            for (int i=0;i<n;i++)
            {
                position[i] = new int[n];
                for (int j=0;j<n;j++)
                {
                    position[i][j] = n*i+j+1;
                }
            }
            position[n-1][n-1] = 0;
        }
    }

    Board(Board* otherBoard) { // copy constructor (uses pointer value)
        ++activeBoardCnt;
        ++totalBoardCnt;
        
        n = (*otherBoard).getN();
        path = (*otherBoard).getPath();

        position = new int*[n];

        for(int i=0;i<n;i++) 
        {
            position[i] = new int[n];
        }

        for (int i=0;i<n;i++)
        {
            for (int j=0;j<n;j++)
            {
                position[i][j] = (*otherBoard).getPosition(i,j);
            }
        }

        dist = (*otherBoard).getDist();
    }

    ~Board() { // destructor
        --activeBoardCnt;
        if (position) 
        {
            for(int i = 0; i < n; i++) 
            {
                delete position[i];
            }
            delete position;
        }
    }

    bool move(char dir) // move up, down, left, or right (direction space moves, not direction of the slide)
    {
        bool ret = false; // returns false if board cannot be moved in given direction
        
        for (int i=0; i<n; i++) { // search for position of space, if possible move and return true
            for (int j=0; j<n; j++) {
                if (position[i][j] == 0) {
                    if (dir == 'u' && i > 0) {
                        position[i][j] = position[i-1][j];
                        position[i-1][j] = 0;
                        ret = true;
                    }
                    else if (dir == 'd' && i < n-1) {
                        position[i][j] = position[i+1][j];
                        position[i+1][j] = 0;
                        ret = true;
                    }
                    else if (dir == 'l' && j > 0) {
                        position[i][j] = position[i][j-1];
                        position[i][j-1] = 0;
                        ret = true;
                    } 
                    else if (dir == 'r' && j < n-1) {
                        position[i][j] = position[i][j+1];
                        position[i][j+1] = 0;
                        ret = true;
                    }
                    
                    if (ret) {
                        path += dir;
                        calcDist();
                        break; // exit loop after moving
                    }
                }
            }
            
            if (ret) {                            
                break; // exit nested loop after moving
            }
       }

        return ret;
    }
    
    void move(string dirs) {
        for (int i = 0; i < dirs.length(); i++) {
            move(dirs[i]);
        }
    }

    void shuffle(int cnt) // shuffle board cnt valid moves
    {
        char dirs[] = {'u','d','l','r'}; // up, down, left and right directions
        char oppDir; // opposite direction of randomized move direction
        
        typedef long long Nanos;
        const Nanos NanosInSecond = 1e9;
        timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        srand((now.tv_sec * NanosInSecond + now.tv_nsec));

        while (cnt > 0) { // countown cnt randomized moves
            int num = rand() % 4; // random 
            if (dirs[num] != oppDir && move(dirs[num])) {
                cnt--;
                if (num == 0)
                    oppDir = 'd';
                else if (num == 1)
                    oppDir = 'u';
                else if (num == 2)
                    oppDir = 'r';
                else if (num == 3)
                    oppDir = 'l';
            }
        }
        calcDist();
    }

    int getN() // get board dimension size
    {
        return n;
    }

    void calcDist() // calculate and set manhattan distance of the entire board
    {
        dist = 0; //path.length();
        for (int i=0;i<n;i++) {
            for (int j=0; j<n; j++) {
                int val = position[i][j];
                if (val != 0) {
                    int solutionRow = (val-1)/n;
                    int solutionCol = val-1-(solutionRow*n);

                    dist += abs(i-solutionRow) + abs(j-solutionCol);
                }
            }
        }
    }

    int getDist() // get manhattan distance of the entire board
    {
        return dist;
    }
    
    int getTileDist(int maxTile) // get 'partial' manhattan distance of tiles 1 through maxTile inclusive
    {
        int* ptilePos;
        int tilePos [2];
        int tileDist = 0;
        
        for (int i = 1; i <= maxTile; i++) { // for each included tile
            ptilePos = getTilePos(i);
        
            tilePos[0] = *ptilePos;
            tilePos[1] = *(++ptilePos);
            
            int solutionRow = (i-1)/n;
            int solutionCol = i-1-(solutionRow*n);
            
            //printf("hi %d %d %d %d %d \n", i, tilePos[0], solutionRow, tilePos[1], solutionCol);
            tileDist += abs(tilePos[0]-solutionRow) + abs(tilePos[1]-solutionCol);
        }
        
        return tileDist;
    }

    void clearPath() // reset board path to empty string regardless of positon
    {
        //dist -= path.length();
        path = "";
    }

    string getPath() // get current board path
    {
        return path;
    }

    char getLastMove() // get most recent move direction from path
    {
        char ret;
        if (path.length() > 0) ret = path.at(path.length()-1);
        return ret;
    }
    
    int getPosition(int i, int j) // get 2d board position array (actual position is private)
    {
        return position[i][j];
    }
    
    int* getTilePos(int tileNum) // return position of tile
    {
        int* ret = new int[2];
        
        if (tileNum >= pow(n,2) || tileNum < 0) { 
            printf("%d tile not found on board\n", tileNum);
            ret = NULL;
            return ret;
        }
        
        for (int j=0; j<n; j++) {
            for (int i=0; i<n; ++i) {
                if(tileNum == position[i][j]) {
                    ret[0] = i;
                    ret[1] = j;
                    return ret;
                }
            }
        }
    }

    void printBoard() { // print a formatted board for display/debugging
        for (int i=0;i<n;i++)
        {
            printf("[");
            for (int j=0;j<n-1;j++)
            {
                printf("%2d, ", position[i][j]);
            }
            printf("%2d]\n", position[i][n-1]);
        }
        
        string path;
        
        if(this->path == "") path = "empty";
        else path = getPath();
        
        printf("%d, %s\n\n", getDist(), path.c_str());
        
    }
    
    static int activeBoardCnt;
    static int getActiveBoardCnt () { return activeBoardCnt; }
    
    static int totalBoardCnt;
    static int getTotalBoardCnt () { return totalBoardCnt; }

private:
    int n = 0;
    int dist = 0;
    string path;
    int** position = NULL;
    
};

} // namespace
