\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ Flow of control for constructs with internal branches to a common
\ exit point, including DO, ?DO, and CASE.
here 0 ,
: chain-swap ( new-chain -- old-chain )
    [ over ] literal @ swap [ swap ] literal ! ;
: start-chain ( -- saved-chain ) 0 chain-swap ;
: mark>chain ( orig -- ) dup chain-swap swap ! ;
: resolve-chain ( saved-chain -- )
    chain-swap begin dup while dup @ swap postpone then repeat drop ;
