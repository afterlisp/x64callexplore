echo build and launch main.exe
@echo off
nasm -fwin64 myfunc.asm
g++ -Wall -no-pie myfunc.obj main.cpp -o main.exe
rem build other codes but do not launch (see README)
rem use fixed length
g++ myfunc.obj main2.cpp -o main2.exe
rem use more than 32 bits 
nasm -fwin64 myfunc2.asm
g++ -Wall -no-pie myfunc2.obj main2.cpp -o main3.exe
rem correct with rax
g++ -Wall -no-pie myfunc2.obj main3.cpp -o main4.exe
main.exe