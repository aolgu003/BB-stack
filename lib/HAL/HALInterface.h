#ifndef BLACKLIBINTERFACE_H
#define BLACKLIBINTERFACE_H
#include <stdint.h>
#include <stddef.h>

namespace sensor {
  class HALInterface {
  public:
    HALInterface(){}

    virtual ~HALInterface(){}

    virtual void open(unsigned int openMode)=0;
    virtual void close()=0;

    virtual uint8_t readI2CByte(uint8_t registerAddr)=0;
    virtual void readI2CBlock(uint8_t registerAddr, uint8_t* readBuffer, size_t bufferSize)=0;

    virtual void writeByte (uint8_t registerAddr, uint8_t value)=0;
    virtual void writeBlock (uint8_t registerAddr, uint8_t *writeBuffer, size_t bufferSize)=0;
  };

  enum hwInterface {
    BlackLib,
    mock
  };
}

#endif // BLACKLIBINTERFACE_H
