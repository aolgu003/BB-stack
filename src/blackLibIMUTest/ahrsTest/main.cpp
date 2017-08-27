#include <iostream>
#include "imu.h"
#include "BlackLib.h"
#include "mpu9250.h"
#include "blacklibwrapper.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello World!" << endl;
  try
  {
    sensor::AHRS<sensor::MPU9250> myAHRS(0,0);
    while(1)
    {
      myAHRS.readData();
    }
  } catch (std::runtime_error err){
    std::cerr << err.what() << std::endl;
    std::terminate();
  }

  return 0;
}
