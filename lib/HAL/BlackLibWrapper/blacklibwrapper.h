#ifndef BLACKLIBWRAPPER_H
#define BLACKLIBWRAPPER_H
#include "HALInterface.h"
#include "BlackLib.h"
#include <stdint.h>
#include <stddef.h>

namespace sensor {
  class BlackLibWrapper : public HALInterface
  {
  public:
    BlackLibWrapper(BlackLib::i2cName i2c, unsigned int i2cDeviceAddress);
  private:
    BlackLib::BlackI2C myI2C;
    // HALInterface interface
  public:
    void open();
    void close();
    uint8_t readI2CByte(uint8_t registerAddr);
    void readI2CBlock(uint8_t registerAddr, uint8_t *readBuffer, size_t bufferSize);
    void writeByte(uint8_t registerAddr, uint8_t value);
    void writeBlock(uint8_t registerAddr, uint8_t *writeBuffer, size_t bufferSize);
    void setDeviceAddress(unsigned int newDeviceAddr);
  };
}

#endif // BLACKLIBWRAPPER_H
