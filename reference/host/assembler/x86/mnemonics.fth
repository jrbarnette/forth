\ Copyright 2011, by J. Richard Barnette
\

( imm imm-opd xxx -- error )
( [ disp ] mem-opd [ disp ] mem-opd -- error )
( [ disp ] mem-opd imm imm-opd -- imm [ disp ] imm-opd mem-opd )
( [ disp ] mem-opd reg-opd -- [ disp ] reg-opd dst-mem-opd )
( reg-opd [ disp ] mem-opd -- [ disp ] reg-opd src-mem-opd )
: 2operand
    dup have-imm? if rot else swap then
    dup is-imm? abort" immediate operand used as destination"
    over over and dup
    is-mem? abort" two memory operands"
    have-imm? if >r rot swap r> then
    over is-mem? if swap
	\ flag as reg-to-memory
    then
;

\ instruction format:
\   prefixes opcode mod-r/m sib disp imm

\ binary operator - ADD, etc.
\ op	0	1	2	3	4	5	6	7
\ 	ADD	OR	ADC	SBB	AND	SUB	XOR	CMP
\	dst src
\	b-reg b-reg	( d=0 w=0 )	-> op*8+00 mod+reg+r/m
\	mem b-reg	( d=0 w=0 )	-> op*8+00 mod+reg+r/m
\	w-reg w-reg	( d=0 w=1 )	-> [ size ] op*8+01 mod+reg+r/m
\	mem w-reg	( d=0 w=1 )	-> [ size ] op*8+01 mod+reg+r/m
\	b-reg mem	( d=1 w=0 )	-> op*8+02 mod+reg+r/m
\	w-reg mem	( d=1 w=1 )	-> [ size ] op*8+03 mod+reg+r/m
\	AL imm		( w=0 )		-> op*8+04 imm8
\	eAX imm		( w=1 )		-> [ size ] op*8+05 immw
\	b-reg imm8	( w=0 )		-> 80 mod+op+r/m imm8
\	b-mem imm8	( w=0 )		-> 80 mod+op+r/m imm8
\	w-reg immw	( w=1 )		-> [ size ] 81 mod+op+r/m immw
\	w-mem immw	( w=1 )		-> [ size ] 81 mod+op+r/m immw
\	w-reg imm8	( w=1 )		-> [ size ] 83 mod+op+r/m imm8
\	w-mem imm8	( w=1 )		-> [ size ] 83 mod+op+r/m imm8


\ INC/DEC
\ op	0	1
\	INC	DEC
\	b-reg				-> FE mod+op+r/m
\	w-reg				-> [ size ] 50+op*8+reg
\	b-mem				-> FE mod+op+r/m
\	w-mem				-> [ size ] FF mod+op+r/m

\ PUSH/POP
\ op	0	1
\	PUSH	POP
\	w-reg				-> [ size ] 60+op*8+reg
\	w-mem	POP			-> [ size ] 8F mod+0+r/m
\	w-mem	PUSH			-> [ size ] FF mod+6+r/m

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
