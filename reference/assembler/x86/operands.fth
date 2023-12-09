\ Copyright 2023, by J. Richard Barnette
\
\ memory operand syntax
\ Intel		Unix		Forth
\ addr		addr		addr #[]		all modes
\ [r1]		(r1)		r1 []			all modes
\ d[r1]		d(r1)		d #[] r1 +[]		all modes
\ [r1+r2]	(r1,r2)		r1 [] r2 +[]		all modes
\ d[r1+r2]	d(r1,r2)	d #[] r1 +[] r2 +[]	all modes
\ [r1*s]	(,r1,s)		r1 [*s]			32- and 64-bit only
\ d[r1*s]	d(,r1,s)	d #[] r1 +[*s]		32- and 64-bit only
\ [r1+r2*s]	(r1,r2,s)	r1 [] r2 +[*s]		32- and 64-bit only
\ d[r1+r2*s]	d(r1,r2,s)	d #[] r1 +[] r2 +[*s]	32- and 64-bit only
\
\ [*s] is one of [*2], [*4] or [*8]

base @ hex asm-defs

\ operand cell layout
\   ---Dx--r XMIBtttt ssxxxrrr
\       D = destination?
\       X = have index?
\       M = memory?
\       I = have immediate?
\       B = have base?
\    tttt = reg-type ( none, 8, 16, 32, 64, reserved )
\      ss = scale
\    xxxx = index register
\    rrrr = base register
\
\  cell  XMIB  interpretation
\  1000  0001  reg
\  2000  0010  imm
\  5000  0101  [reg]
\  6000  0110  address
\  7000  0111  disp[reg]
\  c000  1100  [reg*scale]
\  d000  1101  [reg+reg*scale]
\  e000  1110  disp[reg*scale]
\  f000  1111  disp[reg+reg*scale]

2000 constant #
: [] ( immed|reg -- memopd ) 4000 or ;		\ addr or [reg]

internal-defs
100000 constant dir-bit

\ These are simple, general purpose queries that could conceivably
\ be exported as part of the assembler syntax for macros.
: is-mem?         ( opnd -- flag )  4000 and  4000 = ;
: is-reg?         ( opnd -- flag )  5000 and  1000 = ;
: is-immed?       ( opnd -- flag )  # = ;
: have-base?      ( opnd -- flag )  1000 and  1000 = ;
: have-index?     ( opnd -- flag )  8000 and  8000 = ;
: have-immed?     ( opnd -- flag )  # and is-immed? ;
: have-disp?      ( opnd -- flag )  6000 and  6000 = ;

\ The following queries correspond to various special encodings
\ that must be tested for while assembling operands.
: is-[r+r*s]?     ( opnd -- flag )  9000 and  9000 = ;
: is-[bp+r*1]?    ( opnd -- flag )  d0c7 and  d005 = ;
: is-[sp*1]?      ( opnd -- flag ) 8c0f8 and  c020 = ;
: is-bp-base?     ( opnd -- flag )  7007 and  5005 = ;
: is-[x*s]?       ( opnd -- flag )  f000 and  c004 = ;
: is-[sp]?        ( opnd -- flag )  d007 and  5004 = ;
: is-addr?        ( opnd -- flag )  f000 and  6000 = ;
: is-sp-index?    ( opnd -- flag ) 88038 and  8020 = ;
: is-disp32?      ( opnd -- flag )  7000 and  6000 = ;

: base<->index ( opnd -- opnd )
    dup dup 3 rshift xor dup 3 lshift or 9003f and xor ;

: is-accum-immed? ( opnd -- flag ) 17007 and  3000 = ;
: is-reg-immed?   ( opnd -- flag )  3000 and  3000 = ;
: is-mem-immed?   ( opnd -- flag )  7000 and  4000 > ;
: is-src-immed?   ( opnd -- flag )  7000 and  3000 > ;

: >REX.R ( regopnd -- REX.-R-- ) #14 rshift 4 and ;
: >REX.B ( opnd -- REX.---B )    #16 rshift 1 and ;
: >REX.XB ( memopnd -- REX.--XB ) dup >REX.B swap #18 rshift 2 and or ;

