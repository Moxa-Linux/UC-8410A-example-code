# History:
# Date		Author			Comment
# 2016-11-08	Ethan SH Lee		This is an example program to demonstrate how to turn on/off buzzer.
#
# Compile:
#	make
CC=arm-linux-gnueabihf-gcc
EXEC=buzzer_example

CFLAGS += -I.


all: $(EXEC).c


$(EXEC).c:	$(EXEC).o
	$(CC) $(EXEC).o -o $(EXEC)

$(EXEC).o:
	$(CC) $(CFLAGS) -c $(EXEC).c
	$(STRIP) -s $(EXEC)
romfs:
	$(ROMFSINST) $(EXEC) /bin/$(EXEC)
	$(STRIP) -s $(ROMFSDIR)/bin/$(EXEC)

clean distclean:
	-rm -fr $(EXEC) $(EXEC).o
