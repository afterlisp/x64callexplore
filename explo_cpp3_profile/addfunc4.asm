; ----------------------------------------------------------------------------------------
; This is a simple asm function called "adder" that adds 100 and add two parameters
; as function should return an byte  we use rax.
; -----------------------------------------------------------------------------------------  
section .text
global adder4
adder4:
	add rcx,64h   ; add 100 to the first parameter
	add rcx,rdx   ; add the second parameter to the first parameter + 100
	mov rax,rcx  ; returns the result with rax
	ret