: start-regs 1100 ;
: def-regs: ( size "names*8" -- size' )
    8 0 do dup i or constant loop
    dup 10000 and 8 rshift + 10000 xor
;
: end-regs drop ;

: [*s] ( regopnd scale -- memopnd )
    3 lshift or dup 0f00 and >r 1103f and 3 lshift r> or [] ;
: [*1] ( reg -- memopnd ) 0 [*s] ;

: join-index ( opnd opnd -- opnd )
    2dup and have-base? abort" can't have two base regs"
    2dup and have-index? abort" can't have two index regs"
    2dup or is-[r+r*s]? if
	over 0f00 and over 0f00 and <>
	abort" base and index registers are of different sizes"
    then
    dup 0f00 and 0400 <> abort" only 64-bit addressing is implemented"
    or
;
: +disp ( x memopnd -- memopnd ) # or ;
asm-defs


: [*2] ( reg -- memopnd ) 1 [*s] ;			\ [reg*s]
: [*4] ( reg -- memopnd ) 2 [*s] ;			\ [reg*s]
: [*8] ( reg -- memopnd ) 3 [*s] ;			\ [reg*s]
: +[] ( memopnd reg -- memopnd ) over have-base? if [*1] then join-index ;
: +[*2] ( memopnd reg -- memopnd ) [*2] join-index ;
: +[*4] ( memopnd reg -- memopnd ) [*4] join-index ;
: +[*8] ( memopnd reg -- memopnd ) [*8] join-index ;

: #[] ( x -- memopnd ) # [] ;				\ addr
: #[.] ( x reg -- memopnd ) #[] or ;			\ d[reg]
: #[*2] ( x reg -- memopnd ) [*2] +disp ;		\ d[reg*s]
: #[*4] ( x reg -- memopnd ) [*4] +disp ;		\ d[reg*s]
: #[*8] ( x reg -- memopnd ) [*8] +disp ;		\ d[reg*s]
: [+] ( reg reg -- memopnd ) [*1] join-index ;		\ [reg+reg]
: [+*2] ( reg reg -- memopnd ) +[*2] ;			\ [reg+reg*s]
: [+*4] ( reg reg -- memopnd ) +[*4] ;			\ [reg+reg*s]
: [+*8] ( reg reg -- memopnd ) +[*8] ;			\ [reg+reg*s]
: #[+] ( x reg reg -- memopnd ) [+] +disp ;		\ d[reg+reg]
: #[+*2] ( x reg reg -- memopnd ) #[*2] join-index ;	\ d[reg+reg*s]
: #[+*4] ( x reg reg -- memopnd ) #[*4] join-index ;	\ d[reg+reg*s]
: #[+*8] ( x reg reg -- memopnd ) #[*8] join-index ;	\ d[reg+reg*s]


start-regs
        \   0      1      2      3      4      5      6      7
def-regs:   %al    %cl    %dl    %bl    %ah    %ch    %dh    %bh
def-regs:   %r8b   %r9b   %r10b  %r11b  %r12b  %r13b  %r14b  %r15b
def-regs:   %ax    %cx    %dx    %bx    %sp    %bp    %si    %di
def-regs:   %r8w   %r9w   %r10w  %r11w  %r12w  %r13w  %r14w  %r15w
def-regs:   %eax   %ecx   %edx   %ebx   %esp   %ebp   %esi   %edi
def-regs:   %r8d   %r9d   %r10d  %r11d  %r12d  %r13d  %r14d  %r15d
def-regs:   %rax   %rcx   %rdx   %rbx   %rsp   %rbp   %rsi   %rdi
def-regs:   %r8    %r9    %r10   %r11   %r12   %r13   %r14   %r15
end-regs

internal-defs
: [+0] ( memopnd -- memopnd ) [ %rsp [*1] 0f00 invert and ] literal or ;
: >reg# ( memopnd -- reg# ) 07 and ;
: >/r ( regopnd -- /r ) >reg# 3 lshift ;

: is-accum? ( opnd -- flag )
    false swap 95f07 and case
	%rax of drop true endof
	%eax of drop true endof
	%ax  of drop true endof
	%al  of drop true endof
     endcase ;
asm-defs


internal-defs
\ The following encodings are special in various ways:
\   meaning		<- encoding
\   d?[b]		<- mod-r/m w/o s-i-b
\   d32[%rip]		<- [%rbp] + d32
\   d32[%rip]		<- [%r13] + d32
\   d?[b+x*s]		<- d?[%rsp] + s-i-b
\   d?[b+x*s]		<- d?[%r12] + s-i-b
\   d?[b+0]		<- d?[b+%rsp*.]
\   d32[x*s]		<- [%rbp+x*s] + d32
\   d32[x*s]		<- [%r13+x*s] + d32
\   d32			<- [%rbp+%rsp*.] + d32
\   d32			<- [%r13+%rsp*.] + d32
\
\ If an operand can't be encoded because it conflicts with one of
\ the forms above, then transformations to equivalent forms are applied
\ as follows.
\
\ Swap base and index:
\   [%rbp+x*1]		-> [x+%rbp*1]
\   [%r13+x*1]		-> [x+%r13*1]
\   d?[b+%rsp*1]	-> d?[%rsp+b*1]
: need-swap? ( opnd -- flag ) dup is-[bp+r*1]? swap is-[sp*1]? or ;

\ Add a displacement of 0.
\   [%rbp]		-> 0[%rbp]
\   [%r13]		-> 0[%r13]
\   [%rbp+x*s]		-> 0[%rbp+x*s]
\   [%r13+x*s]		-> 0[%r13+x*s]
\   [x*s]		-> 0[x*s]
: need-disp? ( opnd -- flag ) dup is-bp-base? swap is-[x*s]? or ;

\ Use a SIB to encode a 0 index.
\   d?[%rsp]		-> d?[%rsp+0]
\   d?[%r12]		-> d?[%r12+0]
\   addr		-> addr[+0]
: need-index0? ( opnd -- flag ) dup is-[sp]? swap is-addr? or ;

\ ESP and RSP can't be used as an index register.
\   d?[b+%rsp*.]	-> error

: check-memopnd ( memopnd -- memopnd )
    dup is-reg? if exit then
    dup need-swap? if base<->index then
    dup is-sp-index? abort" cannot use %rsp as index register"
    dup need-disp? if 0 swap +disp then
    dup need-index0? if [+0] then
;

: >mod ( /r mod -- mod-r/m ) 6 lshift or ;
: sib ( memopnd /r mod -- ) >mod [ %rsp >reg# ] literal or db, db, ;
: no-sib ( memopnd /r mod -- ) >mod swap >reg# or db, ;

: mod-r/m, ( opnd /r -- )
    >r dup is-reg? if r> %11 no-sib exit then
    dup is-disp32? if [ %rbp >reg# ] literal or r> %00 sib dd, exit then
    dup have-disp?
	if over -80 80 within if %01 else %10 then
	else %00
	then r> swap >r
    over have-index? if r@ sib else r@ no-sib then
    r> case %01 of db, endof %10 of dd, endof endcase
;

asm-defs

base !
