; ----------------------------------------------------------------------------------------
; This is a Win64 console program that prints "Hello" on one line and then exits.  
; we simulate a main C function (named main of course)
; we need to reserve some shadow space (see our notes) as mentionned by Microsoft doc...
; as we use gcc this is linked to standard library that contains the puts() function this
; is why it is marked as extern(al).
; assemble, compile/link and run :
; nasm -fwin64 hello.asm && gcc hello.obj && a
; ----------------------------------------------------------------------------------------

        global  main
        extern  puts
        section .text
main:
        sub     rsp, 28h                        ; Move stack pointer to reserve the shadow space (4 registers x 8 bytes + 1 call x 8 bytes=40 bytes=28h)
        mov     rcx, message                    ; First argument is pointer (address) of message
        call    puts                            ; the call to puts(message)
        add     rsp, 28h                        ; Remove shadow space
        ret
message:
        db      'Hello', 0                      ; C strings are null terminate (0 byte at end)