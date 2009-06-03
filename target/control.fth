\ +LOOP                 6.1.0140 CORE                   27
\ BEGIN                 6.1.0760 CORE                   34
\ DO                    6.1.1240 CORE                   37
\ ELSE                  6.1.1310 CORE                   38
\ I                     6.1.1680 CORE                   40
\ IF                    6.1.1700 CORE                   40
\ J                     6.1.1730 CORE                   41
\ LEAVE                 6.1.1760 CORE                   41
\ LOOP                  6.1.1800 CORE                   42
\ REPEAT                6.1.2140 CORE                   45
\ THEN                  6.1.2270 CORE                   47
\ UNLOOP                6.1.2380 CORE                   48
\ UNTIL                 6.1.2390 CORE                   48
\ WHILE                 6.1.2430 CORE                   49
\ ?DO                   6.2.0620 CORE EXT               53
\ AGAIN                 6.2.0700 CORE EXT               53
\ CASE                  6.2.0873 CORE EXT               54
\ ENDCASE               6.2.1342 CORE EXT               54
\ ENDOF                 6.2.1343 CORE EXT               55
\ OF                    6.2.1950 CORE EXT               56

\ BEGIN		( C: -- dest )
\ THEN		( C: orig -- )
\ IF		( C: -- orig )
\ ELSE		( C: orig1 -- orig2 )
\ WHILE		( C: dest -- orig dest )
\ REPEAT	( C: orig dest -- )
\ UNTIL		( C: dest -- )
\ AGAIN		( C: dest -- )

: REPEAT POSTPONE AGAIN POSTPONE THEN ;

\     BEGIN a AGAIN
\ L0:
\ 	a
\ 	goto L0
\ 
\     BEGIN a UNTIL
\ L0:
\ 	a
\ 	IF-FALSE goto L0
\ 
\     BEGIN a WHILE b REPEAT
\ L0:
\ 	a
\ 	IF-FALSE goto L1
\ 	b
\ 	goto L0
\ L1:
\ 
\     BEGIN a WHILE b UNTIL c THEN
\ L0:
\ 	a
\ 	IF-FALSE goto L1
\ 	b
\ 	IF goto L0
\ 	c
\ L1:
\ 
\     BEGIN a WHILE b WHILE c REPEAT d THEN
\ L0:
\ 	a
\ 	IF-FALSE goto L1
\ 	b
\ 	IF-FALSE goto L2
\ 	c
\ 	goto L0
\ L2:
\ 	d
\ L1:
\ 
\ 
\ DO - Run-time
\     ( limit index -- )
\ LOOP - Run-time
\     index++
