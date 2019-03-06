CC=arm-linux-gnueabihf-gcc
STRIP=arm-linux-gnueabihf-strip

all:
	$(CC) -o watchdog-simple-release watchdog-simple.c
	$(CC) -o watchdog-test-release watchdog-test.c
	$(STRIP) -s watchdog-test-release watchdog-simple-release
