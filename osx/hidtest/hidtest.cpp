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
    uint16_t    red;
    uint16_t    grn;
    uint16_t    blu;
    uint16_t    rate;
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
	BL_RGB,
	BL_AVAILABLE,
	BL_BUSY,
	BL_DND,
	BL_AWAY,
	BL_RAINBOW,
	BL_LAST
};

int main(int argc, char* argv[])
{
	hid_device* hDevice = hid_open(0xb337, 0x01, NULL);
	if (!hDevice) 
	{
		std::cerr << "Unable to open device; is it plugged in?" << std::endl;
 		return (-1);
	}
	
	// Set the hid_read() function to be non-blocking.
	hid_set_nonblocking(hDevice, 1);
	
	config_t txData;
	memset((void*)&txData, 0, sizeof(config_t));

	response_to_host_t rxData;
	memset((void*)&rxData, 0, sizeof(response_to_host_t));

	std::string userIn;
	bool quit = false;

	do
	{
		userIn.clear();

		std::cout << "+---------------------------------+" << std::endl
				  << "|  Super duper beacon controller  |" << std::endl
				  << "+---------------------------------+" << std::endl
				  << "| 1 - off :(      2 - available   |" << std::endl
				  << "| 3 - rainbow     4 - busy        |" << std::endl
				  << "| 5 - DND         6 - away        |" << std::endl
				  << "| x - buh-bye                     |" << std::endl
				  << "+---------------------------------+" << std::endl;

		std::cin >> userIn;

		if (userIn == std::string("x"))
		{
			quit = true;
		}
		else
		{
			uint16_t what = std::atoi(userIn.c_str());
			bool duh = false;

			memset((void*)&txData, 0, sizeof(config_t));

			switch (what)
			{
				case 1:	txData.cmd.ptrn = BL_CLEAR;		break;
				case 2: txData.cmd.ptrn = BL_AVAILABLE;	break;
				case 3: txData.cmd.ptrn = BL_RAINBOW;	break;
				case 4: txData.cmd.ptrn = BL_BUSY;		break;
				case 5: txData.cmd.ptrn = BL_DND;		break;
				case 6: txData.cmd.ptrn = BL_AWAY;		break;
				default: duh = true;					break;
			}

			if (!duh)
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
				}
			}
		}
		
	} while (!quit);

	hid_close(hDevice);

	/* Free static HIDAPI objects. */
	hid_exit();

	return (0);
}
