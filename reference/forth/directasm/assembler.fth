\ Copyright 2020, by J. Richard Barnette, All Rights Reserved.

DIRECT-MODE definitions

\ We accumulate assembled code into a local buffer that we flush at
\ the end of processing.
\
\ The buffer is needed for cases where we backpatch content, as for
\ forward branches (e.g. IF, WHILE).
\
\ The buffer has the following parts:
\   asm-buffer		The actual cells of the assembled code
\   tag-buffer		Tags for .entry, packed as bit fields
\   strings-buffer	Storage for strings for .handler and .expr

6			constant #tag-bits
1 #tag-bits lshift 1-	constant tag-mask

24 1024 *
here over cells allot		constant asm-buffer
here swap chars aligned allot	constant tag-buffer
here 4096 chars allot		constant strings-buffer

variable  origin	asm-buffer origin !
variable  strp		strings-buffer strp !

\ : >buffer ( index -- a-addr ) cells asm-buffer + ;
\ : buffer@ ( index -- ) >buffer @ ;
\ : buffer! ( x index -- ) >buffer ! ;
: >index ( a-addr -- index ) asm-buffer - [ 1 cells ] literal / ;

: >tag ( a-addr -- c-addr ) >index chars tag-buffer + ;
: tag@ ( a-addr -- indent tag )
    >tag c@ dup #tag-bits rshift swap tag-mask and ;

: origin>tag ( -- c-addr ) origin @ [ 1 cells ] literal - >tag ;
: origin>tag! ( tag -- ) origin>tag c! ;
: origin>tag-state! ( indent -- )
    origin>tag swap #tag-bits lshift over c@ or swap c! ;


\ table of entry handlers
:noname swap @ swap .str ;	\ tags $10-$3f - index $10
:noname @ .cell ;		\ tag  $00     - index $f
:noname @ count .expr ;		\ tag  $01     - index $e
:noname @ count .exec ;		\ tag  $02     - index $d
:noname @ count .str ;		\ tag  $03     - index $c
:noname @ .str ;		\ tag  $04     - index $b
:noname drop ." invalid tag" ;	\ tag  $05     - index $a
dup				\ tag  $06     - index $9
dup				\ tag  $07     - index $8
dup				\ tag  $08     - index $7
dup				\ tag  $09     - index $6
dup				\ tag  $0a     - index $5
dup				\ tag  $0b     - index $4
dup				\ tag  $0c     - index $3
dup				\ tag  $0d     - index $2
dup				\ tag  $0e     - index $1
dup				\ tag  $0f     - index $0

:noname does> ( tag table -- len xt | xt )
    swap $f - dup 0> if swap $-10 then negate cells + @ ;
create tag>handler execute
\ 0 1 2 3 4 5 6 7 8 9 a b c d e f 10  - index
\ f e d c b a 9 8 7 6 5 4 3 2 1 0 10  - tag
  , , , , , , , , , , , , , , , , ,

: .indent ( indent -- )  1- if 4 spaces then ;
: .index ( a-addr indent -- a-addr )
    ?dup if ." /* " over >index 4 u.r ."  */" .indent then ;
: start-entry ( a-addr indent -- indent' a-addr tag )
    .index dup tag@ rot swap ;
: end-entry ( indent -- indent ) dup if cr then ;

: .entry ( a-addr indent -- indent' )
    start-entry ."  { " tag>handler execute ."  }," end-entry ;

: flush-asm-buffer
    decimal 1 origin @ asm-buffer do
	i swap .entry
    [ 1 cells ] literal +loop drop
;

\ : flush-asm-buffer
\     decimal origin @ asm-buffer 1 >r
\     begin 2dup > while
\ 	dup r> .entry >r cell+
\     repeat r> drop 2drop
\ ;

: >strings ( c-addr u -- c-addr' )
    strp @ 2dup c! char+ swap chars dup >r move strp @ r> char+ strp +! ;
: assemble-cell ( x tag -- )
    origin @ swap over >tag c! ! [ 1 cells ] literal origin +! ;
: cell-allocator: create c, does> c@ assemble-cell ;

\ three kinds of string allocator:
\ ( c-addr u ) -> counted allocated in string space (tag $3)
\ ( c-addr u ) -> pre-allocated, length in tag (tag $10-$3f)
\ ( c-addr ) -> pre-allocated counted string (tag $3)

0 cell-allocator: >.cell
1 cell-allocator: >.expr
2 cell-allocator: >.exec

: .str=NULL 0 $44 assemble-cell ;
: >.meta if c" meta_compile" else c" meta_interpret" then
	 $82 assemble-cell ;

variable emit-state  0 emit-state !

: end-instr ( indent -- )
: start-instr ( new-state -- )
    dup emit-state @ <> if
	emit-state @ ?dup if .str=NULL then
	dup if dup 1- >.meta then
	dup emit-state !
    then drop
;

: meta-cell ( name tag/len state -- ) start-instr assemble-cell end-instr ;
: meta-interpret ( name len -- ) $10 + 1 meta-cell ;
: meta-compile ( name len -- ) $10 + 2 meta-cell ;
: meta-emit ( name len state -- ) >r >strings $03 r> meta-cell

: {  0 start-instr ;
: }  emit-state @ 0= 2 + origin>tag-state! ;
: }{ ;

: handler? ( c-addr u | 0 -- c-addr u ) ?dup 0= if parse-name >strings then ;
: do-name { >.exec }{ parse-name >strings >.str }{ handler? >.exec } ;
: addname ( c-addr u | 0 -- ) s" i_addname" do-name ;
: startname ( c-addr u | 0 -- ) s" i_startname" do-name ;
: linkname ( -- ) { s" i_linkname" >.exec } ;
: setflags ( flags -- ) { s" i_setflags" >.exec }{ .cell } ;
: direct-lookup ( c-addr u -- ) { s" i_lookup" >.exec }{ >strings .str } ;
