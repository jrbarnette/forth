\ Copyright 2011, by J. Richard Barnette

base @ hex

: ." postpone s" postpone type ; immediate
: abort" postpone if postpone ." postpone abort postpone then ; immediate
20 constant bl
: within ( test low high -- flag) over - >r - r> u< ;

create hex-digits
  char 0 c, char 1 c, char 2 c, char 3 c,
  char 4 c, char 5 c, char 6 c, char 7 c,
  char 8 c, char 9 c, char a c, char b c,
  char c c, char d c, char e c, char f c,

: .x 0f and hex-digits + c@ emit ;
variable db-col 0 db-col !

: db, db-col @ if bl emit then
      dup 4 rshift .x .x
      db-col @ 1 + dup 10 = if cr drop 0 then db-col !
;
: dw, dup db, 8 rshift db, ;
: dd, dup dw, 10 rshift dw, ;
: newline cr 0 db-col ! ;

base !
