# Overview
Command line interface for serial comminucation and data logging for the Vaisala GMP343 non-dispersive infrared CO<sub>2</sub> sensor, for Microsoft Windows and Linux.
This repository uses code from the RS-232 library located [here](https://gitlab.com/Teuniz/RS-232). `source/rs232.c` and `includes/rs232.h` are taken from this library and have not been modified in any way.

# Compilation
A Makefile is included in this repository for compilation. Has been tested on Windows 10 with the MinGW compiler, as well as on Ubuntu 24.04 LTS using the GNU g++ compiler. There are no dependencies that aren't included in this repository. Simply `cd` into the cloned repository and type
```
make
```
into your command line. This creates an executable named co2-logger.exe by default. It is recommended that you move or copy it into a directory in your $PATH.

# Use
```
co2-logger [options] logfile
```
By default, logs the GMP343's CO<sub>2</sub> concentration reading with a timestamp as csv data to the logfile as well as stdout. The program is ended by pressing "x" followed by the return key.

Options:
```
-c | --comport	Set the name of the COM port the GMP343 is connected to eg. "COM1". Default is "COM5".
-d | --discard	Suppresses output to logfile. When this flag is set, a logfile does not need to be supplied.
-h | --help	Prints this help page to stdout and exits.
-p | --post	This flag takes one argument, appends a carriage return, posts it to the specified COM port, prints the response to stdout and exits. Useful for configuring the GMP343's settings in accordance with its manual.
-q | --quiet	Suppresses output to stdout.
```
