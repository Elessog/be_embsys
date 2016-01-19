#!/bin/sh

export file_name=sda                  

export led21=/sys/class/gpio

echo 21 > $led21/export
echo out > $led21/gpio21/direction


cd /dev

while ! [ -e $file_name ]
do
sleep 1
done

mount /dev/sda1 /mnt/keyUSB
echo "detection of an USB key"

umount /mnt/keyUSB

sh /root/blink_long.sh

sleep 1

echo 1 > $led21/gpio21/value

sleep 1
echo 0 > $led21/gpio21/value


sleep 1
echo 1 > $led21/gpio21/value

sleep 1           
echo 0 > $led21/gpio21/value


sleep 1
echo 1 > $led21/gpio21/value

sleep 1
echo 0 > $led21/gpio21/value


#relaunch the detection code
sh /root/detec_key.sh &

exit 0
