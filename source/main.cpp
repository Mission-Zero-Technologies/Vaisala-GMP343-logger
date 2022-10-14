#include <iostream>
#include <fstream>
#include "rs232.h"
#include <Windows.h>

int main()
{
	int portNumber{RS232_GetPortnr("COM5")};

	if (RS232_OpenComport(portNumber, 19200, "8N1", 0))
	{
		std::cerr << "COM port could not be opened\n";
		return 1;
	}

	const char *send{"SEND\r"};
	RS232_cputs(portNumber, send);
	
	Sleep(100);

	unsigned char *input_buffer{new unsigned char[256]};
	int n{RS232_PollComport(portNumber, input_buffer, 256)};
	//Sleep(100);
	//std::cout << n << '\n';
	// Number starts at index 8. Discard the final 3 characters.
	if (n > 0)
	{
		for (int x{8}; x < n - 3; x++)
		{
			std::cout << (char)input_buffer[x];
		}
		std::cout << '\n';
		//for (int i{0}; i < n; i++)
		//{
		//	if (input_buffer[i] < 32)
		//	{
		//		input_buffer[i] = '.';
		//	}
		//}

		//std::cout << (char *)input_buffer << '\n';
	}

	delete[] input_buffer;
	RS232_CloseComport(portNumber);
	return 0;
}
