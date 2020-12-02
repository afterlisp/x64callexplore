# explo_cpp2_simplearith
 Simple function call from g++
 
### we want now write a program that calls an asm subroutine that does some add
Getting two values, we want to return theses values + 10.
all values are int (8 bytes)
```
ret= a + b + 100
```
In C++ we will have the following code - see add.cpp with pseudo constants
```
#include <cstdint>
#include <iostream>

// the asm function is called adder()
// we use the fixed length types to be sure to have byte 8 bits !
extern "C" std::uint8_t adder(uint8_t,uint8_t);

int main() {
	uint8_t a=3; // this is unsigned char from C++POV
	uint8_t b=5;
	// as uint8_t is just a typedef the compiler will show 'j' (unsigned char) and not an int
	// so cast the return to int...
	std::cout<<"adder returns "<<(int)adder(a,b)<<"\n";
	return 0;
}
```

Remember that parameters are passed into RCX,RDX,R8,R9 for the first 4 parameters. In our case 2 parameters
so we will receive a and b respectively into RCX et RDX
We will use a new instruction in asm call ADD :
ADD registerdest,registersrc or value
(at this step)
So writing the asm code is quite simple. Do not forget to put the result into RAX with a MOV...

We use bytes so we will use the small registers CL, DL, AL as they are right sized to 8 bits...
the function is name adder()

Let's look to asm  into addfunc.asm
```assembly
section .text
global adder
adder:
	add cl,64h   ; add 100 to the first parameter
	add cl,dl    ; add the second parameter to the first parameter + 100
	mov al,cl   ; returns the result with ax
	ret
```
we have search if a add instruction could exist with the register RCX (its subset register CL here), yes so we use it (see Intel doc)
so the first line is lique   CL=CL+64h
the second line helps use to add two registers together , so this lique CL=CL+DL
the third line is to copy our result that is in CL to the caller C++ , so AL=CL
That's all.

If you build the code as usual and run you will obtain 108. Fine!


Is it better to use al or eax or rax, if you see the work on https://www.agner.org/optimize/ you will see that efficiency is not proved
It would be better to use EAX or RAX in numerous cases...
Generally it seems to adress byte it would be better to use EAX instead using "old" style register...
As this step we do not know how to profile performance, so wait a little... But using ah inform a human that we are using a byte...so good

But this code is not very efficient in terms of size , there are very high level instructions in 80x86 assembleur and one is potentially
interesting as it offers to us to load EAX directly and do this stuff
Let's look to addunc2.asm.
This is a very compact code 
```assembly
section .text
global adder
adder:
	lea     eax, [ecx+100+edx]
	ret
```
The purpose of LEA or load effective address instruction is not to do add historically. It appears with 8086 to helps some manipulations in C.
LEA EAX,[address]
In short, LEA performs the calculation of the address inside the brackets and place that calculated address into the destination operand
What is this strange syntax. The bracket [] indicates an adress mode. In our case it says take the content of, this is like *() in C if 
registers could be assumed as address... Be careful we will see brackets have different meanings, this is specific to LEA.

As the purpose is to calculate an adress by adding some registers and values , it appears that using it do some add (or sub or multiply) operations
was quite good, so when you read
```
lea eax,[ecx+100+edx] we are saying
EAX=ECX+100+EDX
```

And the trick is done (see also http://www.jagregory.com/abrash-zen-of-asm/, this is vintage - 1989 -  but still efficient)
If you compile and execute, you will find the same value (that from a point a view of the processor is an address as you used LEA...)
```
nasm -fwin64 addfunc2.asm
g++ -Wall -no-pie addfunc2.obj add.cpp -o add2.exe
add2.exe

adder returns 108
```



Be careful, do not consider that LEA is faster than ADD this is just a trick to write quickly a three values addition...You will encounter
such generations of LEA when you use the optimisation flag of gcc/g++ -Os (s pour size or space ) that generate code optimized for space usage (and medium speed).
Optimisations are passed as options like we use -Wall, so using The -O or -Ox (where x is a number like 1, 2, or 3) is an optimization level. 
The absence of -O or -Ox means no optimizations. -O is equivalent to -O1, 
and -Os , oenables all -O2 optimizations which do not increase the size of the executable, 
and then it also toggles some optimization flags to further reduce executable size.
 (see http://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html)
 
To see faster what is generated you could use the great site compile explorer : https://godbolt.org/
 
