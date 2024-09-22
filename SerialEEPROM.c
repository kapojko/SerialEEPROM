#include "SerialEEPROM.h"

#define I2C_TIMEOUT_MS 5 // 5ms
#define WRITE_TIME_MS 5 // 5ms

static bool writeProtectSet = false;

static struct Serial_EEPROM_Platform platform;

void Serial_EEPROM_Init(const struct Serial_EEPROM_Platform *platformPtr) {
    platform = *platformPtr;
}

void Serial_EEPROM_SetWriteProtect(bool enable) {
    platform.gpioSetNwc(enable ? 1 : 0);
    writeProtectSet = enable;
}

bool Serial_EEPROM_ReadByte(uint16_t address, uint8_t *data) {
    uint8_t addrBuf[2] = {address >> 8, address & 0xFF};
    int ret = platform.i2cWriteRead(platform.i2cAddr, addrBuf, 2, data, 1, I2C_TIMEOUT_MS);
    if (ret < 0) {
        platform.debugPrint("Error reading EEPROM: %d\r\n", -ret);
        return false;
    }

    return true;
}

bool Serial_EEPROM_WriteByte(uint16_t address, uint8_t data) {
    bool wasWriteProtect = writeProtectSet;
    if (writeProtectSet && platform.gpioSetNwc) {
        platform.gpioSetNwc(0);
    }

    uint8_t addrBuf[2] = {address >> 8, address & 0xFF};
    int ret = platform.i2cWriteWrite(platform.i2cAddr, addrBuf, 2, &data, 1, I2C_TIMEOUT_MS, 1);

    platform.delayMs(WRITE_TIME_MS);

    if (wasWriteProtect && platform.gpioSetNwc) {
        platform.gpioSetNwc(1);
    }

    if (ret < 0) {
        platform.debugPrint("Error writing EEPROM: %d\r\n", -ret);
        return false;
    }
    return true;
}

bool Serial_EEPROM_ReadPage(uint16_t address, uint8_t *data, uint8_t length) {
    uint8_t addrBuf[2] = {address >> 8, address & 0xFF};
    int ret = platform.i2cWriteRead(platform.i2cAddr, addrBuf, 2, data, length, I2C_TIMEOUT_MS);
    if (ret < 0) {
        platform.debugPrint("Error reading EEPROM: %d\r\n", -ret);
        return false;
    }
    return true;
}

bool Serial_EEPROM_WritePage(uint16_t address, const uint8_t *data, uint8_t length) {
    bool wasWriteProtect = writeProtectSet;
    if (writeProtectSet && platform.gpioSetNwc) {
        platform.gpioSetNwc(0);
    }

    uint8_t addrBuf[2] = {address >> 8, address & 0xFF};
    int ret = platform.i2cWriteWrite(platform.i2cAddr, addrBuf, 2, data, length, I2C_TIMEOUT_MS, 1);
    
    platform.delayMs(WRITE_TIME_MS);
    
    if (wasWriteProtect && platform.gpioSetNwc) {
        platform.gpioSetNwc(1);
    }

    if (ret < 0) {
        platform.debugPrint("Error writing EEPROM: %d\r\n", -ret);
        return false;
    }
    return true;
}
