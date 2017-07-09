#include "imu.h"

//TODO

namespace sensor
{
  imuData IMU::readAHRS()
  {
    imuData imuMeasurement, ahrsMeasurement;

    imuMeasurement = readIMU();
    ahrsMeasurement = computeAHRSUpdate(imuMeasurement);

    return ahrsMeasurement;
  }

    imuData IMU::computeAHRSUpdate(imuData input)
    {
      MadgwickAHRSupdate(input.w[0], input.w[1], input.w[2],
          input.a[0], input.a[1], input.a[2],
          input.m[0], input.m[1], input.m[2]);
      imuData output;
      output = input;
      output.q[0] = q0;
      output.q[1] = q1;
      output.q[2] = q2;
      output.q[3] = q3;

      return output;
    }
}
