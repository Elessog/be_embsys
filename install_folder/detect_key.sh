#!/bin/sh

export file_name=/dev/sda
export this_file_path=/root
export key_path=/mnt/keyUSB
export crypt_key=/etc/cryptkey.conf

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
sleep 2
echo "USB key detected"

if [ -e $key_path/autoupdate.sh ]
    then
    sh $key_path/autoupdate.sh
fi



touch $this_file_path/tempFile.xyz
sh $this_file_path/blink_long.sh &

#execution of cryptography
if [ -d $key_path/toCryptFolder ]
then
	sh -c '$this_file_path/cryptprog $key_path/toCryptFolder/ `cat $crypt_key`'
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
    usleep $time
    echo 1 > $led21/gpio21/value
    usleep $time
    echo 0 > $led21/gpio21/value
done

#relaunch the detection code
sh $this_file_path/detect_key.sh &

exit 0
