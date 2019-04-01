\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ ------  ------  ------  ------  ------  ------  ------  ------
\  (                     6.1.0080 CORE                   26
\ ------  ------  ------  ------  ------  ------  ------  ------

: ( [char] ) parse drop drop ; immediate


\ ------  ------  ------  ------  ------  ------  ------  ------
\  .(                    6.2.0200 CORE EXT               49
\  \                     6.2.2535 CORE EXT               58
\ ------  ------  ------  ------  ------  ------  ------  ------

\ XXX not yet ready to implement type in `target`
\ : .( [char] ) parse type ; immediate
: \ source >in ! drop ; immediate
