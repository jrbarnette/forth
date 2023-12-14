\  Copyright 2019, by J. Richard Barnette. All Rights Reserved.

ONLY FORTH definitions

\ We accumulate the target direct-mode code into a local buffer that
\ we flush at the end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE).

create    target-buff	24 1024 * cells allot
variable  tp		target-buff tp !

create direct-forth-name 80 chars allot 

: direct-name@ direct-forth-name count ;
: .direct-forth direct-name@ type ;
: direct-name! direct-forth-name 2dup c! char+ swap chars move ;

1 cells 8 * 3 - constant tag-shift
: +tag tag-shift lshift or ;
: -tag [ 0 invert tag-shift lshift invert ] literal and ;
: +bias target-buff - ;
: -bias target-buff + ;
: >offset +bias 1 cells / ;

: .cell     ." .cell = " .c-hex ;
: .expr     ." .cell = (cell_ft) (" -tag -bias count type ." )" ;
: .str      ." .id = " -tag -bias count .c-string ;
: .label    ." .ip = &" .direct-forth ." [" -tag .c-decimal ." ]" ;
: .handler  ." .handler = " -tag -bias count type ;

: .offset base @ swap decimal >offset 5 .r base ! ;
: .{ ." /* " .offset ."  */    { " ;
: }, ."  }," cr ;

here
    ' .cell ,		\ tag #0
    ' .expr ,		\ tag #1
    ' .str ,		\ tag #2
    ' .label ,		\ tag #3
    ' .handler ,	\ tag #4
    0 ,			\ tag #5
    0 ,			\ tag #6
    ' .cell ,		\ tag #7

: >entry tag-shift rshift cells [ swap ] literal + ;
: .entry ( a-addr -- ) dup .{ @ dup >entry @ execute }, ;

: >target ( tagged -- ) tp @ tuck ! cell+ tp ! ;
: biased>target ( c-addr tag -- ) >r +bias r> +tag >target ;
: cell>target ( x -- ) >target ;
: expr>target ( c-addr -- ) #1 biased>target ;
: str>target ( c-addr -- )  #2 biased>target ;
: label>target ( a-addr -- ) >target ;
: handler>target ( c-addr -- ) #4 biased>target ;


\ We only need these while building up the ASSEMBLER vocabulary.
vocabulary BUILD-ASM
also BUILD-ASM definitions
: prim: create parse-name counted, does> handler>target ;
: create-ref create tp @ >offset #3 +tag , ;

: IMMEDIATE       ;
: COMPILE-ONLY    ;
: COMPILE-SPECIAL ;

vocabulary ASSEMBLER

: >>> source >in @ over >in ! swap over - >r chars + r> type cr ;
: direct-forth: parse-name direct-name! target-buff tp ! ONLY ASSEMBLER ;

also ASSEMBLER definitions previous
: ( postpone ( ;
: \ postpone \ ;
: .( postpone .( ;

: end-direct
    ." DIRECT_FORTH("  .direct-forth ." ) // {" cr
    tp @ target-buff begin 2dup > while dup .entry cell+ repeat 2drop
    ." END_DIRECT // }" cr
    ONLY FORTH
;

ONLY FORTH

also assembler definitions
: operands> r> ;
: operand dup @ swap cell+ ;
: <operands >r ;

direct-forth: direct_program
code char+ ( c-addr -- c-addr )
    1 literal +
end-code

code cell+ ( a-addr -- a-addr )
    <C> CELL_SIZE; +
end-code

code aligned ( c-addr -- a-addr )
    <C> CELL_SIZE - 1; + <C> -CELL_SIZE; and
end-code

code addchar ( here char -- here' )
    over c! char+
end-code

code addcell ( here x -- here' )
    over ! cell+
end-code

code addname ( wid here I: name len handler -- wid' here' )
    dup rot addcell
    operands>
    swap >r operand operand operand r> swap
    <operands
    over addcell over over + aligned >r swap move
    r> swap addcell
end-code

code setflags
    over r> operand >r swap cell+ dup >r c@ or r> c!
end-code

ref: entry
end-direct
