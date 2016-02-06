#!/bin/sh

install_folder=/mnt/keyUSB/install_folder

VERSIONUSB=29

if [ -z "$VERSION_CRYPT" ]
then
    echo "No version found"
    cp $install_folder/blink_long.sh /root/
    cp $install_folder/detect_key.sh /root/
    cp $install_folder/S99app /etc/init.d/
    cp $install_folder/cryptprog /root/
    cp $install_folder/exportscript.sh /root/
    cp $install_folder/cryptkey.conf /etc/
    source /root/exportscript.sh
    echo "Version $VERSION_CRYPT installed"
else
    echo "Old Version : $VERSION_CRYPT"
    echo "USB Version : $VERSIONUSB"
    if ! [ $VERSION_CRYPT = $VERSIONUSB ]
    then
        echo "Updating Version..."
        rm /root/blink_long.sh 
        rm /root/detect_key.sh 
        rm /etc/init.d/S99app
        rm /root/cryptprog
	rm /root/exportscript.sh
        rm /root/cryptkey.conf
        cp $install_folder/blink_long.sh /root/
        cp $install_folder/detect_key.sh /root/
        cp $install_folder/S99app /etc/init.d/
        cp $install_folder/cryptprog /root/
	cp $install_folder/exportscript.sh /root/
        cp $install_folder/cryptkey.conf /etc/
	source /root/exportscript.sh
        echo "Version $VERSION_CRYPT installed"
    fi
fi

sync

exit 0
