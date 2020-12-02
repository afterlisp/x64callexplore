# explo_cpp1_simplefunc
 Simple function call from g++
 
 
 ###
 main.cpp is a small nasm program that contains a function called myfunc()
 
 '''cpp
 
#include <iostream>                   // loads the standard library for io
extern "C" int myfunc(void);             // declare our asm function as an external one returning an int and with no arguments

int main() {
	std::cout<<"Foo returns "<<myfunc()<<"\n";        // call of our function ...
	return 0; // return 0 to say all is ok
}
'''

Note that this function is not underscored as you could find some using old compiler models

What is inside ou asm program (subroutine) myfunc.asm :
```assembly
section .text
global myfunc
myfunc:
	mov eax,66
	ret
'''
	
section text
============
When you use C or C++ we do not use such memory map directive. Synthetically (and presumbaly not real true) a program as a processus in
memory has the following structure :

'''
zone .text
(program code)
zone .data
(static data)
zone heap
(dynamic data as with malloc())
zone stack
(execution context)
'''

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

'''cpp
#include <cstdint>
#include <iostream>

// the asm function is called myfunc()
// we use the fixed length types to be sure to have 32 bits !
extern "C" std::int32_t myfunc(void);

int main() {
	std::cout<<"myfunc returns "<<myfunc()<<"\n";
	return 0;
}
'''

extern now indicates an explicit length of 32 bytes for the return of function.

g++ myfunc.obj main2.cpp -o main2.exe

Will give the same result as previous

What append if we modify the return with a big 64 bits values into asm ?
Let's check the effect with 3771334343960484476  it is 3456789ABCFEDA7Ch
Update and save myfunc2.asm :
```assembly
; ----------------------------------------------------------------------------------------
; This is a simple asm function called "myfunc" that returns a constant integer 66
; as function should return into RAX or otheinclueded , we use EAX as value is a simple integer
; -----------------------------------------------------------------------------------------  
section .text
global myfunc
myfunc:
	mov rax,3456789ABCFEDA7Ch
	ret
'''

#
nasm -fwin64 myfunc2.asm
g++ -Wall -no-pie myfunc2.obj main2.cpp -o main3.exe
main3.exe
myfunc returns -1124148612

Why ?  because the function is an int with 32 bits, only 32 bits are copied from rax, in big endian (ABCD) the full number  in hexa and decimal is as 

|   |   hex       |     dec     |
|---|-------------|-------------|
| 0	| 34 56 78 9A |   878082202 |
| 4	| BC FE DA 7C |	-1124148612 |

So the return will contain the low part of RAX that contains 34 56 78 9A BC FE DA 7C in hexa, so it returns (4)  BC FE DA 7C, 
FE DA BC starts with a bit with 1 (B=1011 in BC),
it is  as the negative number -1124148612...
Conclusion : works good as only a 32 bits value is returned ! 
Solution : to have the rigth result use a 64 bits value as in main3.cpp
'''cpp
#include <cstdint>
#include <iostream>

// the asm function is called myfunc()
// we use the fixed length types to be sure to have 32 bits !
extern "C" std::int64_t myfunc(void);

int main() {
	std::cout<<"myfunc returns "<<myfunc()<<"\n";
	return 0;
}
'''

if we compile as a main4.exe we will have :
main4.exe
myfunc returns 3771334343960484476


Retrieve the ASM from .EXE
==========================
Use the tool utility objdump that is given with mingw 
objdump -S --disassemble main2.exe > main2.dump
See the first line :
smain2.exe:     file format pei-x86-64 

Format PEI ?  Portable Executable Image (format) - this term does not exist into Microsoft Doc., only PE...
PE is normally the format defined by Microoft to build an executable image. as,they say :
" structure of executable (image) files and object files under the Windows family of operating systems"
For the original definition see Microsoft Portable Executable
   and Common Object File Format Specification 4.1
the best description could be found into https://docs.microsoft.com/en-us/windows/win32/debug/pe-format

   
PEI is just a PE format where a DOS part has been introduced see for more details :
https://opensource.apple.com/source/gdb/gdb-1515/src/bfd/libpei.h

This is to prevent people when they try to execute 64 bits on less platforms (like DOS) and prints a message when you launch
the exe as 'This app must be run under Windows' (message vendor could change from who generates the PE image)

In te previous file, search "myfunc", you will find a block like :
```assembly
0000000000401550 <myfunc>:
  401550:	b8 42 00 00 00       	mov    $0x42,%eax
  401555:	c3                   	retq   
  401556:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  40155d:	00 00 00 
'''
  
  This is the desassembly of the content of myfunc. Note that this is not NASM syntax as the C follows the "“AT&T syntax” for x86-64 assembly. 
  For the “Intel syntax, used by NASM there is no standard mingw utility... bad 
  We identify AT&T syntax by the % in front of registers and the
  invert of operands : in Intel syntax you put destination (registers) before source (registers).
  mov eax,42h  => mov $0x42,%eax...
  
  The AT&T syntax is the default for inlining assembly with GCC (and some other compilers) as this is the historical way to inject
  assembly in C...So if you want to add inline asm into you C/C++ you will need to learn this syntax.
  In our case we do not whant to inline, so we prefer the Intel Syntax as it is more like using affectation in programming languages
  As an illustraiton mov eax,42h is more like  eax=42h ..
  I would not enter in the debate.
  
  Let's look also in this file to our main function :
  ```assembly
  0000000000401560 <main>:
  401560:	55                   	push   %rbp
  401561:	53                   	push   %rbx
  401562:	48 83 ec 28          	sub    $0x28,%rsp
  401566:	48 8d ac 24 80 00 00 	lea    0x80(%rsp),%rbp
  40156d:	00 
  40156e:	e8 8d 01 00 00       	callq  401700 <__main>
  401573:	48 8d 15 87 2a 00 00 	lea    0x2a87(%rip),%rdx        # 404001 <_ZStL19piecewise_construct+0x1>
  40157a:	48 8b 0d af 2d 00 00 	mov    0x2daf(%rip),%rcx        # 404330 <__fu0__ZSt4cout>
  401581:	e8 aa 00 00 00       	callq  401630 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc>
  401586:	48 89 c3             	mov    %rax,%rbx
  401589:	e8 c2 ff ff ff       	callq  401550 <myfunc>
  40158e:	89 c2                	mov    %eax,%edx
  401590:	48 89 d9             	mov    %rbx,%rcx
  401593:	e8 b0 00 00 00       	callq  401648 <_ZNSolsEi>
  401598:	48 8d 15 72 2a 00 00 	lea    0x2a72(%rip),%rdx        # 404011 <_ZStL19piecewise_construct+0x11>
  40159f:	48 89 c1             	mov    %rax,%rcx
  4015a2:	e8 89 00 00 00       	callq  401630 <_ZStlsISt11char_traitsIcEERSt13basic_ostreamIcT_ES5_PKc>
  4015a7:	b8 00 00 00 00       	mov    $0x0,%eax
  4015ac:	48 83 c4 28          	add    $0x28,%rsp
  4015b0:	5b                   	pop    %rbx
  4015b1:	5d                   	pop    %rbp
  4015b2:	c3                   	retq   
  '''
  
  
  

