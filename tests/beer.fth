\ ANS Forth, well-factored

' min @
here swap dup , ] dup . ." bottles" exit [
here swap dup , ] ." 1 bottle" exit [
here swap ,     ] ." no more bottles" exit [
create bottles , , ,

: .bottles  dup 2 min cells bottles + @ execute ;
: .beer     .bottles ."  of beer" ;
: .wall     .beer ."  on the wall" ;
: .take     ." Take one down and pass it around" ;
: .verse    .wall ." , " .beer ." ." cr
         1- .take ." , " .wall ." ." cr ;
: verses   dup begin .verse cr dup 0= until
	   .wall ." , " .beer ." ." cr drop
	   ." Go to the store and buy some more, " .wall cr drop ;

99 verses
