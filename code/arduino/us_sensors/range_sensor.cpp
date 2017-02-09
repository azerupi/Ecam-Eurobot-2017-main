RangeSensorModule::RangeSensorModule(int address): I2CModule(address), sensor(0) {
}

void RangeSensorModule::setup() {
    I2CModule::setup();

    for (char i = 0; i < N_SENSORS; i++) {
        sensors[N_SENSORS] = NewPing(i + 2, i + 8, 250);
    }
}

// Following commands are defined:
//  - 0001 xxxx get the measurement of sensor xxxx
//  - 0010 0000 get measurements of all sensors
//  - 0011 0000 get number of sensors
//
// Set to 0 if the command is not valid
void RangeSensorModule::extractCommand(char bytes[], char size) {
    const char N_COMMANDS = 3;
    // Commands for the range sensor are coded on one byte
    // If we get any more than that, there is a bug somewhere
    if (size != 1) {
        return;
    }

    if (bytes[0] == 0) {
        return;
    }

    char cmd = 0;
    char sns = 0;

    I2CModule::unpack8(bytes[0], &cmd, &sns);

    // If the command is not valid, set it to zero
    if (cmd < 1 || cmd > N_COMMANDS || sns > N_SENSORS) {
        command = 0;
        sensor = 0;
    }
    else {
        command = cmd;
        sensor = sns;
    }
}

void RangeSensorModule::actionOnRequest() {
    switch (command) {
        case MeasureOne:
            char high = 0;
            char low = 0;
            I2CModule::unpack16(measures[sensor], &high, &low);
            Wire.write({high, low}, 2);
            break;

        case MeasureAll:
            break;

        case Count:
            break;

        case Error:
            break;

        default:

    }
}

void RangeSensorModule::takeReadings() {
    for(byte i = 0; i < N_SENSORS; i++) {
        measures[i] = sensors[i].ping_cm();
    }
}
