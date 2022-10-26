#include "sensor_interface.h"
#include "rs232.h"
#include <Windows.h>
#include <thread>
#include <ctime>
#include <fstream>
#include <iostream>
#include "fl2_utils.h"

SensorInterface::SensorInterface(const char *ComPortName, const char *filename, bool ipQuiet, bool ipDiscard)
{
    portNumber = RS232_GetPortnr(ComPortName);

    if (RS232_OpenComport(portNumber, 19200, "8N1", 0))
	{
		throw ComPortName;
	}

    if (!ipDiscard)
    {
        Writer.open(filename);
        if (Writer.fail())
        {
            throw filename;
        }
    }

    quiet = ipQuiet;
    discard = ipDiscard;
}

SensorInterface::~SensorInterface()
{
    RS232_CloseComport(portNumber);
    Writer.close();
}

bool SensorInterface::isWhitespace(const char ip)
{
    if (ip == ' ' || ip == '\t' || ip == '\n')
    {
        return true;
    }
    return false;
}

char* SensorInterface::scrubWhitespace(const char *ip)
{
    int opLen{0}; //I don't know why this is -1 but it works lol
    int index{0};
    while (ip[index] != '\0')
    {
        if (!isWhitespace(ip[index]))
        {
            opLen++;
        }
        index++;
    }

    char *op{new char[opLen + 1]};

    index = 0;
    int opIndex{0};
    while (ip[index] != '\0')
    {
        if (!isWhitespace(ip[index]))
        {
            op[opIndex] = ip[index];
            opIndex++;
        }
        index++;
    }
    op[opLen] = '\0';
    return op;
}

char* SensorInterface::Read()
{
	RS232_cputs(portNumber, send);
	Sleep(2000);

    unsigned char *input_buffer{new unsigned char[256]};
	int ResponseLength{RS232_PollComport(portNumber, input_buffer, 256)};
    int opLen{0};
    char *op;

    if (ResponseLength > 0)
    {
        for (int n{5}; n < ResponseLength; n++)
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
            op[n] = (char)input_buffer[n + 5];
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

        char *raw_output{Read()};
        char *op{scrubWhitespace(raw_output)};

        delete[] raw_output;

        if (!discard)
        {
            Writer << timestamp << ',' << op << '\n';
        }

        if (!quiet)
        {
            std::cout << timestamp << ',' << op << '\n';
        }

        delete[] op;
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

char* SensorInterface::appendCarriageReturn(const char *ip)
{
    int ipLen{fl2_utils::len(ip)};
    char *op{new char[ipLen + 2]};

    for (int n{0}; n < ipLen; n++)
    {
        op[n] = ip[n];
    }
    op[ipLen] = '\r';
    op[ipLen +  1] = '\0';
    return op;
}

void SensorInterface::post(const char *ip)
{
    char *post{appendCarriageReturn(ip)};
    RS232_cputs(portNumber, post);
    delete[] post;
    post = nullptr;
	Sleep(500);

    //const int bufferLen{2048};

    unsigned char *input_buffer{new unsigned char[2048]};
	int ResponseLength{RS232_PollComport(portNumber, input_buffer, 2048)};


    if (ResponseLength > 0)
    {
        /*
        int opLen{0};
        int crCount{0};

        for (int n{0}; n < ResponseLength; n++)
        {   
            if ((char)input_buffer[n] != '\r')
            {
                opLen++;
            }
            else
            {
                if (crCount >= 1)
                {
                    break;
                }
                crCount++;
            }
        }

        char *op{new char[opLen + 1]};
        int opIndex{0};

        for (int n{0}; n < ResponseLength; n++)
        {
            if ((char)input_buffer[n] != '\r')
            {   
                op[opIndex] = (char)input_buffer[n];
                opIndex++;
            }
        }
        op[opLen] = '\0';

        std::cout << op << '\n';

        delete[] op;
        */
        input_buffer[ResponseLength] = '\0';
        std::cout << (char*)input_buffer << '\n';
    }
    else
    {
        std::cout << "No response received\n";
    }
}