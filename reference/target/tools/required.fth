\ Copyright 2018, by J. Richard Barnette, All Rights Reserved.

: .S ( -- ) depth begin dup while dup pick u. 1- repeat drop cr ;
: ? ( a-addr -- ) @ . ;
: .WORDLIST ( wid -- ) begin @ dup while dup name>id type space repeat drop ;
: WORDS ( -- ) get-order swap .wordlist 1- 0 do drop loop ;
