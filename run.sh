#! /usr/bin/zsh

g++ sortAux.cpp -std=c++17
./a.out ip.txt > sortedip.txt
g++ cantree.cpp -std=c++17
./a.out sortedip.txt perf.txt res.txt
perl file_ok.pl
python3 run.py