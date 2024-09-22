#ifndef SERIAL_EEPROM_H
#define SERIAL_EEPROM_H

#include <stdint.h>
#include <stdbool.h>

// Base I2C address (minor bits A0:A2 are configured schematically by E0:E2 pins)
#define SERIAL_EEPROM_I2C_ADDR_BASE 0x50

struct Serial_EEPROM_Platform {
    int (*i2cWrite)(uint8_t addr_7bit, const uint8_t *data, uint8_t length, uint8_t wait, uint8_t send_stop);
    int (*i2cRead)(uint8_t addr7bit, uint8_t *data, uint8_t length, int timeout);
    int (*i2cWriteWrite)(uint8_t addr7bit, const uint8_t *data1, uint8_t length1,
        const uint8_t *data2, uint8_t length2, uint8_t wait, uint8_t send_stop);
    int (*i2cWriteRead)(uint8_t addr7bit, const uint8_t *writeData, uint8_t writeLength,
        uint8_t *readData, uint8_t readLength, int timeout);

    void (*gpioSetNwc)(int nwc);

    void (*delayMs)(int ms);
    void (*debugPrint)(const char *fmt, ...);

    uint8_t i2cAddr;
};

void Serial_EEPROM_Init(const struct Serial_EEPROM_Platform *platform);

void Serial_EEPROM_SetWriteProtect(bool enable);

bool Serial_EEPROM_ReadByte(uint16_t address, uint8_t *data);
bool Serial_EEPROM_WriteByte(uint16_t address, uint8_t data);

bool Serial_EEPROM_ReadPage(uint16_t address, uint8_t *data, uint8_t length);
bool Serial_EEPROM_WritePage(uint16_t address, const uint8_t *data, uint8_t length);

#endif // SERIAL_EEPROM_H
