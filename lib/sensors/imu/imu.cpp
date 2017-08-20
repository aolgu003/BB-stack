#include "imu.h"
#include "mpu9250.h"
#include "MadgwickAHRS.h"

namespace sensor
{
template<class IMU>
AHRS<IMU>::AHRS(HALInterface *hwInterface)
{
  myIMU_ = IMU(hwInterface);
}

template<class IMU>
ahrsData AHRS<IMU>::readData()
{
  sensor::imuData imuOutput;
  imuOutput = myIMU_.readData();
  MadgwickAHRSupdateIMU(imuOutput.w[0], imuOutput.w[1], imuOutput.w[2], imuOutput.a[0], imuOutput.a[1], imuOutput.a[2]);
  sensor::ahrsData ahrsOutput;
  ahrsOutput.a = imuOutput.a;
  ahrsOutput.w = imuOutput.w;
  ahrsOutput.q[0] = q0;
  ahrsOutput.q[1] = q1;
  ahrsOutput.q[2] = q2;
  ahrsOutput.q[3] = q3;

  return ahrsOutput;
}

}
