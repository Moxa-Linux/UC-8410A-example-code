/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.

    com.c

    Routines to test RS232/RS422.

    2011-06-25	Lock Lin
		new release
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "mserial_port.h"

#define VERSION "1.0"
#define loop_count 100 // test 100 times
#define MAX_BYTE 512


static void 
usage()
{
	printf( "\n************ UART test utility %s************\n"
                "Usage: com [options]\n"
                "\t\t-b 115200 -m 1,2,3,4...  ttyM* RS232 port.\n"
                "\n" ,VERSION);
}


/*  Split optarg and open serial port.
    Inputs:
        <optarg> The input port which will be opened
	<fd>     array of file descriptors
	<flag>
                 0: ttyM*
                 1: ttyS*
                 2: console port  
	<port>   Record ports mapping with fd
    Outputs:
        array of file descriptors
    Returns:
	count of file descriptors
*/

int
open_serial(char *optarg,int fd[32],int flag,int port[32])
{
	char *sub_opt=NULL;
	int count=0;
	/* Console port */
	if(flag==2){
		fd[count] = mxsp_open(atoi(sub_opt),flag);
                port[count]=atoi(sub_opt);
                if(fd[count]==0){
                        printf("fail to open console port \n");
                        exit(-1);
                }
		count+=1;
	}
	else{
		sub_opt = strtok( optarg, ",");
		while( sub_opt != NULL ) {
			fd[count] = mxsp_open(atoi(sub_opt),flag);		
			port[count]=atoi(sub_opt);
			if(fd[count]==0){
				printf("fail to open COM port %d\n", (atoi(sub_opt)));
        	       	 	exit(-1);
			}
			count+=1;
			sub_opt = strtok( NULL, "," );
		}
	}
	return count;

}

/*  Set serial parameter to each serial port.
    Inputs:
        <fd> file descriptors
        <serial_count> number of opened serial port
	<baudrate> Specific baudrate
  	      
*/
void 
serial_setting(int fd[32],int serial_count,int baudrate)
{
	int i=0;
	for(i=0;i<serial_count;i++){
		if(mxsp_set_baudrate(fd[i], baudrate) < 0)	
			exit(-1);
		if(mxsp_set_parity(fd[i], 0) < 0)
			exit(-1);
		if(mxsp_set_databits(fd[i], 8) < 0)
                        exit(-1);
		if(mxsp_set_stopbits(fd[i], 1) < 0)
                        exit(-1);
	}
}

/*  generate random data and assign to buffer
    Inputs:
	<snd_data> The buffer used to send data
    Outputs:
	Array with random data
    Returns:
	snd_data
*/
void
random_data(char *snd_data)
{
	int value,count=0;
  	/* Initialize buffer */
	memset(snd_data,'\0',MAX_BYTE);

	/* Use random function to generate random data */
        for(count=0;count<MAX_BYTE;count++){
                srand( (unsigned)time( NULL ) );
                value=(rand()+count)%255;
		snd_data[count]=value;
        }
}

/*  compare data between snd_data and rcv_data
    Inputs:
        <snd_data> The buffer used to send data
	<rcv_data> The buffer used to receive data
	<serial_count> number of opened serial port
	<port> Which port is processing
    Outputs:
        Two buffers are the same or not
    Returns:
	-1 on different
	0  on the same
*/
int
compare(char *snd_data,char *rcv_data,int serial_count,int port)
{
	int i=0;
	for (i=0; i<MAX_BYTE; i++) {
		if(snd_data[i]!=rcv_data[i]){
			printf("Error in COM %d\n",port);
			exit(-1);
		}
	}
	return 0;
}

/*
        Close fd and return value
        Inputs:
                <fd> serial port
		<serial_count> number of opened serial port
*/
void
close_serial(int fd[32],int serial_count)
{
	int count=0;
        /* Close fd */
	for(count=0;count<serial_count;count++){
        	if(fd[count] > 0){
              		mxsp_close(fd[count]);//fd
       		}
	}
	exit(0);
}



/*  declare and initialize COM variables. generate random data, send and receive it to exam.
    Inputs:
        <optarg> The input port which will be opened
	<baudrate> Specific baudrate
        <flag>
                 0: ttyM*
                 1: ttyS*
                 2: console port
    Outputs:
	Pass this test or not.
*/

void
com_test(char *optarg,int baudrate,int flag)
{
	static int count=0;
	int port[32]={0};
	int serial_num,rt_val,i=0;
	char snd_data[MAX_BYTE],rcv_data[MAX_BYTE];
	//memset(snd_data,'\0',MAX_BYTE);
	//memset(rcv_data,'\0',MAX_BYTE);
	unsigned int fd[32];
	float wait_time=0;
	float send_byte=MAX_BYTE;
	serial_num=open_serial(optarg,fd,flag,port);

       	serial_setting(fd,serial_num,baudrate);

        random_data(snd_data);
	
	for(count=0;count<loop_count;count++){
		rt_val=0;
		/* Send data */
           	for(i=0;i<serial_num;i++){
               		rt_val = mxsp_write(fd[i], snd_data, MAX_BYTE, NULL);
               	        if(rt_val < 0 ){
				printf("Failed to write %d\n",port[i]);
                                exit(-1);
			}
                }
                /* wait time depends on baud rate */
		wait_time=(send_byte/(baudrate/8)*1500);
		usleep(wait_time*1000);


		/* Rcv data */
		for(i=0;i<serial_num;i++){
                	rt_val = 0;
                        memset(rcv_data,'\0',MAX_BYTE);
                        while(!rt_val){
                        	rt_val=mxsp_read(fd[i], rcv_data, MAX_BYTE, NULL);
                         	if (rt_val < 0){
					printf("Failed to read %d\n",port[i]);
	                        	exit(-1);
				}
                        }
			/* Compare data */
			compare(snd_data,rcv_data,serial_num,port[i]);
                }
	}
		/* Pass the test */
		printf("Pass COM test with data %d, baud_rate %d in %d times\n",MAX_BYTE,baudrate,count);
		close_serial(fd,serial_num);
}


int
main (int argc, char **argv)
{
	int opt_val=0,flag=0;
	int baudrate=115200; // default baud rate 
	if (argc < 3){
	        usage();
		exit(-1);
	}
	while ( (opt_val=getopt(argc, argv, "b:c:m:s:")) != -1 ){
		switch(opt_val) {
			case 'b':
				baudrate = atoi(optarg );
                                break;
			case 'm':
				com_test(optarg,baudrate,0);
				break;				
			case 's': 
                                com_test(optarg,baudrate,1);
                                break;
			case 'c':
				com_test(optarg,baudrate,2);
                                break;
			default:
				usage();
				break;
		}
	
	}
	return 0;
}
