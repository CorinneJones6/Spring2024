extern puts

section .rodata ;this is the read only data(hello is a constant)
helloString: db "hello",0 ;hellostring is the name of our symbol
; db is the directive to put data in our object file
; the 0 is the null terminator that puts is expecting
; nasm does NOT null terminate our string automatically 

section .text ; this says that we're about to write code (as opposed to data)

global sayHello  
global myPuts 
global myGTOD 

sayHello: ; and here we go...
mov rdi, helloString ; address of helloString into rdi
call puts
ret ; just return (for now)

myPuts: 
mov rax, 1 ;move 1 to rax to indicate this is a write
mov rdx, rsi ;move the string to the rsi
mov rsi, rdi ;move the integer to the rd
mov rdi, 1 ;"just how it is" - gloria 
syscall
ret

myGTOD: 
sub rsp, 16 ; subtract space on the stack
mov rdi, rsp ; pointer to timeval struct to rdi
mov rsi, 0 ; pass through a null pointer
mov rax, 96 ; according to the ABI for gettimeofday
syscall
;Mov values from time val
mov rax, [rsp] ; load secs into rax
mov rdx, [rsp +8] ; load microsecs into rds
add rsp, 16 ;restore the stack
ret
