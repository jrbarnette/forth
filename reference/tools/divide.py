# Knuth Algorithm D (division) from "Seminumerical Algorithms"

def digitlist(n, b):
    d = []
    while n > 0:
        d.insert(0, n % b)
        n /= b
    return d


def divide(u, v, b):
    u = [0] + digitlist(u, b)
    v = [0] + digitlist(v, b)
    n = len(v) - 1
    m = len(u) - n - 1
    q = [0] * (m + 1)
    assert n > 1

    # step D1 [Normalize]
    d = b / (v[1] + 1)
    i = m + n
    c = 0
    while i > 0:
	p = d * u[i] + c
	u[i] = p % b
	c = p / b
	i -= 1
    u[0] = c
    i = n
    c = 0
    while i > 0:
	p = d * v[i] + c
	v[i] = p % b
	c = p / b
	i -= 1
    assert c == 0

    # step D2 [Initialize]
    j = 0
    # step D7 [Loop on j]
    while j <= m:
	# step D3 [Calculate estimated q]
	qpart = b * u[j] + u[j+1]
	if u[j] >= v[1]:
	    qhat = b - 1
	else:
	    qhat = qpart / v[1]
	while qhat * v[2] > b * (qpart - qhat * v[1]) + u[j+2]:
	    qhat -= 1

	# step D4 [Multiply and subtract]
	i = n
	c = 0
	while i > 0:
	    p = u[j+i] - qhat * v[i] + c
	    u[j+i] = p % b
	    c = p / b
	    i -= 1
        p = u[j] + c
        u[j] = p % b
        c = p / b

	# step D5 [Test remainder]
	if c < 0:
	    # step D6 [Add back]
	    qhat -= 1
	    i = n
	    c = 0
	    while i > 0:
		p = u[j+i] + v[i] + c
		u[j+i] = p % b
		c = p / b
                i -= 1
	q[j] = qhat
        j += 1

    # step D8 [Unnormalize]
    i = 1
    r = 0
    while i <= n:
        p = b * r + u[m+i]
        u[m+i] = p / d
        r = p % d
        i += 1
    assert r == 0

    return q, u[m+1 :]
