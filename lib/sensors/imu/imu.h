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
  template <class IMU>
  class AHRS
  {
  public:
    AHRS(HALInterface* hwInterface);

  public:
    /**
     * @brief readData
     * @return imuData
     */
     ahrsData readData()=0;

  private:
    IMU myIMU;
  };
}

#endif // IMU_H
