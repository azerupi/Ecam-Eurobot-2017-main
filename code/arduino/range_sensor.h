#ifndef RANGE_SENSOR_H
#define RANGE_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <NewPing.h>

template <int N_SENSORS>
class RangeSensorModule: public I2CModule
{
    public:
        RangeSensorModule(int address);
        virtual void setup();
        virtual void actionOnReceive(int byteCount);
        virtual void actionOnRequest();
        virtual void extractCommand(char bytes[], char size);
        void takeReadings();

    private:
        char sensor;
        NewPing sensors[N_SENSORS];
        int measures[N_SENSORS];

    enum Commands {
        MeasureOne = 1,
        MeasureAll = 2,
        Count = 3,
        Error = 0,
    }
};

#endif
