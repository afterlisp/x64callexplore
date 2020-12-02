echo build and launch main.exe
@echo off
nasm -fwin64 addfunc.asm
g++ -Wall -no-pie addfunc.obj add.cpp -o add.exe
add.exe
rem other codes construction
rem with lea
nasm -fwin64 addfunc2.asm
g++ -Wall -no-pie addfunc2.obj add.cpp -o add2.exe