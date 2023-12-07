: # ( ud1 -- ud2 ) 0 base @ um/mod >r base @ um/mod swap digit>char hold r> ;

also forth definitions previous
: get-bits ( ud1 nbits -- ud2 bits )
    1 swap lshift >r 0 r@ um/mod r> swap >r um/mod swap r> swap ;
: show-operand
    dup >r check-memopnd 7 mod-r/m
    #30 db-col @ 5 * - spaces ."   # "
    [ only forth ]
    r> 0 <#
	8 base ! 2 0 do # bl hold loop
	2 get-bits 1 swap lshift '0' + hold '*' hold bl hold
	hex # bl hold
	1 get-bits if 'B' else '-' then hold
	1 get-bits if 'I' else '-' then hold
	1 get-bits if 'M' else '-' then hold
	1 get-bits if 'X' else '-' then hold
	bl hold
	2 base ! # # # #
    #> type hex
    [ also assembler also asm-internal ]
    newline
    depth 0 <> if ." not all arguments consumed" cr drop then ;

%rax			show-operand
%rdx			show-operand
cr
%rax []			show-operand
%rdx []			show-operand
%rsp []			show-operand
%rbp []			show-operand
%rbp %rdx [+]		show-operand
%rdx %rsp [+]		show-operand
%rdx [*2]		show-operand
%rax %rdx [+*2]		show-operand
%rbp %rdx [+*2]		show-operand
cr
01 %rax #[.]		show-operand
01 %rdx #[.]		show-operand
01 %rsp #[.]		show-operand
01 %rbp #[.]		show-operand
01 %rbp %rdx #[+]	show-operand
01 %rdx %rsp #[+]	show-operand
01 %rdx #[*2]		show-operand
01 %rax %rdx #[+*2]	show-operand
01 %rbp %rdx #[+*2]	show-operand
cr
0201 %rax #[.]		show-operand
0201 %rdx #[.]		show-operand
0201 %rsp #[.]		show-operand
0201 %rbp #[.]		show-operand
0201 %rbp %rdx #[+]	show-operand
0201 %rdx %rsp #[+]	show-operand
0201 %rdx #[*2]		show-operand
0201 %rax %rdx #[+*2]	show-operand
0201 %rbp %rdx #[+*2]	show-operand
