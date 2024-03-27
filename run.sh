#! /usr/bin/zsh

g++ cantree.cpp -std=c++17
./a.out ipe.txt perf.txt res.txt
perl file_ok.pl
python3 run.py