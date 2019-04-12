\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  BL                    6.1.0770 CORE
\
\  PARSE                 6.2.2008 CORE EXT
\  PARSE-NAME            6.2.2008 CORE EXT (ANS 2012)
\ ------  ------  ------  ------  ------  ------  ------  ------

32 constant BL

: PARSE ( char "ccc<char>" -- c-addr u )
    >r source swap >in @ chars + >r		( R: delim c-addr )
    >in @ - -1
    begin 1+ 2dup > while			( end cnt )
	dup 2r@ rot chars + c@			( end cnt delim c )
	over bl = if nip graphic? 0= else = then
    until
	nip dup 1+
    then >in +! r> swap r> drop
;

\ Oh my, I'm surely going to pay for this...
: WORD here dup rot parse counted, bl c, here - allot ;

\ CORE EXT (2012 standard)
: PARSE-NAME
    source swap >r >in @ 1-                     ( end idx ) ( R: c-addr )
    begin 1+ 2dup > while			( end idx )
	r@ over chars + c@ graphic?
    until then >in ! drop r> drop bl parse
;
