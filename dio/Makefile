FNAME=mx_dio_control
CC=arm-linux-gnueabihf-gcc

release:
	$(CC) -o $(FNAME) $(FNAME).c dio.c -lpthread
	arm-linux-gnueabihf-strip $(FNAME)

debug:
	$(CC) -DDEBUG -o $(FNAME)-dbg $(FNAME).cxx dio.cxx -lpthread

clean:
	/bin/rm -f $(FNAME) $(FNAME)-dbg *.o
