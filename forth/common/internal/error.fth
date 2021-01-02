\ Copyright 2019, by J. Richard Barnette. All Rights Reserved.

: .ERROR ( error-code -- )
    source over swap type cr
    >in-start @ chars over + swap ?do
	i c@ 9 = if 9 emit else space then
    [ 1 chars ] literal +loop
    >in @ >in-start @ ?do '^' emit loop cr throw ;
