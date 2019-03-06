# History:
# Date		Author			Comment
# 05-21-2013	Jared Wu.		Configure UART to work in RS-232/422/485 module
#
# Compile:
#	make user/setinterface_only
CC=arm-linux-gnueabihf-gcc
EXEC=setinterface

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
