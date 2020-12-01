# explo_cpp1_simplefunc
 Simple function call from g++
 
 
 ###
 myfunc.cpp is a small nasm program that contains a function called myfunc()
 
#include <iostream>                   // loads the standard library for io
extern "C" int myfunc(void);             // declare our asm function as an external one returning an int and with no arguments

int main() {
	std::cout<<"Foo returns "<<myfunc()<<"\n";        // call of our function ...
	return 0; // return 0 to say all is ok
}
Note that this function is not underscored as you could find some using old compiler models

What is inside ou asm program (subroutine) myfunc.asm :

section .text
global myfunc
myfunc:
	mov eax,66
	ret
	
section text
============
When you use C or C++ we do not use such memory map directive. Synthetically (and presumbaly not real true) a program as a processus in
memory has the following structure :

zone .text
(program code)
zone .data
(static data)
zone heap
(dynamic data as with malloc())
zone stack
(execution context)

So in .text zone we will put some code. Note that is a good practice to put "section text" even if it is in some assembly optional...


global myfunc
=============
By default all label like "myfunc :" that is a label declaration are hidden, visible only into the file. As we need to call myfunc by 
a C/C++ program that is external we need to say that this label "myfunc" is not hidden, this is the role of the global directive.

mov eax,66
==========
mov is an instruction that get a value and put in here in a register. It's a copy function betwween generally registers of the 
processor and memory.
Here we use EAX, this is a general register that contains 32 bits value so an int, this is the low value of RAX that is a 64 bits 
general register.
Note that when you put a two byte value into EAX, the update affects subregisters called respectively AX, AH, AL. 
Note that AL is the low 8 bits value of AX.
AX is the low 16 bits value of EAX. 
And EAX is the low  32 bits value of RAX.

|12345678|12345678|12345678|12345678|12345678|12345678|12345678|12345678|
|--------|--------|--------|--------|--------|--------|--------|--------|
|        |        |        |        |        |        |    AH  |    AL  |
|        |        |        |        |        |        |    AX  |    AX  |
|        |        |        |        |   EAX  |   EAX  |   EAX  |   EAX  |
|   RAX  |   RAX  |   RAX  |   RAX  |   RAX  |   RAX  |   RAX  |   RAX  |                                                


Note that some others  general registers that have such sub registers are RBX, RCX and RDX. Note that other registers have same subdivions
but not whith the same convention H,L,Ex,Rx...(for instance

When you return a value, the calee needs to use RAX or one of its subdivision.
In our case, we want to use a standard int in C that is a 32 bits value, so we select the EAX register.
When we write mov EAX,66 we are saying put the value in decimal 66 into the register EAX

ret
===
When the C/C++ program has called our subroutine or function myfunc(), it has placed the return address on top of the parameters
on the stack and has branched to our asm subroutine. When the subroutine is achieved we needs to use a return to the caller. This is
the role of ret to take the return address stored into the stack and jump to the caller.

FIXED LENGTH TYPES
==================
Note : to avoid issues with the C++ code with length of types, we need to use some fixed width interger types
The name of the type is different from int, uint,... but you are sure that the length is what you want.
This is defined into  <cstdint>
For instance main2.cpp contains such usage

#include <cstdint>
#include <iostream>

// the asm function is called myfunc()
// we use the fixed length types to be sure to have 32 bits !
extern "C" std::int32_t myfunc(void);

int main() {
	std::cout<<"myfunc returns "<<myfunc()<<"\n";
	return 0;
}

extern now indicates an explicit length of 32 bytes for the return of function.

g++ myfunc.obj main2.cpp -o main2.exe

Will give the same result as previous

What append if we modify the return with a big 64 bits values into asm ?
Let's check the effect
Update and save myfunc2.asm
; ----------------------------------------------------------------------------------------
; This is a simple asm function called "myfunc" that returns a constant integer 66
; as function should return into RAX or otheinclueded , we use EAX as value is a simple integer
; -----------------------------------------------------------------------------------------  
section .text
global myfunc
myfunc:
	mov rax,3456789ABCFEDA7Ch
	ret
#
nasm -fwin64 myfunc2.asm
g++ -Wall -no-pie myfunc2.obj main2.cpp -o main3.exe
main3.exe
myfunc returns -1124148612

Why, because the function is an int with 32 bits, only 32 bits are copied from rax, in big endian (ABCD) the full number  in hexa and decimal is as 
0	34 56 78 9A	878082202
4	BC FE DA 7C	-1124148612
So the return will contain the low part of RAX that contains 56 78 9A FE DA 7C in hexa, so it returns FE DA 7C, FE DA 7C starts with a bit with 1 (F=1111 in FE),
it is  as the negative number -1124148612...
Conclusion : works good as only a 32 bits value is returned ! 




Retrieve the ASM from .EXE
==========================
Use the tool utility objdump that is given with mingw 
objdump -S --disassemble main2.exe > main2.dump
See the first line :
smain2.exe:     file format pei-x86-64 



