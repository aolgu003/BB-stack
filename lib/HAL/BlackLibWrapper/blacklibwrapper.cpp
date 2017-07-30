#include "blacklibwrapper.h"


sensor::BlackLibWrapper::BlackLibWrapper(BlackLib::i2cName i2c, unsigned int i2cDeviceAddress):
  myI2C(i2c, i2cDeviceAddress)
{
  std::cout << std::hex << myI2C.getDeviceAddress() << std::endl;
  std::cout << myI2C.getPortName() << std::endl;
}

void sensor::BlackLibWrapper::open()
{
  std::cout << "-------- Opening Socket ----------\n";
  if (!myI2C.open(BlackLib::ReadWrite | BlackLib::NonBlock))
    throw std::runtime_error("Error opening i2c port\n");
}

void sensor::BlackLibWrapper::close()
{
  if (!myI2C.close())
    throw std::runtime_error("Error closing i2c port\n");
}

uint8_t sensor::BlackLibWrapper::readI2CByte(uint8_t registerAddr)
{

  uint8_t registerValue;
  if (myI2C.isOpen())
  {
    registerValue = myI2C.readByte(registerAddr);
  }
  else
  {
    throw std::runtime_error("i2c is not open");
  }

  //if ( registerValue == 0x00 )
  //  throw std::runtime_error("Error reading from i2c port\n");

  return registerValue;
}

void sensor::BlackLibWrapper::readI2CBlock(uint8_t registerAddr, uint8_t *readBuffer, size_t bufferSize)
{
  //std::cout << "-------- Reading Byte -----------" << std::endl;
  //std::cout << "Reading register: " << std::hex << (int) registerAddr << std::endl;
  //std::cout << "My device address: " << std::hex << myI2C.getDeviceAddress() << std::endl;
  //std::cout << "My Port path: " << myI2C.getPortName() << std::endl;
  uint8_t blockSize;
  if (myI2C.isOpen())
    blockSize = myI2C.readBlock(registerAddr, readBuffer, bufferSize);
  else
    throw std::runtime_error("i2c is not open");

  if ( blockSize == 0x00 )
    throw std::runtime_error("Error reading block from i2c port\n");
}

void sensor::BlackLibWrapper::writeByte(uint8_t registerAddr, uint8_t value)
{

  bool writeStatus;
  if (myI2C.isOpen())
  {
    writeStatus = myI2C.writeByte(registerAddr, value);
  }
  else
  {
    throw std::runtime_error("i2c is not opened");
  }

  if ( !writeStatus )
    throw std::runtime_error("Error writting block from i2c port\n");
}

void sensor::BlackLibWrapper::writeBlock(uint8_t registerAddr, uint8_t *writeBuffer, size_t bufferSize)
{
  if ( !myI2C.writeBlock(registerAddr, writeBuffer, bufferSize) )
    throw std::runtime_error("Error reading block from i2c port\n");
}

void sensor::BlackLibWrapper::setDeviceAddress(unsigned int newDeviceAddr)
{
  myI2C.setDeviceAddress(newDeviceAddr);
}
