# Notes about hello.asm
###
The code shoud respect the convention of x64 Microsoft that is cited into Visual Studio Documentation
as x64 software conventions into Configure MSVC for 64 bits,x64 targets
see 
https://msdn.microsoft.com/en-us/library/7kcdt6fy.aspx
(https://docs.microsoft.com/en-us/cpp/build/x64-software-conventions?redirectedfrom=MSDN&view=msvc-160)

In very short resume ...

The convention is  as follow when you call a function  :
The first 4 integer or address parameters (pointer) are passed into 
RCX
RDX
R8 & R9

If float use :
XMM0 to XMM3

for instance if you call puts(char *string) => string should be placed into RCX
but if you call foo(int one,int two) => one would be into RCX and two into RDX
and if you call foo2(float fone,float ftwo)=> fone would be into XMM0 and ftwo into XMM3
but if you call foo2(float *pone,float *ptwo) => pone would into RCX, ptwo would be into RDX as
they are pointers.
...

if more parameters they are pushed on the stack

The callee must preserve
RBX,
RBP
RDI
RSI
R12 to R15
Note that if you call a standard library coming from C this is automatically done by the library
so no need to manage that.

Return value is always : if integer into RAX 
                         if float into  XMM0

A shadow space should be reserved becore calling : at least 32 bytes to preserve registers (8x4 bytes)
The shadow space should be desallocated after calling.

Note that on x86 ABI  (Application Binary Interface) the convention is that stacks grows down :
On an x86, the memory "allocation" of a stack frame consists simply of subtracting
the necessary number of bytes from the stack pointer.

So you need to substract the Stack Pointer (the register RSP) from the size you need to preserve.
To get back the memory space reserved simply add the same displacement...

Note that each time you have to do a call you need to preserve the return address too, so you need
add a 8 bytes adress also into the stack frame.

This is mention into stack usage section of x64 software conventions doc : The parameter area is always at the bottom of the stack (even if alloca is used),
 so that it will always be adjacent to the return address during any function call. It contains at
 least four entries, but always enough space to hold all the parameters needed by any function 
 that may be called. Note that space is always allocated for the register parameters, even 
 if the parameters themselves are never homed to the stack; a callee is guaranteed that space 
 has been allocated for all its parameters
See  also x64 prolog and epilog into the previous microsoft x64 convention doc...they save more than us as
illustration.

In our code we will implement :
save the stack of 4 registers + 1 address = 40 bytes
call
restore the stack
return 

The return "exposes" the RAX to the caller (see other subprojects) ...




