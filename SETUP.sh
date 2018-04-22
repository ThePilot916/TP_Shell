#!/bin/bash


make clean
make
cd DEBUG/
make clean
make
cd ..
cd NONINT/
make clean
make
cd ..
cp DEBUG/.pilotshell.dbg ../TP_Shell-master/
cp NONINT/.pilotshell.nin ../TP_Shell-master/
