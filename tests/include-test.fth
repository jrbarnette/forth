: .depth ." depth: " depth 2 u.r cr ;
: test-included  s" include.fth" included ;
: open-include-file s" include.fth" r/o open-file ;
: test-include-file s" include.fth" include-file ;
: test-no-such-include s" nosuch.fth" included ;

.depth
test-included
.depth
\ open-include-file drop included
\ test-include-file
