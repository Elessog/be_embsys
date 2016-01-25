# be_embsys
Repository for practical exam

# V1.2

When the USB key is plugged in, a script automatically finds every files located in /toCryptFolder and encrypts them using a XOR encryption method.
Now : 
  - The LED is blinking slowly (2 seconds) whenever the Armadeus is running and waiting for instructions
  - The LED is blinking a bit faster (1 second) when the Armadeus crypts the files on the key.
  - The LED blinks even faster (200 milliseconds) when the USB flash drive can be unmounted and lasts until the USB key is removed.


"blink_long.sh" must be copied in /root

"detect_key.sh" must be copied in /root

"S99app" must be copied in /etc/init.d

"cryptprog.c" must be compiled with a cross-compiler compatible with the armadeus APF28 (use autotools) and then placed into /root
