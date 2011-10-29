: nop ;
variable n

: test0 [ niter ] literal begin 1 - dup 0= until ;

: test1 [ niter ] literal begin dup while 1 - repeat ;

: test2 [ niter ] literal begin nop 1 - dup 0= until ;

: test3 [ niter ] literal begin dup drop 1 - dup 0= until ;

: test4 [ niter ] literal begin dup swap 1 - 0= until ;

: test5 [ niter ] literal n ! begin n @ 1 - n ! n @ 0- until ;
