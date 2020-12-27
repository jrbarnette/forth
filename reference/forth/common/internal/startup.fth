\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

: parse-remainder source >in @ over >in ! swap over - >r chars + r> ;
: start-errors here 0 ;
: no-error  1+ here swap ;
: error:    1+ parse-remainder chars, here swap ;
: error-table:
    ( maxerr ptrn ... ptr0 n "spaces<name>" -- )
    create here >r 2 cells allot dup >r
    0 ?do over - [ 1 chars ] literal / , dup , loop drop
    1+ dup r> - r> 2!
    does> ( throw-code body -- c-addr u | 0 )
    2dup 2@ within if 2drop 0 exit then
    swap over @ - 1+ 2* cells + 2@ dup 0= if 2drop 0 then ;

-1 start-errors
(  -1 ) no-error  \ ABORT
(  -2 ) error: -2 THROW invoked without using ABORT"
(  -3 ) error: stack overflow
(  -4 ) error: stack underflow
(  -5 ) error: return stack overflow
(  -6 ) error: return stack underflow
(  -7 ) error: do-loops nested too deeply during execution
(  -8 ) error: dictionary overflow
(  -9 ) error: invalid memory address
( -10 ) error: division by zero
( -11 ) error: result out of range
( -12 ) error: argument type mismatch
( -13 ) error: undefined word
( -14 ) error: interpreting a compile-only word
( -15 ) error: invalid FORGET
( -16 ) error: attempt to use zero-length string as a name
( -17 ) error: pictured numeric output string overflow
( -18 ) error: parsed string overflow
( -19 ) error: definition name too long
( -20 ) error: write to a read-only location
( -21 ) error: unsupported operation (e.g., AT-XY on a too-dumb terminal)
( -22 ) error: control structure mismatch
( -23 ) error: address alignment exception
( -24 ) error: invalid numeric argument
( -25 ) error: return stack imbalance
( -26 ) error: loop parameters unavailable
( -27 ) error: invalid recursion
( -28 ) error: user interrupt
( -29 ) error: compiler nesting
( -30 ) error: obsolescent feature
( -31 ) error: >BODY used on non-CREATEd definition
( -32 ) error: invalid name argument (e.g., TO xxx)
( -33 ) error: block read exception
( -34 ) error: block write exception
( -35 ) error: invalid block number
( -36 ) error: invalid file position
( -37 ) error: file I/O exception
( -38 ) error: non-existent file
( -39 ) error: unexpected end of file
( -40 ) error: invalid BASE for floating point conversion
( -41 ) error: loss of precision
( -42 ) error: floating-point divide by zero
( -43 ) error: floating-point result out of range
( -44 ) error: floating-point stack overflow
( -45 ) error: floating-point stack underflow
( -46 ) error: floating-point invalid argument
( -47 ) error: compilation word list deleted
( -48 ) error: invalid POSTPONE
( -49 ) error: search-order overflow
( -50 ) error: search-order underflow
( -51 ) error: compilation word list changed
( -52 ) error: control-flow stack overflow
( -53 ) error: exception stack overflow
( -54 ) error: floating-point underflow
( -55 ) error: floating-point unidentified fault
( -56 ) error: -56 THROW invoked without using QUIT
( -57 ) error: exception in sending or receiving a character
( -58 ) error: [IF], [ELSE], or [THEN] exception

error-table: ans-error

: .throw-code ( throw-code -- )
    dup -58 0 within if ans-error ?dup if type then exit then
    dup -255 -58 within if
	." unhandled standard Forth error code: " 0
    then
    dup -4095 -255 within if
	." unhandled system error: " 0
    then
    ?dup if
	." unhandled program-defined error: "
    then 1 .r cr
;

: abort-message! ( c-addr u -- )
    [ ' ans-error >body -2 1+ over @ - 2* cells + ] literal 2! ;
: quit-message! ( c-addr u -- )
    [ ' ans-error >body -56 1+ over @ - 2* cells + ] literal 2! ;

: report-exception ( throw-code -- )
    \ ABORT
    \   may print message
    \   clear
    \ ABORT"
    \   print message
    \   clear
    \ QUIT
    \   if have error message
    \     print message
    \     clear
    \   else
    \     may print message
    \ others
    \   if have error message
    \     print message
    \     clear
    \   else
    \     print generic message with error code
    [ -2 ans-error swap ] literal literal abort-message! ;
    [ -56 ans-error swap ] literal literal quit-message! ;
;

: QUIT s" " quit-message! -56 throw ;

: ABORT -1 throw ;

: do-abort" ( c-addr u -- ) abort-message! -2 throw ;
: ABORT" postpone if postpone s" postpone do-abort" postpone then ;
compile-special

\ FIXME:  We unconditionally display the prompt at the start of the
\ loop, but the standard says that invoking QUIT from the program
\ isn't supposed to display a prompt until *after* interpreting a
\ source line...

: state-prompt state @ if s" " else s" ok " then prompt! ;
: INTERPRET-LOOP
    postpone [ begin state-prompt! refill while interpret repeat
;

: ENTER-FORTH
    source<terminal
    begin ['] INTERPRET-LOOP catch dup while report-exception repeat
    drop
;

: QUIT
    rclear source<terminal postpone [
    begin state-prompt refill while
	['] interpret catch ?dup if report-exception then
    repeat 0 >r
;
