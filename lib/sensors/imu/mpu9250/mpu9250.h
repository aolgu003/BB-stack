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
    void verifyIMUConnected();
    void calibrationSetup();
    HALInterface* hwInterface_;
  }; 
}

#endif // MPU9150_H
