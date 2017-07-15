#ifndef MPU9150_H
#define MPU9150_H
#include "imu/imu.h"
#include "mpu9250-registers.h"

namespace sensor
{
  class MPU9250: public IMU
  {
  public:
    MPU9250( HALInterface* hwSelection );
    imuData readData();

  private:
    enum aRes
    {
      aRes2g,
      aRes4g,
      aRes10g
    };

    enum gRes
    {
      gRes2g,
      gRes4g,
      gRes10g
    };

    HALInterface* hwInterface_;
  }; 
}

#endif // MPU9150_H
