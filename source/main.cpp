#include <iostream>
#include <fstream>
#include "rs232.h"
#include <Windows.h>
#include "flag_manager.h"
#include <ctime>
#include <future>
#include "sensor_interface.h"

int help()
{
	std::cout << "Usage: co2-logger [options] output_filename\n";
	std::cout << "Polls the Vaisala GMP343 every second and logs csv data with timestamps to output file, as well as stdout. Press \"x\" followed by enter to stop.\n";
	std::cout << "Options:\n";
	std::cout << "-c\t--comport\tSet the name of the COM port that the CO2 sensor is connected to eg. \"COM1\" or \"ttyS5\". Default is COM5\n";
	std::cout << "-d\t--discard\tDoes not log any data to file\n";
	std::cout << "-h\t--help\tPrints this help page and exits\n";
	std::cout << "-q\t--quiet\tSuppresses output to stdout\n";
	return 0;
}


int main(int argc, char **argv)
{
	FlagManager flag;
	flag.SetFlag('c', "comport", true);
	flag.SetFlag('h', "help", false);
	flag.SetFlag('q', "quiet", false);
	flag.SetFlag('d', "discard", false);

	try
	{
		flag.Init(argc, argv);
	}
	catch (const char *e)
	{
		std::cerr << e << '\n';
		return 1;
	}

	if (flag.IsRaised('h'))
	{
		return help();
	}

	if (flag.ArgCount("nonflags") != 1 && !flag.IsRaised('d'))
	{
		std::cerr << "Unexpected number of arguments given. Usage: co2-logger [options] output_filename\n";
		return 1;
	}

	SensorInterface *sensor;

	bool quiet{false};
	bool discard{false};

	if (flag.IsRaised('q'))
	{
		quiet = true;
	}

	if (flag.IsRaised('d'))
	{
		discard = true;
	}
	
	try
	{
		if (flag.IsRaised('c'))
		{
			 sensor = new SensorInterface(flag.FetchArg('c', 0), flag.FetchArg('-', 0), quiet, discard);
		}
		else
		{
			sensor = new SensorInterface("COM5", flag.FetchArg('-', 0), quiet, discard);
		}
	}
	catch (const char *e)
	{
		std::cerr << "\"" << e << "\" could not be opened\n";
		return 1;
	}

	sensor->StartLogging();

	delete sensor;
	return 0;
}
