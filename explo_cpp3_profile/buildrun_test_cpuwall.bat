echo build and launch test_cpuwall.exe
del addfunc.obj
del addfunc2.obj
del addfunc3.obj
del test_cpuwall.o
del test_cpuwall.exe
nasm -fwin64 addfunc.asm
nasm -fwin64 addfunc2.asm
nasm -fwin64 addfunc3.asm
nasm -fwin64 addfunc4.asm
g++ -O3 -o test_cpuwall.exe test_cpuwall.cpp addfunc.obj addfunc2.obj addfunc3.obj addfunc4.obj -I"C:\Tools\boost_1_74_0\boost_1_74_0" -L"C:\Tools\boost_1_74_0\boost_1_74_0\stage\lib" -lboost_timer-mgw9-mt-x64-1_74 -lboost_chrono-mgw9-mt-x64-1_74

