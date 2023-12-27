\ Copyright 2023, by J. Richard Barnette. All Rights Reserved.

\ Branching building blocks - non-standard.
: >BRANCH ( -- orig ) postpone branch >mark ; compile-only
: >?BRANCH ( -- orig ) postpone ?branch >mark ; compile-only
: <BRANCH ( dest -- ) postpone branch <resolve ; compile-only
: <?BRANCH ( dest -- ) postpone ?branch <resolve ; compile-only
