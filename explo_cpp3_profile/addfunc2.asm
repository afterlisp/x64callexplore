
; ----------------------------------------------------------------------------------------
; This is a simple asm function called "adder" that adds 100 and add two parameters
; as function should return an byte  we use al
; -----------------------------------------------------------------------------------------  
section .text
global adder2
adder2:
	lea     eax, [ecx+100+edx]
	ret

