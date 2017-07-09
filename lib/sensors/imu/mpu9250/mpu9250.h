#ifndef MPU9150_H
#define MPU9150_H
#include "imu.h"
#include "mpu9150-registers.h"

namespace sensor
{

  class MPU9150: IMU
  {
    MPU9150();
    imuData readIMU();

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

    float calculateAres(aRes sf);
    float calculateGres(gRes sf);
  };
}

#endif // MPU9150_H
