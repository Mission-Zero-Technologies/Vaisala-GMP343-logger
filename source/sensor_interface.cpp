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
    int opLen{0};
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

bool SensorInterface::isNumber(const char ip)
{
    int ascii{(int) ip};
    if (ascii >= 48 && ascii <= 57)
    {
        return true;
    }
    return false;
}

char* SensorInterface::Read()
{
	Sleep(2000);

    unsigned char *input_buffer{new unsigned char[256]};
	int ResponseLength{RS232_PollComport(portNumber, input_buffer, 256)};
    int opLen{0};
    char *op;

    if (ResponseLength > 0)
    {
        int startIndex{0};
        while (!isNumber((char)input_buffer[startIndex]))
        {
            startIndex++;
            if (startIndex >= ResponseLength)
            {
                break;
            }
        }

        for (int n{startIndex}; n < ResponseLength; n++)
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
            op[n] = (char)input_buffer[n + startIndex];
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
    RS232_cputs(portNumber, "R\r");

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
    RS232_cputs(portNumber, "S\r");

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
    op[ipLen + 1] = '\0';
    return op;
}

void SensorInterface::post(const char *ip)
{
    char *post{appendCarriageReturn(ip)};
    RS232_cputs(portNumber, post);
    delete[] post;
    post = nullptr;
	Sleep(500);

    unsigned char *input_buffer{new unsigned char[2048]};
	int ResponseLength{RS232_PollComport(portNumber, input_buffer, 2048)};


    if (ResponseLength > 0)
    {
        input_buffer[ResponseLength] = '\0';
        std::cout << (char*)input_buffer << '\n';
    }
    else
    {
        std::cout << "No response received\n";
    }
}