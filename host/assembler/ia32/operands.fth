\ Copyright 2011, by J. Richard Barnette
\
\ 32-bit memory operands
\ Intel		AT&T		Forth
\ addr		addr		addr #[]
\ [r1]		(r1)		r1 []
\ d[r1]		d(r1)		d #[] r1 +[]
\ [r1+r2]	(r1,r2)		r1 [] r2 +[]
\ d[r1+r2]	d(r1,r2)	d #[] r1 +[] r2 +[]
\ [r1*s]	(r1,r2,s)	r1 [*s]
\ d[r1*s]	d(r1,r2,s)	r1 [] r2 +[*s]
\ [r1+r2*s]	(r1,r2,s)	r1 [] r2 +[*s]
\ d[r1+r2*s]	d(r1,r2,s)	d #[] r1 +[] r2 +[*s]

base @ hex

: ." postpone s" postpone type ; immediate
: abort" postpone if postpone ." postpone abort postpone then ; immediate
20 constant bl
: within ( test low high -- flag) over - >r - r> u< ;

create hex-digits
  char 0 c, char 1 c, char 2 c, char 3 c,
  char 4 c, char 5 c, char 6 c, char 7 c,
  char 8 c, char 9 c, char a c, char b c,
  char c c, char d c, char e c, char f c,

: .x 0f and hex-digits + c@ emit ;
variable db-col 0 db-col !

: db, db-col @ if bl emit then
      dup 4 rshift .x .x
      db-col @ 1 + dup 10 = if cr drop 0 then db-col !
;
: dw, dup db, 8 rshift db, ;
: dd, dup dw, 10 rshift dw, ;
: newline cr 0 db-col ! ;

\ operand layout
\   XMIB-zzz ssxxxrrr
\       X = have index?
\       M = memory?
\       I = have immediate?
\       B = have base?
\     zzz = size ( none, 8, 16, 32, reserved )
\      ss = scale
\     xxx = index register
\     rrr = base register
\
\  mask  XMIB  interpretation
\  1000  0001  reg
\  2000  0010  imm
\  5000  0101  [reg]
\  6000  0110  address
\  7000  0111  disp[reg]
\  c000  1100  [reg*scale]
\  d000  1101  [reg+reg*scale]
\  e000  1110  disp[reg*scale]
\  f000  1111  disp[reg+reg*scale]

1007 constant [b]-mask
3007 constant .[b]-mask
80c0 constant [.*s]-mask
80f8 constant [x*s]-mask
9007 constant [b+.*.]-mask
9038 constant [.+x*.]-mask
b0c7 constant .[b+.*s]-mask

1004 constant [esp]-opd
1005 constant [ebp]-opd
2000 constant addr-opd
3000 constant .[.]-opd
8000 constant [.*1]-opd
8020 constant [esp*1]-opd
8040 constant [.*2]-opd
8080 constant [.*4]-opd
80c0 constant [.*8]-opd
9005 constant [ebp+.*1]-opd
9020 constant [.+esp*.]-opd

1000 constant reg-opd
2000 constant imm-opd
4000 constant mem-opd
8000 constant idx-opd

5000 constant mem-indirect
6000 constant mem-direct
a000 constant mem-indexed
a040 constant mem-indexed*2
a080 constant mem-indexed*4
a0c0 constant mem-indexed*8

f000 constant format-mask

0100 constant size-byte
0200 constant size-word
0300 constant size-dword
0f00 constant size-mask

0004 constant reg-esp
0005 constant reg-ebp
0007 constant reg-mask
00ff constant sib-mask

\ : def-regs: ( size "names*8" -- )
\     reg-opd or 8 0 do dup i or constant loop drop ;
: def-regs: ( size "names*8" -- ) reg-opd or
    0 begin ( size idx ) dup 8 < while over over or constant 1+ repeat
    drop drop
;

size-byte  def-regs: %al  %cl  %dl  %bl  %ah  %ch  %dh  %bh 
size-word  def-regs: %ax  %cx  %dx  %bx  %sp  %bp  %si  %di
size-dword def-regs: %eax %ecx %edx %ebx %esp %ebp %esi %edi

