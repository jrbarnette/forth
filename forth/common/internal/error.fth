\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ THROW requires that we restore the input source back to its state
\ prior to returning to our CATCH.  That involves resetting the values
\ of >IN and >IN-START.  We want to print error information about where
\ in the source line the error occurred before THROW clobbers those
\ variables.  Hence this definition.

\ Ideally, we'd push this printing up the chain to someplace around the
\ CATCH, so that we didn't force printing if a handler wants to override
\ default exception behavior, but that would require more work. :-(

: .ERROR ( error-code -- )
    source over swap type cr
    >in-start @ chars over + swap ?do
	i c@ 9 = if 9 emit else space then
    [ 1 chars ] literal +loop
    >in @ >in-start @ ?do '^' emit loop cr throw ;
