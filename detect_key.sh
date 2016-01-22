#!/bin/sh

export file_name=/dev/sda
export this_file_path=/home/pi/be_embsys
export key_path=/media/pi/AEGIS

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

mount /dev/sda1 $key_path

echo "detection of an USB key"

touch $this_file_path/tempFile.xyz
sh $this_file_path/blink_long.sh &

#execution of cryptographie
if [ -d $key_path/toCryptFolder ]
then
	find $key_path/toCryptFolder -type f | xargs -I '{}' sh -c '$this_file_path/a.out {} {}crypt' \;
fi

rm $this_file_path/tempFile.xyz

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

umount $key_path

while [ -e $file_name ]
do
sleep 1
done

#relaunch the detection code
sh $this_file_path/detect_key.sh &

exit 0
