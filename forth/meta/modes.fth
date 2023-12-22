: META] meta-mode
    0 source-id! ( postpone [ )
    begin prompt-refill while
	['] interpret catch handle-exception
    repeat ;
