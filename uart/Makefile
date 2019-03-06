CC = $(PREFIX)gcc

all: mserial_port_lx.o
	$(CC) $(LDFLAGS) -o com com.c mserial_port_lx.o $(LIBS)

mserial_port_lx.o: mserial_port_lx.c mserial_port.h
	$(CC) $(LDFLAGS) -c mserial_port_lx.c $(LIBS) 

 
clean:

	rm -rf *.gdb com  *.o
