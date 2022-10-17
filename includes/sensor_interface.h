#ifndef SENSOR_INTERFACE
#define SENSOR_INTERFACE

#include <fstream>

class SensorInterface
{
    public:
        SensorInterface(const char *ComPortName, const char *filename, bool ipQuiet, bool ipDiscard);
        char* Read();
        ~SensorInterface();
        void StartLogging();

    private:
        int portNumber;
        bool cont, quiet, discard;
        const char *send{"SEND\r"};
        std::ofstream Writer;
        void loggingLoop();
        bool isWhitespace(const char ip);
        char* scrubWhitespace(const char *ip);
};

#endif