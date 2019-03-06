CC=arm-linux-gnueabihf-gcc
STRIP=arm-linux-gnueabihf-strip

all:
	$(CC) -o aes-release aes.c -I.
	$(CC) -o sha-release sha.c -I.
	$(STRIP) -s aes-release sha-release 
