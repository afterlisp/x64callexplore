echo build and launch test_chrono.exe
del addfunc.obj
del addfunc2.obj
del addfunc3.obj
del test_chrono.o
del test_chrono.exe
nasm -fwin64 addfunc.asm
nasm -fwin64 addfunc2.asm
nasm -fwin64 addfunc3.asm
nasm -fwin64 addfunc4.asm
g++ -O3 -o test_chrono.exe test_chrono.cpp  addfunc.obj addfunc2.obj addfunc3.obj addfunc4.obj -I"C:\Tools\boost_1_74_0\boost_1_74_0" -L"C:\Tools\boost_1_74_0\boost_1_74_0\stage\lib" -lboost_chrono-mgw9-mt-x64-1_74 -lboost_date_time-mgw9-mt-x64-1_74

