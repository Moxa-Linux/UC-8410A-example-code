/*  Copyright (C) MOXA Inc. All rights reserved.

    This software is distributed under the terms of the
    MOXA License.  See the file COPYING-MOXA for details.

    buzzer_example.c
 
    This is an example program to demonstrate how to turn on/off buzzer.	
    2016-11-08	Ethan SH Lee
	new release
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUZZER_PATH "/sys/class/leds/UC8410A:BEEP/brightness"

/*
	The control function to turn on/off buzzer

	Input: unsigned int 0 or 1
 
	output : return 0 if success,
		 return -1 if fails
 */
static int control_buzzer(unsigned int enabled)
{
	FILE *fptr;
	fptr = fopen(BUZZER_PATH,"w");
	if (fptr == NULL)
	{	
		printf ("Fail to open %s\nProgram exits.\n",BUZZER_PATH);
		return -1;
	}
	if (enabled) //turn on buzzer 
	{
		fwrite("1", sizeof("1"), 1, fptr);
	} else {   //turn off buzzer
	
		fwrite("0", sizeof("0"), 1, fptr);
	}
	fclose(fptr);
	return 0;
}

void usage()
{
	printf ("Usage:./buzzer_example [on/off]\n");
}

static int chk_root()
{
	int user;
	user = getuid();
	return user;
}

int main(int argc, char *argv[])
{
	//check user runs as root
	if (chk_root() != 0)
	{
		printf ("You must run this program as root.\n");
		usage();	
		exit(1);
	}
	
	//check user input arguments	
	if (argc != 2)
	{
		usage();
	} else {
		if(!strcmp(argv[1],"on"))
		{
			control_buzzer(1);
		} else if (!strcmp(argv[1],"off")){
			control_buzzer(0);
		} else {
			usage();
		}
	}	
	return 0;
}
