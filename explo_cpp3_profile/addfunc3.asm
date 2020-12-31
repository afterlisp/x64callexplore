; ----------------------------------------------------------------------------------------
; This is a simple asm function called "adder" that adds 100 and add two parameters
; as function should return an byte  we use eax.
; -----------------------------------------------------------------------------------------  
section .text
global adder3
adder3:
	add ecx,64h   ; add 100 to the first parameter
	add ecx,edx   ; add the second parameter to the first parameter + 100
	mov eax,ecx  ; returns the result with ax
	ret
