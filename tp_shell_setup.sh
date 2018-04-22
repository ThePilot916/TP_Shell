#!/bin/bash
make clean
make
cd DEBUG/
make clean
make
cd ..
cp DEBUG/pilotshell.dbg ../tpshell/
