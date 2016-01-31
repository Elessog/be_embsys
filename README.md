# be_embsys
Repository for pratical exam

# V1.5

**UPDATE FROM 1.4**:

Add syslog messages from the __cryptprog__ program.

#How to use

If its the first time you install via the key:

On the Armadeus:

        mkdir -p /mnt/keyUSB
        mount /dev/sda1 /mnt/keyUSB
        source /mnt/keyUSB/autoupdate.sh


The file autoupdate.sh compares the version on the USB key and the one on the Amadeus card. If both versions are equal, nothing is done. Otherwise the file autoupdate.sh update the version currently installed on the armadeus card.

To compile the cryptprog programm (in console you must be in be_embsys):

        cd autotools
        export PATH=$PATH:path_to_folder_of_crosscompiler
        ./autogen.sh
        ./configure --host=arm-linux
        make
        make update

Then copy the folder **install_folder** and file **autoupdate.sh** to the root of the usb key.

Your code should be installed.


When the USB key is plugged in, a script automatically finds every files located in /toCryptFolder and encrypts them using a XOR encryption method.

Now :

 - The LED is blinking slowly (2 seconds) whenever the Armadeus is running and waiting for instructions
 - The LED is blinking a bit faster (1 second)
 - The LED blinks even faster (200 milliseconds) when the USB flash drive can be unmounted and lasts until the USB key is removed. 


Have Fun !


