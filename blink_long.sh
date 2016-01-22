#!/bin/sh

export this_file_path=/home/pi/be_embsys

while [ -e $this_file_path/tempFile.xyz ];
do
sleep 1
echo 1 > /sys/class/gpio/gpio21/value
sleep 1
echo 0 > /sys/class/gpio/gpio21/value

done

exit 0
