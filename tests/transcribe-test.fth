: escape-test ." this text is printed by an escape" cr ;
TRANSCRIBE-ESCAPE: escape-test

TRANSCRIBE>
First line, unaltered
\ This is a comment; no output
The line that follows ends with a \
    \
Testing embedded forth text: `.( ok )` -- should have been "ok"
Next line should be blank:
`
Embedded expression (bis): `.( no terminating backtick )
FORTH> .( FORTH> with code that follows works ) cr
escape-test
    escape-test .( Executing Forth code after escape ) cr
FORTH>
.( Back to Forth -- test completed ) cr
