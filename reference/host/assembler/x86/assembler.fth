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
    db-col @ if ." , " else 9 emit ." .byte " then
    base @ >r #16 base ! ." 0x" 0 <# # # #> type r> base !
    1 db-col +!
;
: dw, dup db,  $8 rshift db, ;
: dd, dup dw, $10 rshift dw, ;
: newline cr 0 db-col ! ;
