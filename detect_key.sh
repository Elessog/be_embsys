#!/bin/sh

export file_name=/dev/sda
export this_file_path=/home/pi/be_embsys

export led21=/sys/class/gpio

echo 21 > $led21/export
echo out > $led21/gpio21/direction



while ! [ -e $file_name ]
do
	echo 1 > $led21/gpio21/value
	sleep 2
	echo 0 > $led21/gpio21/value
	sleep 2
done

mount /dev/sda1 /mnt/keyUSB

echo "detection of an USB key"


#sh /root/blink_long.sh &

n=1
while [ $n -le 5 ]
do
	echo 1 > $led21/gpio21/value
	sleep 1
	echo 0 > $led21/gpio21/value
	sleep 1
	n=$(( n+1 ))	 # increments $n
done

echo 1 > $led21/gpio21/value

export time=200000

usleep $time
echo 0 > $led21/gpio21/value


usleep $time
echo 1 > $led21/gpio21/value

usleep $time
echo 0 > $led21/gpio21/value


usleep $time
echo 1 > $led21/gpio21/value

usleep $time
echo 0 > $led21/gpio21/value

umount /mnt/keyUSB

while [ -e $file_name ]
do
sleep 1
done

#relaunch the detection code
sh $this_file_path/detect_key.sh &

exit 0
