#!/usr/bin/python

import re
import sys

#  6.1.0010 ! ................ store ................. CORE ............ 25
PATTERN = re.compile(" *([1-9][0-9.]*) ([^ ]*) [.]+ ([^.]* |)[.]+ "
		     "([A-Z]+( EXT)?) [.]+ ([1-9][0-9]*)")

for line in sys.stdin.readlines():
    m = PATTERN.match(line)
    print "%-18s %11s %-21s %3s" % (
	m.group(2), m.group(1), m.group(4), m.group(6))
