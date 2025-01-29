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
        void post(const char *ip);

    private:
        int portNumber;
        bool cont, quiet, discard;
        std::ofstream Writer;
        friend void loggingLoop(SensorInterface *s);
        bool isWhitespace(const char ip);
        bool isNumber(const char ip);
        char* scrubWhitespace(const char *ip);
        char* appendCarriageReturn(const char *ip);
};

void loggingLoop(SensorInterface *s);

#endif
