#!/bin/sh

while [ -e /dev/sda ];
do
sleep 3
echo 1 > /sys/class/gpio/gpio21/value
sleep 3
echo 0 > /sys/class/gpio/gpio21/value

done

exit 0
