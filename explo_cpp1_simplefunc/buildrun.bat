echo build and launch main.exe
@echo off
nasm -fwin64 myfunc.asm
g++ -Wall -no-pie myfunc.obj main.cpp -o main.exe
main.exe