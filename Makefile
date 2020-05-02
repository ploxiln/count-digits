CFLAGS = -Wall -Wextra -O1 -g

sortdig: sortdig.c
	cc ${CFLAGS} -o $@ $^

run-c: sortdig
	./sortdig

run-js:
	node sortdig.js

.PHONY: run-c run-js
