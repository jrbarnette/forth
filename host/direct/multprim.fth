\ Copyright 2017, by J. Richard Barnette. All Rights Reserved.

\  multprim.fth - Standard Forth words for single and double cell
\    multiply and divide operations.

\ ------  ------  ------  ------  ------  ------  ------  ------
\  *                     6.1.0090 CORE                   26
\  */                    6.1.0100 CORE                   26
\  */MOD                 6.1.0110 CORE                   26
\  /                     6.1.0230 CORE                   28
\  /MOD                  6.1.0240 CORE                   28
\  FM/MOD                6.1.1561 CORE                   39
\  M*                    6.1.1810 CORE                   41
\  MOD                   6.1.1890 CORE                   42
\  SM/REM                6.1.2214 CORE                   45
\  UM*                   6.1.2360 CORE                   46
\  UM/MOD                6.1.2370 CORE                   46
\ ------  ------  ------  ------  ------  ------  ------  ------

\ CORE
prim: *         x_star
\     */
\     */MOD
prim: /         x_slash
prim: /MOD      x_slash_mod
\     FM/MOD
prim: M*        x_m_star
prim: MOD       x_mod
\     SM/REM
prim: UM*       x_u_m_star
prim: UM/MOD    x_u_m_slash_mod
