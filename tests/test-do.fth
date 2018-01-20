: ." postpone s" postpone type ; immediate

: test1 ." loop two times" cr 2 0 do ." loop" cr loop ;
: test2 ." loop by 2" cr 3 0 do i . cr 2 +loop ;
: test3 ." down by 1" cr 0 1 do i . cr -1 +loop ;
: test4 ." down by 2" cr 0 3 do i . cr -2 +loop ;
: test5 ." print odd I" cr 4 0 do i 1 and if i . cr then loop ;

test1 cr
test2 cr
test3 cr
test4 cr
test5 cr
