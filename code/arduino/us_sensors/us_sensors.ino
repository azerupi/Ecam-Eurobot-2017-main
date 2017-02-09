#include "range_sensor.h"

const byte ADDRESS = 0x04;
const byte N_SENSORS = 2;

int measures[N_SENSORS];
unsigned long last_measure = 0;

RangeSensorModule<N_SENSORS> sensor_module = RangeSensorModule<N_SENSORS>(ADDRESS);

void setup() {
    sensor_module.setup();
}

void loop() {
    if (millis() - last_measure > 100) {
        sensor_module.takeReadings();
        last_measure = millis();
    }
}
