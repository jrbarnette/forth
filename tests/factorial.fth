: factorial
    dup 3 U< if dup 0= if drop 1 then 0 exit then
    0 over 2 do swap i um* rot + loop ;
