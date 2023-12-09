\ Copyright 2011, by J. Richard Barnette
\
base @ hex

internal-defs
\ canonical forms for two operands on stack
\   ( imm reg ) - D=x: ( src dst ), I=1
\   ( imm mem ) - D=x: ( src dst )
\   ( mem reg ) - D=0: ( dst src ), D=1: ( src dst )
\   ( reg reg ) - D=0: ( dst src ), D=1: ( src dst )
\
\   * When TOS is a reg other than the accumulator, second operand is also
\     not the accumulator
\   * Immediate operands omit the # indicator cell.

: 2operand ( opnd opnd -- imm reg | imm mem | mem reg | reg reg )
    dup have-immed? if rot swap then
    dup is-immed? abort" immediate operand used as destination"
    dup is-reg? if
	over is-accum? if dup is-accum? 0= if swap exit then then
	over is-immed? 0= if dir-bit then or
    else
	swap dup is-mem? abort" two memory operands"
	dup is-immed? if drop then
    then
;

: 8-bit-signed? ( immed -- flag ) -80 80 within ;
: 8-bit? ( immed -- flag ) -100 and 0= ;
: 16-bit-signed? ( immed -- flag ) -8000 8000 within ;
: 16-bit? ( immed -- flag ) -10000 and 0= ;
: 32-bit-signed? ( immed -- flag ) -80000000 80000000 within ;
: 32-bit? ( immed -- flag ) -100000000 and 0= ;


\ instruction format:
\   prefixes opcode mod-r/m sib disp imm

\ The three bits in opcode data encode size as follows:
\  RVW     size
\ %000 -  8-bit
\ %011 - 16-bit
\ %001 - 32-bit
\ %101 - 64-bit
\
\ They affect instruction encodeding as follows:
\   W - encodes the w bit in the opcode
\   V - 1 means use data size (66h) override
\   R - encodes the W bit in the REX prefix

: >REX.W ( size -- REX.W ) 4 and 1 lshift ;
: data16, ( size -- ) 2 and if 66 db, then ;
: REX, ( -RXB size -- )
    dup data16, >REX.W or ?dup if 40 or db, then ;
: prefixes0 ( size -- size ) 0 over REX, ;
: prefixes1 ( opnd size -- opnd size )
    >r dup dup is-reg? if >REX.R else >REX.XB then r> tuck REX, ;
: prefixes2 ( opnd1 opnd2 size -- opnd1 opnd2 size )
    >r >r dup >REX.XB r> tuck >REX.R or r> tuck REX, ;

: immed, ( imm size -- )
    \ check imm value doesn't overflow
    dup 1 and if 2 and if dd, else dw, then else drop db, then
;


\ binary operator - ADD, etc.
\ op	0	1	2	3	4	5	6	7
\ 	ADD	OR	ADC	SBB	AND	SUB	XOR	CMP
\	dst,   src
\	b-reg, b-reg	( d=0 w=0 )	-> op*8+dw mod+reg+r/m
\	mem,   b-reg	( d=0 w=0 )	-> op*8+dw mod+reg+r/m
\	w-reg, w-reg	( d=0 w=1 )	-> [ size ] op*8+dw mod+reg+r/m
\	mem,   w-reg	( d=0 w=1 )	-> [ size ] op*8+dw mod+reg+r/m
\	b-reg, mem	( d=1 w=0 )	-> op*8+dw mod+reg+r/m
\	w-reg, mem	( d=1 w=1 )	-> [ size ] op*8+dw mod+reg+r/m
\	AL,    imm	( w=0 )		-> op*8+10w imm8
\	eAX,   imm	( w=1 )		-> [ size ] op*8+10w immw
\	b-reg, imm8	( w=0 )		-> 80 mod+op+r/m imm8
\	b-mem, imm8	( w=0 )		-> 80 mod+op+r/m imm8
\	w-reg, immw	( w=1 )		-> [ size ] 81 mod+op+r/m immw
\	w-mem, immw	( w=1 )		-> [ size ] 81 mod+op+r/m immw
\	w-reg, imm8	( w=1 )		-> [ size ] 83 mod+op+r/m imm8
\	w-mem, imm8	( w=1 )		-> [ size ] 83 mod+op+r/m imm8


: alu-binary-op ( opnd opnd data -- )
    >r 2operand r>
    \ check operand sizes match mnemonic size
    over is-src-immed? if
	prefixes1
	over is-accum? if
	    dup 39 and 4 or db,
	else
	    swap dup have-disp? if rot >r then >r
	    80 r@ 1 and if
		over 8-bit-signed? dup 38 or r> and >r 2 and 1+ +
	    then db,
	    r> r> dup have-disp? if r> swap rot else swap then
	    dup >r 38 and mod-r/m, r>
	then immed,
    else
	prefixes2
	over dir-bit or dir-bit = 2 and swap 39 and or db, >/r mod-r/m,
    then
