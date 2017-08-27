#include <iostream>
#include "imu.h"
#include "BlackLib.h"
#include "mpu9250.h"
#include "blacklibwrapper.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Hello World!" << endl;
  cout << "Running AHRS Intergration test!" << endl;

  try
  {
    sensor::AHRS<sensor::MPU9250> myAHRS(2,0x68);
    std::cout << "Startup complete begin vomitting data" << std::endl;
    while(1)
    {
      myAHRS.readData();
      usleep(100000);
    }
  } catch (std::runtime_error err){
    std::cerr << err.what() << std::endl;
    std::terminate();
  }

  return 0;
}