: is-imm? ( operand -- flag ) imm-opd and 0<> ;
: is-reg? ( operand -- flag ) format-mask and reg-opd = ;
: is-mem? ( operand -- flag ) mem-opd and 0<> ;
: is-byte? ( operand -- flag ) size-mask and size-byte = ;
: is-word? ( operand -- flag ) size-mask and size-word = ;
: is-dword? ( operand -- flag ) size-mask and size-dword = ;
: have-base? ( operand -- flag ) reg-opd and 0<> ;
: have-idx? ( operand -- flag ) idx-opd and 0<> ;
: base>idx ( operand -- operand ) 3 lshift ;
: idx>base ( operand -- operand ) 3 rshift ;
: base<->index dup dup idx>base xor [b]-mask and dup base>idx or xor ;

imm-opd constant #
mem-direct constant #[]

: check-addr ( operand -- operand )
    dup is-reg? 0= abort" [] operand must be a register"
    dup is-dword? 0= abort" only 32-bit addressing is supported"
    reg-mask and
;
: >scaled ( regop ss -- memop ) swap check-addr base>idx or ;

: [] ( regop -- memop ) check-addr mem-indirect or ; 
: [*2] ( regop -- memop ) mem-indexed*2 >scaled ;
: [*4] ( regop -- memop ) mem-indexed*4 >scaled ;
: [*8] ( regop -- memop ) mem-indexed*8 >scaled ;

: check-scaled ( memop regop ss )
    >scaled over have-idx? abort" can only scale one register"
    or
;

: +[] ( memop regop -- memop )
    over have-base? if
	[.*1]-opd check-scaled
    else
	check-addr reg-opd or or
    then
;
: +#[] ( memop disp -- disp memop )
    over is-imm? abort" can't apply +#[] to immediate or address"
    swap #[] or
;
: +[*2] ( memop regop -- memop ) [.*2]-opd check-scaled ;
: +[*4] ( memop regop -- memop ) [.*4]-opd check-scaled ;
: +[*8] ( memop regop -- memop ) [.*8]-opd check-scaled ;

\ standard forms:
\   d?[b]		<- mod r/m (no s-i-b)
\   d?[b+x*s]		<- d?[%esp] + s-i-b
\   d?[b+.*.]		<- d?[b+%esp*.]
\   d32			<- [%ebp]
\   d32[x*s]		<- [%ebp+x*s]
\
\ alternatives to unavailable forms:
\   [%ebp]		-> 0[%ebp]
\   [%ebp+x*1]		-> [x+%ebp*1]
\   [%ebp+x*s]		-> 0[%ebp+x*s]
\   d?[%esp]		-> d?[%esp+-*-]
\   d?[b+%esp*1]	-> d?[%esp+b*1]
\   d?[b+%esp*.]	-> error

: check-memory-operand ( memopd -- memopd )
    \ if indexed with scale *1, we may want to swap the base
    \ and index register fields
    dup [.*s]-mask and [.*1]-opd = if
	\ if ESP*1, swap to avoid error
	dup [x*s]-mask and [esp*1]-opd = if
	    base<->index
	else
	    dup have-base? if
		\ base = EBP (no displacement) - avoid adding displacement
		dup reg-mask and reg-ebp = if base<->index then
	    else
		\ no base - avoid unnecessary sib
		base<->index
	    then
	then
    then

    \ ESP*n - ESP not allowed as index
    dup [.+x*.]-mask and [.+esp*.]-opd = 
	abort" can't use %esp as a scaled index"

    \ EBP as base reg requires displacement
    dup .[b]-mask and [ebp]-opd = if 0 +#[] then

    \ ESP as base reg requires sib
    dup [b+.*.]-mask and [esp]-opd = if [esp*1]-opd or then

    \ displacement without base register encodes as EBP
    dup .[b]-mask and [ebp]-opd = if reg-ebp or then
;

: >mod-r/m ( reg-byte operand -- )
    \ reg
    dup is-mem? 0= if reg-mask and 00c0 or or db, exit then

    check-memory-operand

    \ join mod and reg bits of mod-r/m
    dup is-imm? if ( reg disp opd )
	dup have-base? if
	    over -80 80 within if 40 else 80 then rot or
	else
	    rot
	then
    else ( reg opd )
	swap
    then

    ( disp? opd mod+reg )
    over have-idx? if
	reg-esp or db, dup
    else
	over reg-mask and or
    then db,

    ( disp? opd )
    dup is-imm? if
	have-base? if
	    dup -80 80 within if db, else dd, then
	else 
	    dd,
	then
    then
;

base !
