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
cp DEBUG/.pilotshell.dbg ../tpshell/
cp NONINT/.pilotshell.nin ../tpshell/
