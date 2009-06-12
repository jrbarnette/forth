: interpret
    BEGIN
    parse-word ?DUP WHILE		( name len )
	2DUP sfind ?DUP IF
	    1 <> STATE @ AND IF EXECUTE ELSE COMPILE, THEN
	ELSE
	    COUNT >NUMBER
	    -13 error
	THEN
    REPEAT
;
