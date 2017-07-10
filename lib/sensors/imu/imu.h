#ifndef IMU_H
#define IMU_H
#include <iostream>

namespace sensor
{
  class AHRS
  {
  public:
    AHRS() {}
    AHRS(AHRS* imu) {}

  public:
    static AHRS* select_imu(std::string imuSelection);
  };
}

#endif // IMU_H
