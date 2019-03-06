/******************************************************************************
File Name : mx_dio_control.c

Description :
	This utility can control DO HIGH/LOW signal, and get DI/DO HIGH/LOW signal.

Usage :
	1.Get signal:
		./mx_dio_control -g [val] -n [slot]
			val=0 -> DO, val=1 -> DI
			slot -> slot number
	2.Set signal
		./mx_dio_control -s [val] -n [slot]
			val=0 -> LOW, val=1 -> HIGH
			slot -> slot number

History :
	Versoin		Author		Date		Comment
	1.0		HarryYJ.Jhou	07-22-2015	First Version
*******************************************************************************/

#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	"moxadevice.h"
#include	<fcntl.h>

#define GET 0
#define SET 1
#define DO 0
#define DI 1
#define GET 0
#define SET 1

static char	*DataString[2]={"Low  ", "High "};

usage(){

	
	printf("Moxa DIO get/set utility.\n");
	printf("Usage: mx_dio_control [-g|-s] -n #slot\n");
	printf("\t-g [1|0] Get DI/DO value\n");
	printf("\t         0:DO\n");
	printf("\t         1:DI\n");
	printf("\t-s [1|0] Set DO value\n");
	printf("\t         0:LOW\n");
	printf("\t         1:HIGH\n");
	printf("\t-n [#num] port number\n");
	printf("\t         num: port number DI:1~%d DO:1~%d\n", MAX_DIN_PORT, MAX_DOUT_PORT);
	printf("\n");
	printf("Example:\n");
	printf("\tGet port 2 DI value\n");
	printf("\t         mx_dio_control -g 1 -n 2\n");
	printf("\tSet port 2 DO value to LOW\n");
	printf("\t         mx_dio_control -s 0 -n 2\n");
	exit(-1);
}

int main(int argc, char * argv[])
{
	int 	i, j, state, retval;
	int	c, val, gs=-1, slot=-1; 
	// val==0 --> DO val==1 --> DI, when gs=GET
	// val==0 --> low val==1 --> high, when gs=SET
	// gs(get/set)

	char optstring[] = "g:s:n:";

	while ((c = getopt(argc, argv, optstring)) != -1)
		switch (c) {
		case 'g':
			if ( gs == -1 ){
				gs = GET;
				val = atoi(optarg);
			}else{
				usage();
			}
			break;
		case 's':
			if ( gs == -1 ){
				gs = SET;
				val = atoi(optarg);
			}else{
				usage();
			}
			break;
		case 'n':
			slot = atoi(optarg);
			break;
		default:
			usage();
			return -1;
		}
	//printf("MAX_DIN_PORT=%d, MAX_DOUT_PORT=%d\n", MAX_DIN_PORT ,MAX_DOUT_PORT);
	//printf("gs=%d, val=%d, slot=%d\n", gs, val, slot);

	
	if ( !( ((gs == GET && slot>=1 && ((val == DI && slot<=MAX_DIN_PORT) || (val == DO && slot<=MAX_DOUT_PORT))))
		|| (gs == SET && slot>=1 && slot<=MAX_DOUT_PORT) ) )
		usage();

	if ( gs == GET ){
		if ( val == DI ){
			get_din_state(slot-1, &state);
			printf("DIN(%d):%s\n", slot, DataString[state]);
		}else if ( val == DO ){
			get_dout_state(slot-1, &state);
			printf("DOUT(%d):%s\n", slot, DataString[state]);
		}
	}else if ( gs == SET ){
			retval=set_dout_state(slot-1, val);
			if ( retval ){
				printf("SET DO error\n");
				return -1;
			}
	}
	return 0;
}
