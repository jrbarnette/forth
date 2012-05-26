\ Copyright 2011, by J. Richard Barnette
\ ------  ------  ------  ------  ------  ------  ------

\ anonymous defintions
\ create-prim skip
\ create-prim fskip
: dest, ( dest -- ) here cell+ - , ;
: orig> ( -- orig ) here 1 cells allot ;
\ target definitions

\ THEN                  6.1.2270 CORE                   46
\ interpretation semantics undefined; special compilation semantics
: THEN ( C: orig -- ) here 1 cells - over - swap ! ; immediate

\ BEGIN                 6.1.0760 CORE                   34
\ interpretation semantics undefined; special compilation semantics
: BEGIN ( C: -- dest) here ; immediate

\ ELSE                  6.1.1310 CORE                   37
\ interpretation semantics undefined; special compilation semantics
: ELSE ( C: orig1 -- orig2 )
    postpone skip orig> swap postpone then
; immediate

\ IF                    6.1.1700 CORE                   40
\ interpretation semantics undefined; special compilation semantics
: IF ( C: -- orig ) postpone fskip orig> ; immediate

\ WHILE                 6.1.2430 CORE                   47
\ interpretation semantics undefined; special compilation semantics
: WHILE ( C: dest -- orig dest ) postpone if swap ; immediate

\ AGAIN                 6.2.0700 CORE EXT               51
\ interpretation semantics undefined; special compilation semantics
: AGAIN ( C: dest -- ) postpone skip dest, ; immediate

\ UNTIL                 6.1.2390 CORE                   47
\ interpretation semantics undefined; special compilation semantics
: UNTIL ( C: dest -- ) postpone fskip dest, ; immediate

\ REPEAT                6.1.2140 CORE                   44
\ interpretation semantics undefined; special compilation semantics
: REPEAT ( C: orig dest -- ) postpone again postpone then ; immediate

\ anonymous defintions
variable leavers 0 leavers !
: leavers-swap ( nleavers -- oleavers ) leavers @ swap leavers ! ;
: next-loop ( n idx -- idx n+idx n+idx ) swap over + dup ;
\ target definitions

\ DO                    6.1.1240 CORE                   36
\ interpretation semantics undefined; special compilation semantics
\ ( R: loop-sys -- ) runtime semantics
: DO ( C: -- leavers dest )
    postpone over postpone - postpone 2>r
    0 leavers-swap
    postpone begin
; immediate

\ UNLOOP                6.1.2380 CORE                   47
\ interpretation semantics undefined
\ ( R: loop-sys -- ) runtime semantics
\ special compilation semantics
: UNLOOP postpone 2r> postpone 2drop ; immediate

\ LEAVE                 6.1.1760 CORE                   41
\ interpretation semantics undefined
\  ( R: loop-sys -- ) execution semantics
\ XXX does POSTPONE LEAVE work right?
: LEAVE postpone skip here leavers-swap , ; immediate

\ +LOOP                 6.1.0140 CORE                   27
: +LOOP
    ( leavers dest -- )
    postpone r>
    postpone next-loop
    postpone >r
    postpone xor
    postpone 0<
    postpone until
    leavers-swap
    begin dup while
	dup @ swap postpone then
    repeat drop
    postpone unloop
;

\ LOOP                  6.1.1800 CORE                   41
: LOOP 1 postpone +loop ; immediate

\ I                     6.1.1680 CORE                   39
\ XXX does POSTPONE I work right?
: I postpone 2r@ postpone + ; immediate \ compile-only

\ J                     6.1.1730 CORE                   40

\ ?DO                   6.2.0620 CORE EXT               51
\ CASE                  6.2.0873 CORE EXT               52
\ ENDCASE               6.2.1342 CORE EXT               53
\ ENDOF                 6.2.1343 CORE EXT               53
\ OF                    6.2.1950 CORE EXT               54
