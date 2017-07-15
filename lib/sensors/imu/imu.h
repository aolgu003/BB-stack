#ifndef IMU_H
#define IMU_H
#include <iostream>
#include <unistd.h>
#include <stdexcept>
#include "HALInterface.h"
#include "sensor-messages.h"

namespace sensor
{
  /**
   * IMU class is the abstraction layer for each of the different types of imu sensors. The
   * IMU is selected by the select_imu() member function.
   */
  class IMU
  {
  public:
    IMU() {}

  public:

    enum imuSensors {
      mpu9250
    };

    /**
     * @brief readData
     * @return imuData
     */
    virtual imuData readData()=0;

    /**
     * @brief select_imu
     * @param imuSelection
     * @return
     */
    static IMU* select_imu( imuSensors imuSelection, hwInterface hwSelection);
  };
}

#endif // IMU_H
