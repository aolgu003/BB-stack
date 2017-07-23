#ifndef BLACKLIBINTERFACE_H
#define BLACKLIBINTERFACE_H
#include <stdint.h>
#include <stddef.h>

namespace sensor {
  class HALInterface {
  public:
    HALInterface(){}

    virtual ~HALInterface(){}
    //Auto open in nonblock with readWrite permisions
    virtual void open()=0;
    virtual void close()=0;

    virtual uint8_t readI2CByte(uint8_t registerAddr)=0;
    virtual void readI2CBlock(uint8_t registerAddr, uint8_t* readBuffer, size_t bufferSize)=0;

    virtual void writeByte (uint8_t registerAddr, uint8_t value)=0;
    virtual void writeBlock (uint8_t registerAddr, uint8_t *writeBuffer, size_t bufferSize)=0;

    virtual void setDeviceAddress(unsigned int newDeviceAddr)=0;
  private:

  };

  enum hwInterface {
    BlackLibInterface,
    mock
  };

}

#endif // BLACKLIBINTERFACE_H
