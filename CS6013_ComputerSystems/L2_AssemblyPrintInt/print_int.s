{\rtf1\ansi\ansicpg1252\cocoartf2709
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fnil\fcharset0 Menlo-Regular;}
{\colortbl;\red255\green255\blue255;\red0\green0\blue0;}
{\*\expandedcolortbl;;\cssrgb\c0\c0\c0;}
\margl1440\margr1440\vieww14220\viewh12980\viewkind0
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0

\f0\fs22 \cf2 \CocoaLigature0 ;;Author: Corinne Jones\
;;Date: 1/29/23\
\
section .rodata ;; read only data section\
\
section .text ;; this says that we\'92re about to write code (as opposed to data)\
\
        global print_int\
        global main\
\
        ; void print_int( long x );\
        ;\
        ; 1234 => \'911\'92 \'912\'92 \'913\'92 \'914\'92\
        ;\
        ; rax\
        ; 1234 / 10 => 123 remainder 4\
        ; 123 / 10 => 12 remainder 3\
        ; 12 / 10 => 1 remainder 2\
        ; 1 / 10 => 0 remainder 1\
        ; 0 <- stop\
\
print_int:\
        ;; Prologue\
        push rbp                        ; Save the previous (in our case \'91main\'92) bp so we can go back to it\
        mov rbp, rsp                    ; Update the bp to your location (in case we call someone)\
\
        sub rsp, 20                     ; save 20 bytes (characters) on stack for each digit\
        mov rax, rdi                    ; copy the passed in (1st) param (x) into AX reg to do work there\
        mov rbx, 20                     ; remainder index, BX is the \'91iTh\'92 location in digit array (the index)\
\
        mov byte [rsp+rbx], 10			; put carriage return (cr) (ascii 10) into the 1st location of my digits array\
\
dividing:\
        cmp rax, 0\
        je print                        ; if the above thing is equal then we jump to print\
\
        mov rdx, 0                      ; when dividing have to initialize rdx to 0 \'91just how it is\'92\
        mov rcx, 10\
        div rcx                         ; rax <= rax / rcx  (integer math) \'85remainder is put into dx\
\
        add rdx, 48\
        dec rbx                         ; subtract rbx, 1 - - increment our counter/index into our array\
        mov [rsp+rbx], dl               ; access the array at rsp + rbx, move remainder in\
\
        jmp dividing\
\
print:\
        mov rax, 1              ; 1 tells the OS to use the WRITE function\
\pard\tx560\tx1120\tx1680\tx2240\tx2800\tx3360\tx3920\tx4480\tx5040\tx5600\tx6160\tx6720\pardirnatural\partightenfactor0
\cf2         mov rdi, 1              ; 1st param: which is file descriptor (1==std out)\
\
        mov rcx, rsp\
        add rcx, rbx			  ; the array is at rsp + 1\
\
        mov rsi, rcx            ; 2nd param: address of the 1st character in our array of characters\
        mov rdx, rbx            ; 3rd param: rbx holds the # of digits - put in rdx so WRITE will see it\
\
        syscall\
\
        ;; done...\
        ;; epilogue\
\
        mov rsp, rbp\
        pop rbp\
        ret\
\
main:\
        mov rdi, 1234           ; the parameters to print\
        call print_int          ; call the function\
\
        mov rax, 60             ; sys_exit system call\
        mov rdi, 0              ; 1st param: error code\
        syscall                 ;}