\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Definition of INTERPRET, and supporting code
\
\ This code is responsible for parsing and recognizing tokens according
\ to the rules in section 3.4 "The Forth text interpreter":
\  a) A token is parsed, according to the rules in 3.4.1.
\  b) If the token is a name in the dictionary per section 3.4.2,
\     processing calls INTERPRET-NAME.
\  c) If the token can be converted to a number per section 3.4.1.3,
\     processing calls INTERPRET-NUMBER.
\  d) Otherwise, processing calls INTERPRET-UNKNOWN.
\
\ The implementations of INTERPRET-NAME, INTERPRET-NUMBER, and
\ INTERPRET-UNKNOWN are external, and this code may compile in
\ different version depending on the vocabulary context.
\ Specifically, meta-interpretation provides implementations that
\ are different from the prescriptions of the Forth standard.


: INTERPRET-TOKEN
    2dup lookup ?dup if >r 2drop r> interpret-name exit then
    2dup number-token? if >r 2drop r> interpret-number exit then
    interpret-unknown if exit then
    -13 .error
;

: INTERPRET begin parse-name dup while interpret-token repeat 2drop ;
