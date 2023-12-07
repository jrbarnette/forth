: print-reg:
    >in @ parse-name dup >r type >in !
    ." : " 7 r> - spaces
    [ only forth ]
    ' execute 0 <# # # # # # #> type cr
    [ also assembler also asm-internal ] ;
hex

print-reg: %al
print-reg: %cl
print-reg: %dl
print-reg: %bl
print-reg: %ah
print-reg: %ch
print-reg: %dh
print-reg: %bh


print-reg: %r8b
print-reg: %r9b
print-reg: %r10b
print-reg: %r11b
print-reg: %r12b
print-reg: %r13b
print-reg: %r14b
print-reg: %r15b

print-reg: %ax
print-reg: %cx
print-reg: %dx
print-reg: %bx
print-reg: %sp
print-reg: %bp
print-reg: %si
print-reg: %di

print-reg: %r8w
print-reg: %r9w
print-reg: %r10w
print-reg: %r11w
print-reg: %r12w
print-reg: %r13w
print-reg: %r14w
print-reg: %r15w

print-reg: %eax
print-reg: %ecx
print-reg: %edx
print-reg: %ebx
print-reg: %esp
print-reg: %ebp
print-reg: %esi
print-reg: %edi

print-reg: %r8d
print-reg: %r9d
print-reg: %r10d
print-reg: %r11d
print-reg: %r12d
print-reg: %r13d
print-reg: %r14d
print-reg: %r15d

print-reg: %rax
print-reg: %rcx
print-reg: %rdx
print-reg: %rbx
print-reg: %rsp
print-reg: %rbp
print-reg: %rsi
print-reg: %rdi

print-reg: %r8
print-reg: %r9
print-reg: %r10
print-reg: %r11
print-reg: %r12
print-reg: %r13
print-reg: %r14
print-reg: %r15
