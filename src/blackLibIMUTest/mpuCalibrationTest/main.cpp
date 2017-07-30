#include <iostream>
#include "mpu9250.h"
#include "blacklibwrapper.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello World!" << endl;
  sensor::BlackLibWrapper myInterface(BlackLib::I2C_2,
                                 0x68);
  try
  {
    sensor::MPU9250 myIMU(&myInterface);
    sensor::imuData imuData;
    while(1)
    {
      imuData = myIMU.readData();
      usleep(100000);
    }
  } catch (std::runtime_error err){
    std::cerr << err.what() << std::endl;
    std::terminate();
  }



  return 0;
}
