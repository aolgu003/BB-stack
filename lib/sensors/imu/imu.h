#ifndef IMU_H
#define IMU_H
#include "senormessages.h"
#include "sensor-template.h"
#include "MadgwickAHRS.h"

namespace sensor
{
  class IMU
  {
  public:
    IMU() {}
    imuData readAHRS();
  protected:
    virtual imuData readIMU()=0;
    imuData computeAHRSUpdate(imuData input);
  };
}

#endif // IMU_H
