g++ board.h mainmem.cpp
rm -r output*.csv

NBOARDS=100
for ((i = 0 ; i < $NBOARDS ; i++)); do
    OUTPUTNAME=`printf "output%03d.csv" "$i"`
   ./a.out > $OUTPUTNAME
done

python slidePuzzlePlot.py
