#include "sensor_interface.h"
#include "rs232.h"
#include <Windows.h>
#include <thread>
#include <ctime>
#include <fstream>
#include <iostream>

SensorInterface::SensorInterface(const char *ComPortName, const char *filename, bool ipQuiet)
{
    portNumber = RS232_GetPortnr(ComPortName);

    if (RS232_OpenComport(portNumber, 19200, "8N1", 0))
	{
		throw ComPortName;
	}

    Writer.open(filename);
    if (Writer.fail())
    {
        throw filename;
    }

    quiet = ipQuiet;
}

SensorInterface::~SensorInterface()
{
    RS232_CloseComport(portNumber);
    Writer.close();
}

char* SensorInterface::Read()
{
	RS232_cputs(portNumber, send);
	Sleep(100);

    unsigned char *input_buffer{new unsigned char[256]};
	int ResponseLength{RS232_PollComport(portNumber, input_buffer, 256)};
    int opLen{0};
    char *op;

    if (ResponseLength > 0)
    {
        for (int n{8}; n < ResponseLength; n++)
        {
            if ((char)input_buffer[n] == '\r')
            {
                break;
            }
            opLen++;
        }

        op = new char[opLen + 1];

        for (int n{0}; n < opLen; n++)
        {
            op[n] = (char)input_buffer[n + 8];
        }
        op[opLen] = '\0';
    }
    else
    {
        op = new char[1];
        op[0] = '\0';
    }

    delete[] input_buffer;
    return op;
}

void SensorInterface::loggingLoop()
{
	long int initialTime{static_cast<long int> (time(nullptr))};

    while (cont)
    {
        long int timestamp = static_cast<long int> (time(nullptr)) - initialTime;

        char *op{Read()};

        Writer << timestamp << ',' << op << '\n';
        if (!quiet)
        {
            std::cout << timestamp << ',' << op << '\n';
        }
        
        delete[] op;

        Sleep(900);
    }
}

void SensorInterface::StartLogging()
{
    cont = true;

    std::thread logger(loggingLoop, this);

    while (cont)
    {
        std::cin.sync();
	    char c;
	    std::cin >> c;
	    if (c == 'x')
	    {
	    	cont = false;
	    }
    }

    logger.detach();

}