\  Copyright 2023, by J. Richard Barnette. All Rights Reserved.

wordlist constant C-CODE-ESCAPES

: forth-escape
    parse-name dup if
	c-code-escapes wid-lookup dup if name>xt then
    else
	nip
    then dup 0= if 0 >in ! then
;

: TRANSCRIBE>
    begin refill while
	forth-escape ?dup if
	    execute source >in @ tuck - >r chars + r> evaluate
	    if exit then
	else
	    source nip begin
		[char] ` parse type
		[char] ` parse evaluate
	    >in @ over < 0= until drop cr
	then
    repeat
;

get-current c-code-escapes set-current
: \ false source dup >in @ = if type cr else >in ! drop then ;
: FORTH> source >in @ = nip ;
set-current

: def-escape
    >in @ ' swap >in ! create , does> false swap @ execute
;
: TRANSCRIBE-ESCAPE:
    get-current c-code-escapes set-current def-escape set-current
;
