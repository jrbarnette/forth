\ Copyright 2023, by J. Richard Barnette
\
vocabulary ASSEMBLER
also assembler definitions
vocabulary ASM-INTERNAL
also asm-internal definitions
: asm-defs also assembler definitions previous ;
: internal-defs also asm-internal definitions previous ;

variable db-col 0 db-col !

: db, ( x -- )
    db-col @ if bl emit then
    base @ >r hex 0 <# # # #> type r> base !
    db-col @ 1 + dup 10 = if cr drop 0 then db-col !
;
: dw, dup db, $08 rshift db, ;
: dd, dup dw, $10 rshift dw, ;
: newline cr 0 db-col ! ;
