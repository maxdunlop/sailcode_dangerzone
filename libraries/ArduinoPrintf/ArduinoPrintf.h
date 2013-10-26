#ifndef ARDUINO_PRINTF_H
#define ARDUINO_PRINTF_H 
#include <Arduino.h>
#include <stdarg.h>

class ArduinoPrintf {
    static const uint8_t PRINTF_BUFFER = 80;
    Stream* interface;

    public:
        ArduinoPrintf( Stream* interface );
        void printf( char *fmt, ... );
        void printf_P( const prog_uint8_t* fmt, ... );
};
#endif
