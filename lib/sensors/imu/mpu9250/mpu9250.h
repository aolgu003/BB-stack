#ifndef MPU9150_H
#define MPU9150_H
#include "imu/imu.h"
#include "mpu9250-registers.h"

namespace sensor
{
  class MPU9250
  {
  public:
    MPU9250() {}
    MPU9250( HALInterface* hwSelection );
    imuData readData();

  private:
    void verifyIMUConnected();
    void resetIMU();
    void initIMU();
    void calibrationSetup();
    void collectCalibrationData();
    void calculateBias();

    int numSamples_;
    float accelBias_[3], gyroBias_[3];
    float aScale_, gScale_;
    HALInterface* hwInterface_;

    int16_t gyroOffset_[3], accelOffset_[3];
    mpu9150::aScale accelScale_;
    mpu9150::gScale gyroScale_;
  }; 
}

#endif // MPU9150_H
