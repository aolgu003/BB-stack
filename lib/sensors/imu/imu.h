#ifndef IMU_H
#define IMU_H
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include "HALInterface.h"
#include "sensor-messages.h"
#include "math.h"

extern "C" {
#include "MadgwickAHRS.h"
}

#define PI 3.14159
namespace sensor
{
  /**
   * IMU class is the abstraction layer for each of the different types of imu sensors. The
   * IMU is selected by the select_imu() member function.
   */
  template <class IMU>
  class AHRS
  {
  public:
    AHRS(int i2cdevice, unsigned int i2caddress):
      myIMU_(i2cdevice, i2caddress)
    {
        std::cout << "AHRS constructor" << std::endl;
    }

  public:
    /**
     * @brief readData
     * @return imuData
     */
     ahrsData readData()
     {
       ahrsData output;
       imuData imuOutput = myIMU_.readData();

       //MadgwickAHRSupdate(wx, wy, wz, ax, ay, az, 0.0, 0.0, 0.0);
       MadgwickAHRSupdateIMU(imuOutput.w[0],
                             imuOutput.w[1],
                             imuOutput.w[2],
                             imuOutput.a[0],
                             imuOutput.a[1],
                             imuOutput.a[2]);

       output.imuSample = imuOutput;
       output.q[0] = q0;
       output.q[1] = q1;
       output.q[2] = q2;
       output.q[3] = q3;

       float Roll = atan2(2.0*(q0*q1 + q2*q3), 1 - 2 * (q1*q1 + q2*q2));
       float Pitch = asin(2.0*(q0*q2 - q3*q1));
       float Yaw = atan2(2.0*(q0*q3 + q1*q2), 1 - 2 * (q1*q1 + q2*q2));
       std::cout << "-------- AHRS OUTPUT -------------" << std::endl;
       std::cout << "Yaw: " << Yaw*(180/PI)
                 << " , Pitch: " << Pitch*(180/PI)
                 << " , Roll: " << Roll*(180/PI)
                 << std::endl;
       return output;
     }


  private:
    IMU myIMU_;
  };
}

#endif // IMU_H
