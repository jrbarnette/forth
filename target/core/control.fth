\ +LOOP                 6.1.0140 CORE                   27
\ BEGIN                 6.1.0760 CORE                   34
\ DO                    6.1.1240 CORE                   36
\ ELSE                  6.1.1310 CORE                   37
\ I                     6.1.1680 CORE                   39
\ IF                    6.1.1700 CORE                   40
\ J                     6.1.1730 CORE                   40
\ LEAVE                 6.1.1760 CORE                   41
\ LOOP                  6.1.1800 CORE                   41
\ REPEAT                6.1.2140 CORE                   44
\ THEN                  6.1.2270 CORE                   46
\ UNLOOP                6.1.2380 CORE                   47
\ UNTIL                 6.1.2390 CORE                   47
\ WHILE                 6.1.2430 CORE                   47
\ ?DO                   6.2.0620 CORE EXT               51
\ AGAIN                 6.2.0700 CORE EXT               51
\ CASE                  6.2.0873 CORE EXT               52
\ ENDCASE               6.2.1342 CORE EXT               53
\ ENDOF                 6.2.1343 CORE EXT               53
\ OF                    6.2.1950 CORE EXT               54

: dest, ( dest -- ) here cell+ - , ;
: orig> ( -- orig ) here 1 cells allot ;

: then ( C: orig -- ) here 1 cells - over - swap ! ;	\ compile-only
immediate

: begin ( C: -- dest) here ; immediate			\ compile-only

: else ( C: orig1 -- orig2 )				\ compile-only
    postpone skip orig> swap postpone then
; immediate

: if ( C: -- orig ) postpone fskip orig> ;		\ compile-only
immediate

: while ( C: dest -- orig dest ) postpone if swap ;	\ compile-only
immediate

: again ( C: dest -- ) postpone skip dest, ;		\ compile-only
immediate

: until ( C: dest -- ) postpone fskip dest, ;		\ compile-only
immediate

: repeat ( C: orig dest -- )				\ compile-only
    postpone again postpone then
; immediate

variable leavers 0 leavers !
: leavers-swap ( nleavers -- oleavers )
    leavers @ swap leavers !
;
: do ( C: -- leavers dest )
    postpone over postpone - postpone 2>r
    0 leavers-swap
    postpone begin
; immediate

: unloop postpone 2r> postpone 2drop ; immediate

: next-loop ( n idx -- idx n+idx n+idx ) swap over + dup ;

: leave postpone skip here leavers-swap , ; immediate
: +loop
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

: loop 1 postpone +loop ; immediate

: i postpone 2r@ postpone + ; immediate \ compile-only
