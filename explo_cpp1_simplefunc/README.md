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



mov eax,66
==========
mov is an instruction that get a value and put in here in a register.
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


