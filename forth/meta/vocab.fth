\  Copyright 2020, by J. Richard Barnette. All Rights Reserved.

vocabulary HOST

: HOST-MODE only FORTH also HOST ;
HOST-MODE definitions

vocabulary META-HOST
vocabulary META-TARGET
vocabulary META-DEFINERS
vocabulary META-SPECIAL

: META-HOST-MODE     HOST-MODE also META-HOST ;
: META-DEFINITIONS   also META-TARGET definitions previous ;
: META-TARGET-MODE   only FORTH also META-DEFINERS META-DEFINITIONS ;
: META-COMPILE-MODE  META-HOST-MODE also META-SPECIAL META-DEFINITIONS ;

' META-TARGET  >body constant META-TARGET-WORDLIST
' META-SPECIAL >body constant META-SPECIAL-WORDLIST
