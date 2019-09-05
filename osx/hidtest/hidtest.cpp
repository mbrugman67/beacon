/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009, All Rights Reserved.
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <unistd.h>

#include "hidapi.h"

#define CA_SIZE     32



struct host_command_t
{
    uint16_t    ptrn;
    uint16_t    posn;
    uint16_t    red;
    uint16_t    grn;
    uint16_t    blu;
    uint16_t    len;
};

struct config_t
{
    uint16_t        signature;
    uint16_t        bootCount;
    host_command_t  cmd;

};

struct response_to_host_t
{
    uint16_t sequence;      //  0 /  2
    uint16_t bootCount;     //  2 /  4
    uint16_t versionMajor;  //  4 /  6
    uint16_t versionMinor;  //  6 /  8
};

enum 
{
	BL_CLEAR = 0,
	BL_START,
	BL_SWING,
	BL_FADE,
	BL_BREATHE,
	BL_PULSE,
	BL_LAST
};

int main(int argc, char* argv[])
{
	config_t txData;
	memset((void*)&txData, 0, sizeof(config_t));

	response_to_host_t rxData;
	memset((void*)&rxData, 0, sizeof(response_to_host_t));
	size_t moveCount(0);

	std::string command;

	if (argc > 1)
	{
		command = std::string(argv[1]);
	}

	if (argc == 5)
	{
		txData.cmd.red = atoi(argv[2]);
		txData.cmd.grn = atoi(argv[3]);
		txData.cmd.blu = atoi(argv[4]);
	}
	else
	{
		txData.cmd.red = 255;
		txData.cmd.grn = 255;
		txData.cmd.blu = 255;
	}


	hid_device* hDevice = hid_open(0xb337, 0x01, NULL);
	if (!hDevice) 
	{
		std::cerr << "Unable to open device; is it plugged in?" << std::endl;
 		return (-1);
	}
	else
	{
		std::cout << "Connected to device!" << std::endl;
	}
	
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(hDevice, 1);

	if (command == "clear")
	{
		txData.cmd.ptrn = BL_CLEAR;
	}
	else if (command == "breathe")
	{
		txData.cmd.ptrn = BL_BREATHE;
	}
	else if (command == "pulse")
	{
		txData.cmd.ptrn = BL_PULSE;
	}
	else
	{
		std::cout << "clear, breathe, or pulse; that's all I know." << std::endl;	
		hid_close(hDevice);
		hid_exit();
		exit (1);
	}
	do
	{
		// set some stuffs
		ssize_t txResult = txResult = hid_write(hDevice, (unsigned char*)&txData.cmd, sizeof(config_t));
		if (!txResult)
		{
			std::cerr << "No bytes sent" << std::endl;
			break;
		}
		else if (txResult < 0)
		{
			std::cerr << "TX Error " << txResult << std::endl;
			break;
		}
		else
		{
			usleep(50 * 1000);
			ssize_t rxResult = hid_read(hDevice, (unsigned char*)&rxData.sequence, sizeof(response_to_host_t));

			if (rxResult < 0)
			{
				std::cerr << "RX Error " << rxResult << std::endl;
				break;
			}
			else
			{
				++moveCount;
				std::cout << "sequence = " << rxData.sequence << std::endl;
			}
		}
		//usleep(50 * 1000);
	} while (false);

	hid_close(hDevice);

	/* Free static HIDAPI objects. */
	hid_exit();

	return (0);
}
