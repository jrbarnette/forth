\ Copyright 2019, by J. Richard Barnette, All Rights Reserved.
\ Definitions of flags bits for names in dictionary entries.

\ FIXME - This code is duplicated with code under forth/direct.
\ We need two slightly different versions because
\   * This code executes under the meta-interpreter, where "BASE"
\     and "HEX" can't do what we want.
\   * The other code is interpreted by the bootstrap interpreter,
\     where the number syntax "$80" doesn't work.

$40 constant NF-COMPILE-ONLY
$80 constant NF-IMMEDIATE
$c0 constant NF-COMPILE-SPECIAL
$1f constant NF-LENGTH
$e0 constant NF-FLAGS
