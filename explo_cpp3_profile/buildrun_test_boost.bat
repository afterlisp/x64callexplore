echo build and launch test_boost.exe
nasm -fwin64 -g addfunc.asm
g++ -o test_boost.exe test_boost.cpp -I"C:\Tools\boost_1_74_0\boost_1_74_0" -L"C:\Tools\boost_1_74_0\boost_1_74_0\stage\lib" -lboost_regex-mgw9-mt-d-x64-1_74
test_boost.exe
