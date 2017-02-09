I2CModule::I2CModule(int address): address(address), command(0) {
}

void I2CModule::setup() {
    Wire.begin(address);
    Wire.onReceive(this->actionOnReceive);
    Wire.onRequest(this->actionOnRequest);
}

void I2CModule::actionOnReceive(int byteCount) {
    int i = 0;
    char received_bytes[byteCount] = {};

    while(Wire.available()){
        char current_byte = Wire.read();

        // Ignore null bytes
        if (current_byte == 0x00)
            continue

        received_bytes[i] = current_byte;
        i++;
    }

    // Call the function to extract commands from the received bytes
    // This method has to be implemented by child classes
    extractCommand(received_bytes, byteCount);
}

static void I2CModule::unpack8(char byte, char &high, char &low) {
    // Write the low 4 bits into the low pointer
    low = byte & 0x0F;
    // Write the high 4 bits into the high pointer
    high = byte >> 4;
}

static void I2CModule::unpack16(int value, char &high, char &low) {
    // Write the low 8 bits into the low pointer
    low = value & 0x00FF;
    // Write the high 8 bits into the high pointer
    high = byte >> 8;
}
