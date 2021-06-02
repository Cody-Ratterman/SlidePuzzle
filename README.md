# SlidePuzzle
Solves and NxN slide puzzles using cpp, outputs data to .csv, and generates plots analyzing one or multiple puzzles using python.

Compile board.h and mainmem.cpp before running a model.
If run.sh script exists, it will compile, run the model, output to .csv files, and create plots using python.

Extensive research has been done to solve slide puzzles in the least amount of moves possible.
The goal of this model is in many ways the opposite, time and memory efficiency is a higher priority than number of moves.
To achieve this the algorithm minimizes the distance for tile 1 from its solved position, then minimizes the sum for distances of tile 1 and 2, 
then minimizes the sum of tiles 1 thorugh 3, etc. Less active boards are explored to minimize memory and explore less total solutions.
This means smaller boards can be soved faster than more extenive path finding algorithms and larger boards can be solved using less memory.

Further work is being done, especially to optimize the final two rows where tiles must rotate around instead of being stored in their solved location.
