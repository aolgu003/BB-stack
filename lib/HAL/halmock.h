#ifndef HALMOCK_H
#define HALMOCK_H
#include "gmock/gmock.h"
#include "HALInterface.h"
#include "mpu9250-registers.h"

namespace sensor {
  class HALMock: public HALInterface
  {
  public:
    HALMock() : HALInterface() {}

    MOCK_METHOD0(open, void());
    MOCK_METHOD0(close, void());

    MOCK_METHOD1(readI2CByte, uint8_t(uint8_t registerAddr));
    MOCK_METHOD3(readI2CBlock, void(uint8_t registerAddr, uint8_t* readBuffer, size_t bufferSize));

    MOCK_METHOD2(writeByte, void(uint8_t registerAddr, uint8_t value));
    MOCK_METHOD3(writeBlock, void(uint8_t registerAddr, uint8_t* writeBuffer, size_t bufferSize));

    MOCK_METHOD1(setDeviceAddress, void(unsigned int newDeviceAddr));
  };

}

#endif // HALMOCK_H
