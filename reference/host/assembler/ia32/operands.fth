\ Copyright 2023, by J. Richard Barnette
\
\ memory operand syntax
\ Intel		AT&T		Forth
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
: [*s] ( reg scale -- memopnd )
    or dup 0700 and >r 1f8ff and 3 lshift r> or [] ;
: [*1] ( reg -- memopnd ) 00 [*s] ;
: have-base? ( opnd -- flag ) 1000 and 1000 = ;
: have-disp? ( opnd -- flag ) 6000 and 6000 = ;
: have-index? ( opnd -- flag ) 8000 and 8000 = ;

: is-mem? ( opnd -- flag ) 4000 and 4000 = ;
: is-reg? ( opnd -- flag ) d000 and 1000 = ;
: is-immed? ( opnd -- flag ) # = ;
: is-disp32? ( opnd -- flag ) f000 and 6000 = ;

: is-[r+r*s]?  ( memopnd -- flag )  9000 and 9000 = ;
: is-[r+r*1]?  ( memopnd -- flag )  90c0 and 9000 = ;
: is-sp-index? ( memopnd -- flag ) 88038 and 8020 = ;
: is-bp-base?  ( memopnd -- flag ) 01007 and 1005 = ;
: need-disp?   ( memopnd -- flag )  5007 and 1005 = ;
: base<->index ( memopnd -- memopnd )
    dup dup 3 rshift xor dup 3 lshift or 9003f and xor ;

: start-regs 1100 ;
: def-regs: ( size "names*8" -- size' )
    8 0 do dup i or constant loop
    dup 10000 and 8 rshift + 10000 xor
;
: end-regs drop ;

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
asm-defs


: [*2] ( reg -- memopnd ) 08 [*s] ;			\ [reg*s]
: [*4] ( reg -- memopnd ) 10 [*s] ;			\ [reg*s]
: [*8] ( reg -- memopnd ) 18 [*s] ;			\ [reg*s]
: +[] ( memopnd reg -- memopnd ) over have-base? if [*1] then join-index ;
: +[*2] ( memopnd reg -- memopnd ) [*2] join-index ;
: +[*4] ( memopnd reg -- memopnd ) [*4] join-index ;
: +[*8] ( memopnd reg -- memopnd ) [*8] join-index ;

: #[] ( x -- memopnd ) # [] ;				\ addr
: #[.] ( x reg -- memopnd ) #[] or ;			\ d[reg]
: #[*2] ( x reg -- memopnd ) [*2] # or ;		\ d[reg*s]
: #[*4] ( x reg -- memopnd ) [*4] # or ;		\ d[reg*s]
: #[*8] ( x reg -- memopnd ) [*8] # or ;		\ d[reg*s]
: [+] ( reg reg -- memopnd ) [*1] join-index ;		\ [reg+reg]
: [+*2] ( reg reg -- memopnd ) +[*2] ;			\ [reg+reg*s]
: [+*4] ( reg reg -- memopnd ) +[*4] ;			\ [reg+reg*s]
: [+*8] ( reg reg -- memopnd ) +[*8] ;			\ [reg+reg*s]
: #[+] ( x reg reg -- memopnd ) [+] # or ;		\ d[reg+reg]
: #[+*2] ( x reg reg -- memopnd ) #[*2] join-index ;	\ d[reg+reg*s]
: #[+*4] ( x reg reg -- memopnd ) #[*4] join-index ;	\ d[reg+reg*s]
: #[+*8] ( x reg reg -- memopnd ) #[*8] join-index ;	\ d[reg+reg*s]


start-regs
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
: is-accum? ( opnd -- flag )
    [ # dir-bit or invert ] literal and
    case %rax of true endof %eax of true endof
	 %ax  of true endof %al  of true endof
	 false swap endcase ;
asm-defs

\ special encoded forms:
\   d?[b]		<- mod r/m (no s-i-b)
\   d32[%rip]		<- [%rbp] + d32
\   d32[%rip]		<- [%r13] + d32
\   d?[b+x*s]		<- d?[%rsp] + s-i-b
\   d?[b+x*s]		<- d?[%r12] + s-i-b
\   d?[b]		<- d?[b+%rsp*.]
\   d32[x*s]		<- [%rbp+x*s] + d32
\   d32[x*s]		<- [%r13+x*s] + d32
\   d32			<- [%rbp+%rsp*.] + d32
\   d32			<- [%r13+%rsp*.] + d32
\
\ alternatives to unavailable forms:
\   [%rbp]		-> 0[%rbp]
\   [%r13]		-> 0[%r13]
\   [%rbp+x*1]		-> [x+%rbp*1]
\   [%r13+x*1]		-> [x+%r13*1]
\   [%rbp+x*s]		-> 0[%rbp+x*s]
\   [%r13+x*s]		-> 0[%r13+x*s]
\   d?[%rsp]		-> d?[%rsp+.*.]
\   d?[%r12]		-> d?[%r12+.*.]
\   d?[b+%rsp*1]	-> d?[%rsp+b*1]
\   d?[b+%rsp*.]	-> error

\ canonical forms on stack for two operands
\   ( imm reg )
\   ( imm mem )
\   ( mem reg )
\   ( reg reg )
\
\   * When TOS is a reg and the reg is destination, D bit is set
\   * When TOS is a reg other than the accumulator, second operand is also
\     not the accumulator
\   * When the second operand is immediate, omit the # indicator cell.
\     If TOS is a reg, also set the I bit.
\
\ In ( reg reg ), if either reg is accumulator, it should be TOS


internal-defs
: check-memopnd ( memopnd -- memopnd )
    dup is-[r+r*1]? if
	\ if index = %rsp or base = %rbp or base = %r13
	dup is-sp-index? over is-bp-base? or if base<->index then
    then
    \ if SP is still index reg, error
    dup is-sp-index? abort" cannot use %rsp as index register"

    dup need-disp? if 0 swap # or then
;

: 2operand ( opnd opnd -- imm reg | imm mem | mem reg | reg reg )
    dup is-reg? if
	over is-accum? if dup is-accum? 0= if swap exit then then
	over is-immed? 0= if dir-bit then or
    else
	have-disp? if rot else swap then
	dup is-immed? if drop then
    then
;

: s-i-b ( opnd mod-r/m -- ) 04 or db, ff and db, ;

: mod-r/m ( opnd /r -- )
    over is-reg? if 00c0 or or 00ff and db, exit then
    over is-disp32? if s-i-b drop dd, exit then
    over have-disp? if over -80 80 within if 40 else 80 then else 00 then
    dup >r or
    over have-index? if s-i-b else swap 07 and or db, then
    r> case 80 of dd, endof 40 of db, endof endcase
;
asm-defs

base !
