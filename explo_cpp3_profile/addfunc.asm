; ----------------------------------------------------------------------------------------
; This is a simple asm function called "adder" that adds 100 and add two parameters
; as function should return an byte  we use al
; -----------------------------------------------------------------------------------------  
section .text
global adder
adder:
	add cl,64h   ; add 100 to the first parameter
	add cl,dl    ; add the second parameter to the first parameter + 100
	mov al,cl   ; returns the result with ax
	ret
