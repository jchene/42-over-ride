#!/bin/bash
for i in $(seq 1 $1); do
	echo "$i:	$(echo -n "a %$i\$lx" | /home/users/level02/level02 | sed -n '6p' | cut -d' ' -f2)"
done