There are two relatively distinct conventions for Intel/AMD instruction
assembly language syntax.  One is used in the Intel and AMD architecture
and programmer reference manuals.  Generally, it looks something like
this:
```
	MOV	RAX, 5
	MOV	RAX, [RBP + RDX*8]
```
The ordering of operands is `_dst_, _src_`.  Operand sizes are usually
determined implicitly from the operand registers.  In some cases,
keywords like `WORD PTR` may be needed for memory operands to indicate
size when no register is present to indicate.  Some instructions with no
operands encode size with a suffix, e.g. `LODSQ`.

The other syntax comes from Unix (back when AT&T owned the source code
and trademarks).  It looks like this:
```
	movl	5, %rax
	movl	(%rbp,%rdx,8), %rax
```
The operand ordering is `_src_, _dst_`.  Register names start with '%',
Memory operands are denoted with `_disp_(_base_,_index_,_scale_)`.
Operand sizes are encoded in a suffix on the mnemonic, and must agree
with any actual operands.

Linux and macOS use the AT&T conventions.  Anything derived from Unix
(e.g. Solaris) also should use them.  Microsoft's assembler uses the
Intel/AMD conventions.

The assembler here leans more towards the Unix/AT&T conventions.
Here's some of the work needed to support either/both.

First, there would need to be separate vocabularies to account for
the different register names and mnemonics, and one or the other would
have to be selected based on the chosen conventions.

Second, there should be some thought as to whether Forth memory
operands should be denoted with round or square brackets.  The
Intel convention would also need things like `WORD-PTR`, `DWORD-PTR`,
etc.

Finally, handling operand order involves factoring 2operand into these
parts:
```
: 2operand ( opnd opnd -- imm reg | imm mem | mem reg | reg reg )
    dup is-immed? abort" immediate operand used as destination"
    dup is-reg? if
	over is-accum? if dup is-accum? 0= if swap exit then then
	over is-immed? 0= if dir-bit then or
    else
	swap dup is-mem? abort" two memory operands"
	dup is-immed? if drop then
    then
;

\ dst, src
: 2operand-Intel ( opnd opnd -- imm reg | imm mem | mem reg | reg reg )
    dup have-immed? if rot else swap then 2operand
    dup have-disp? if >r swap r> then
;

\ src, dst
: 2operand-Unix ( opnd opnd -- imm reg | imm mem | mem reg | reg reg )
    dup have-immed? if rot swap then 2operand
;
```
