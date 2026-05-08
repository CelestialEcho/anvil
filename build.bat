@echo off
g++ src/config/config.cpp src/anvil.cpp main.cpp -O2 -std=c++17 -o bin/anvil.exe