;

\ INC/DEC
\ op	0	1
\	INC	DEC
\	b-reg		( w=0 )		-> FE+w mod+op+r/m
\	b-mem		( w=0 )		-> FE+w mod+op+r/m
\	b-mem		( w=0 )		-> FE+w mod+op+r/m
\	w-reg		( w=1 )		-> [ size ] FE+w mod+op+r/m
\ 32-bit only:
\	w-reg				-> [ size ] 40+op*8+reg

: inc-dec-op ( opnd data -- )
    prefixes1 fe over 1 and + db, 38 and mod-r/m,
;

\ PUSH/POP
\ op	0	1
\	PUSH	POP
\	w-reg				-> 50+op*8+reg
\	w-mem	POP			-> 8F mod+0+r/m
\	w-mem	PUSH			-> FF mod+6+r/m
\	imm8	PUSH			-> 6A imm8
\	immw	PUSH			-> 68 immw
\
\ 16-bit size override shouldn't be used.

: stack-op ( opnd data -- )
    prefixes1 38 and
    over is-immed? if
	dup abort" can't POP an immediate operand"
	2drop dup 8-bit-signed? if
	    6a db, db,
	else
	    dup 32-bit-signed? 0= abort" PUSH immediate exceeds 32 bits"
	    68 db, dd,
	then exit
    then
    over is-mem? if
	dup if 8f else ff then db, 6 * mod-r/m,
    else
	50 + swap >reg# + db,
    then
;

\ MOV
\	AL addr		( d=0 )		-> A0 addr
\	eAX addr	( d=0 )		-> [ size ] A1 addr
\	addr AL		( d=1 )		-> A2 addr
\	addr eAX	( d=1 )		-> [ size ] A3 addr
\	b-reg b-reg	( d=0 w=0 )	-> 80 mod+reg+r/m
\	mem b-reg	( d=0 w=0 )	-> 80 mod+reg+r/m
\	w-reg w-reg	( d=0 w=1 )	-> [ size ] 81 mod+reg+r/m
\	mem w-reg	( d=0 w=1 )	-> [ size ] 81 mod+reg+r/m
\	b-reg mem	( d=1 w=0 )	-> 82 mod+reg+r/m
\	w-reg mem	( d=1 w=1 )	-> [ size ] 83 mod+reg+r/m
\	b-reg imm8			-> B0+reg imm8
\	w-reg immw			-> [ size ] B8+reg immw
\	b-mem imm8	( w=0 )		-> C6 mod+0+r/m imm8
\	w-mem immw	( w=1 )		-> [ size ] C6 mod+0+r/m immw
\	w-mem immw	( w=1 )		-> [ size ] C6 mod+0+r/m immw

: get-mnemonic-size
    >in @ >r parse-name r> >in ! 1- chars + c@ case
	'b' of 0 endof
	'w' of 3 endof
	'l' of 1 endof
	'q' of 5 endof
	." invalid end character on mnemonic" cr abort
    endcase
;

: def-alu-binary:
    3 lshift get-mnemonic-size or create c,
    does> c@ alu-binary-op ;

: def-inc-dec:
    3 lshift get-mnemonic-size or create c,
    does> c@ inc-dec-op ;

: def-stack:
    3 lshift get-mnemonic-size or create c,
    does> c@ stack-op ;

asm-defs
0 def-alu-binary: addb
0 def-alu-binary: addw
0 def-alu-binary: addl
0 def-alu-binary: addq

1 def-alu-binary: orb
1 def-alu-binary: orw
1 def-alu-binary: orl
1 def-alu-binary: orq

2 def-alu-binary: adcb
2 def-alu-binary: adcw
2 def-alu-binary: adcl
2 def-alu-binary: adcq

3 def-alu-binary: sbbb
3 def-alu-binary: sbbw
3 def-alu-binary: sbbl
3 def-alu-binary: sbbq

4 def-alu-binary: andb
4 def-alu-binary: andw
4 def-alu-binary: andl
4 def-alu-binary: andq

5 def-alu-binary: andb
5 def-alu-binary: andw
5 def-alu-binary: andl
5 def-alu-binary: andq

6 def-alu-binary: xorb
6 def-alu-binary: xorw
6 def-alu-binary: xorl
6 def-alu-binary: xorq

7 def-alu-binary: cmpb
7 def-alu-binary: cmpw
7 def-alu-binary: cmpl
7 def-alu-binary: cmpq

0 def-inc-dec: incb
0 def-inc-dec: incw
0 def-inc-dec: incl
0 def-inc-dec: incq

1 def-inc-dec: decb
1 def-inc-dec: decw
1 def-inc-dec: decl
1 def-inc-dec: decq

0 def-stack: pushq
1 def-stack: popq

base !
