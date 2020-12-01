echo build and launch hello.exe
@echo off
nasm -fwin64 hello.asm & gcc hello.obj -o hello.exe && hello