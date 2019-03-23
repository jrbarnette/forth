\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Internal definitions relating to text interpretation.

only FORTH also TARGET definitions DIRECT

\ N.B. "," and "allot" here come from the DIRECT vocabulary
: CONSTANT s" do_constant" addname direct-emit , ;
: VARIABLE s" do_variable" addname
    direct-emit { s" do_literal" .exec }{ s" CELL_SIZE" .cell } allot ;


