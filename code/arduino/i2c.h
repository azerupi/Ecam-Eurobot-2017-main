#ifndef I2CModule_H
#define I2CModule_H

#include <Arduino.h>
#include <Wire.h>

// Abstract class
class I2CModule
{
    public:
        I2CModule(int address);
        virtual void setup();
        virtual void actionOnReceive(int byteCount);
        virtual void actionOnRequest() = 0;
        virtual void extractCommand(char bytes[], char size) = 0;

        static void unpack8(char byte, char &high, char &low);
        static void unpack16(int value, char &high, char &low);

    private:
        int address;
        char command;
};

#endif
