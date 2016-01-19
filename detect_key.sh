#!/bin/sh

export file_name=sda                  

cd /dev

while ! [ -e $file_name ]
do
sleep 1
done

mount /dev/sda1 /mnt/keyUSB
echo "detection of an USB key!"

sleep 1

export led21=/sys/class/gpio

echo 21 > $led21/export
echo out > $led21/gpio21/direction
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

umount /mnt/keyUSB

 
while [ -e "$file_name" ]
do
sleep 1
done


#relaunch the detection code
#/root/detec_key.sh &

exit 0
