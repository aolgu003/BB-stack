#include <iostream>
#include <BlackI2C/BlackI2C.h>

using namespace std;

int main(int argc, char *argv[])
{
  BlackLib::BlackI2C myI2c(BlackLib::I2C_2, 0x68);
  bool isOpened = myI2c.open( BlackLib::ReadWrite | BlackLib::NonBlock );

  if (myI2c.fail(BlackLib::BlackI2C::openErr))
  {
    std::cout << "Error opening" << std::endl;
    return 0;
  }

  std::cout << "Opened device" << std::endl;
  uint8_t iAm = myI2c.readByte(0x75);

  if (myI2c.fail(BlackLib::BlackI2C::readErr))
  {
    std::cout << "Error reading" << std::endl;
    return 0;
  }

  std::cout << "I am: ";
  std::cout << std::hex << (int) iAm << std::endl;

  return 0;
}
