#ifndef SENSOR_INTERFACE
#define SENSOR_INTERFACE

#include <fstream>

class SensorInterface
{
    public:
        SensorInterface(const char *ComPortName, const char *filename, bool ipQuiet);
        char* Read();
        ~SensorInterface();
        void StartLogging();

    private:
        int portNumber;
        bool cont, quiet;
        const char *send{"SEND\r"};
        std::ofstream Writer;
        void loggingLoop();
};

#endif