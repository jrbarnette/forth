\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

\ THROW requires that we restore the input source back to its state prior
\ to calling CATCH.  We want to print error information about the source
\ line that caused an error before that happens.  Hence this definition.

\ Ideally, we'd push this printing up the change to someplace around the
\ CATCH, so that we didn't force printing if a handler wants to override
\ default exception behavior, but that would require more work. :-(

: .ERROR ( error-code -- )
    source over swap type cr
    >in-start @ chars over + swap ?do
	i c@ 9 = if 9 emit else space then
    [ 1 chars ] literal +loop
    >in @ >in-start @ ?do '^' emit loop cr throw ;
