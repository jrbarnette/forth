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
3000 constant .[.]-mask
3007 constant .[b]-mask
80c0 constant [.*s]-mask
80f8 constant [x*s]-mask
9007 constant [b+.*.]-mask
9038 constant [.+x*.]-mask
b0c7 constant .[b+.*s]-mask

1000 constant reg-opd
2000 constant imm-opd
4000 constant mem-opd
8000 constant idx-opd

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
0020 constant idx-esp
0007 constant reg-mask
0038 constant idx-mask
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

: is-imm? ( operand -- flag ) format-mask and imm-opd = ;
: is-reg? ( operand -- flag ) format-mask and reg-opd = ;
: is-mem? ( operand -- flag ) mem-opd and 0<> ;
: is-byte? ( operand -- flag ) size-mask and size-byte = ;
: is-word? ( operand -- flag ) size-mask and size-word = ;
: is-dword? ( operand -- flag ) size-mask and size-dword = ;
: have-imm? ( operand -- flag ) imm-opd and 0<> ;
: have-base? ( operand -- flag ) reg-opd and 0<> ;
: have-idx? ( operand -- flag ) idx-opd and 0<> ;
: base>idx ( operand -- operand ) 3 lshift ;
: idx>base ( operand -- operand ) 3 rshift ;

\ swap base and index fields
: base<->index ( operand -- operand )
    dup dup idx>base xor [b]-mask and dup base>idx or xor ;

: #
    dup -80 80 within if size-byte imm-opd or exit then
    dup -8000 8000 within if size-word imm-opd or exit then
    size-dword imm-opd or
;

mem-direct constant #[]

: check-addr ( operand -- operand )
    dup is-reg? 0= abort" [] operand must be a register"
    dup is-dword? 0= abort" only 32-bit addressing is supported"
    reg-mask and ;
: >scaled ( regop ss -- memop ) swap check-addr base>idx or ;

: [] ( regop -- memop ) check-addr mem-indirect or ; 
: [*2] ( regop -- memop ) mem-indexed*2 >scaled ;
: [*4] ( regop -- memop ) mem-indexed*4 >scaled ;
: [*8] ( regop -- memop ) mem-indexed*8 >scaled ;

: check-scaled ( memop regop ss )
    >scaled over have-idx? abort" can only scale one register"
    or ;

: +[] ( memop regop -- memop )
    over have-base?
    if   [.*1]-opd check-scaled
    else check-addr reg-opd or or
    then ;
: +#[] ( memop disp -- disp memop )
    over have-imm? abort" can't apply +#[] to immediate or address"
    swap #[] or ;
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
	dup idx-mask and idx-esp =		\ [b+ESP*1] -> [ESP+b]
	over have-base? 0= or			\ [EBP+x*1] -> [x+EBP*1]
	over reg-mask and ebp-reg = or		\ [x*1] -> [x]
	if base<->index then
    then

    \ ESP*n - ESP not allowed as index
    dup [.+x*.]-mask and [.+esp*.]-opd = 
	abort" can't use %esp as a scaled index"

    \ EBP as base reg requires displacement
    dup .[b]-mask and [ebp]-opd = if 0 +#[] then

    \ ESP as base reg without index requires sib
    dup [b+.*.]-mask and [esp]-opd = if [esp*1]-opd or then

    \ displacement without base register encodes as EBP
    dup .[.]-mask and imm-opd = if reg-ebp or then
;

: >mod-r/m ( reg-byte operand -- )
    \ reg
    dup is-mem? 0= if reg-mask and 00c0 or or db, exit then

    check-memory-operand

    \ join mod and reg bits of mod-r/m
    dup have-imm? if ( reg disp opd )
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
    dup have-imm? if
	have-base? if
	    dup -80 80 within if db, else dd, then
	else 
	    dd,
	then
    then
;

base !
