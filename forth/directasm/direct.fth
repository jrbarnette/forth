\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

DIRECT-MODE definitions

\ We accumulate assembled code into a local buffer that we flush at
\ the end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE).
\
\ The buffer has the following parts:
\   direct-buffer	The actual cells of the direct-mode code
\   tag-buffer		Tags for .entry; one tag per cell in direct-buffer
\   strings-buffer	Storage for strings for .handler and .expr

24 1024 *
here over cells allot		constant direct-buffer
here swap chars aligned allot	constant tag-buffer
here 32 1024 * chars allot	constant strings-buffer

variable  strp		strings-buffer strp !

variable  origin
variable  origin-index
variable  origin-indent

: initialize-origin ( -- )
    1 origin-indent ! 0 origin-index ! 0 origin ! ;
: advance-origin ( indent -- )
    origin-indent ! origin @ origin-index +! 0 origin ! ;
initialize-origin

\ Tag interpretation
\ Every cell has an associated tag byte that determines how the cell
\ should be printed by flush-direct-buffer.
\
\ The top two bits of the tag value are the "indent" state that
\ determines what text is printed between this cell and the next
\ cell as follows:
\   0 - No output should go between this cell and the next.
\   1 - This cell is the last cell on the line.  The next cell
\       should be printed with a comment showing its buffer offset.
\   2 - As with index 1, plus the next cell should be indented
\       by 4 spaces following the offset comment.
\
\ The bottom 6 bits of the tag are the format code.
\ ... (explain the encoding of the "format code") ...

6			constant #fmt-bits
1 #fmt-bits lshift 1-	constant fmt-mask

: direct-ptr ( index -- a-addr ) cells direct-buffer + ;
: tag-ptr ( index -- c-addr ) chars tag-buffer + ;
: tag@ ( index -- indent fmt )
    tag-ptr c@ dup #fmt-bits rshift swap fmt-mask and ;
: >tag ( fmt indent -- tag ) #fmt-bits lshift + ;
: tag! ( tag index -- ) tag-ptr c! ;

: assemble-cell ( x fmt indent -- )
    >tag origin @ dup 1+ origin ! tuck tag! direct-ptr ! ;


0 constant >.cell
1 constant >.expr
2 constant >.id
3 constant >.exec
: >.str ( c-addr u -- c-addr fmt ) 3 + ;

\ table of .entry handlers
:noname swap @ swap .str ;	\ fmt $04 + len - index 4
:noname @ .cell ;		\ fmt $00       - index 3
:noname @ count .expr ;		\ fmt $01       - index 2
:noname @ .id ;			\ fmt $02       - index 1
:noname @ count .exec ;		\ fmt $03       - index 0

:noname does> ( fmt table -- len xt | xt )
    swap $03 - dup 0> if swap -4 then negate cells + @ ;
create fmt>handler execute
\ 0 1 2 3 4  - index
\ 3 2 1 0 4+ - fmt
  , , , , ,


: .indent ( indent -- )  1- if 4 spaces then ;
: .index ( index indent -- index )
    ?dup if ." /* " over origin-index @ + 4 u.r ."  */" .indent then ;
: start-entry ( index indent -- indent' a-addr fmt )
    .index dup tag@ rot direct-ptr swap ;
: end-entry ( indent -- indent ) dup if cr then ;

: .entry ( index indent -- indent' )
    start-entry ."  { " fmt>handler execute ."  }," end-entry ;

: flush-direct-buffer
    decimal origin-indent @ origin @ 0 do
	i swap .entry
    loop advance-origin ;


variable emit-state  0 emit-state !

: meta-instr 2 assemble-cell ;
: start-meta ( meta-state -- )
    if c" meta_compile" else c" meta_interpret" then >.exec meta-instr ;


: end-instr 1 assemble-cell ;
: start-instr ( new-state -- )
    dup emit-state @ <> if
	emit-state @ if 0 >.id end-instr then
	dup if dup 1- start-meta then
	dup emit-state !
    then drop
;

: {  0 start-instr ;
: }  end-instr ;
: }{ 0 assemble-cell ;

\ : >strings-s ( c-addr u -- c-addr' u )
\     dup >r chars tuck strp @ dup >r swap move strp +! r> r> ;

: >strings ( c-addr u -- c-addr' )
    strp @ 2dup c! char+ swap chars dup >r move strp @ r> char+ strp +! ;

: meta-cell ( name len state -- ) start-instr >.str meta-instr ;
: meta-interpret ( name len -- ) 1 meta-cell ;
: meta-compile ( name len -- ) 2 meta-cell ;
: meta-emit ( name len state -- )
    start-instr >strings >.id meta-instr ;

: handler? ( c-addr u | 0 -- c-addr u ) ?dup 0= if parse-name then >strings ;
: do-name { >.exec }{ parse-name >strings >.id }{ handler? >.exec } ;
: addname ( c-addr u | 0 -- ) c" i_addname" do-name ;
: startname ( c-addr u | 0 -- ) c" i_startname" do-name ;
: linkname ( -- ) { c" i_linkname" >.exec } ;
: setflags ( flags -- ) { c" i_setflags" >.exec }{ >.cell } ;
: direct-lookup ( c-addr u -- ) { c" i_lookup" >.exec }{ >strings >.id } ;

\ N.B. do-literal isn't _unnecessary_ complexity.  Both .offset and
\ .c-hex use pictured string formatting.  Thus, the following
\ sequence breaks because { will plaster the output from .c-hex:
\     .c-hex { c" do_literal" >.exec }{ >.expr }
: do-literal { c" do_literal" >.exec }{ execute } ;
: literal-expr ( c-addr u -- ) >strings ['] >.expr do-literal ;
: literal-cell ( x -- ) ['] >.cell do-literal ;
: literal-handler ( c-addr u ) >strings ['] >.exec do-literal ;

: prim: create parse-name counted, does> { >.exec } ;

: IMMEDIATE       ;
: COMPILE-ONLY    ;
: COMPILE-SPECIAL ;

\ ---- ----
\ meta-emit
\ meta-interpret
\ meta-compile
\ addname
\ startname
\ linkname
\ setflags
\ direct-lookup
\ literal-expr
\ literal-cell
\ literal-handler
\ prim:

vocabulary DIRECT-COMPILE
DIRECT-MODE also DIRECT-COMPILE definitions previous
