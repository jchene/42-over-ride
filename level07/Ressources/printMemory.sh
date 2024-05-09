#!/bin/bash
rm -f /tmp/input07
touch /tmp/input07
chmod 777 /tmp/input07
for i in $(seq 0 250)
do
        echo "read" >> /tmp/input07
        echo $i >> /tmp/input07
done
echo "quit" >> /tmp/input07

data=4158936339
cat /tmp/input07 | /home/users/level07/level07 > /tmp/output07
cat /tmp/output07 | grep "$data"