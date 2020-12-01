; ----------------------------------------------------------------------------------------
; This is a simple asm function called "myfunc" that returns a constant integer 66
; as function should return into RAX or otheinclueded , we use EAX as value is a simple integer
; -----------------------------------------------------------------------------------------  
section .text
global myfunc
myfunc:
	mov eax,66
	ret
