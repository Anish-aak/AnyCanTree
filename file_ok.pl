#! /usr/bin/perl

system('perl -pli -e "s/\s*$//" res.txt');
system('perl -pli -e "s/^\s*//" res.txt');


