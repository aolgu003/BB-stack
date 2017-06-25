#ifndef IMU_H
#define IMU_H
#include "../sensor-template.h"
#include "../senormessages.h"
#include "MadgwickAHRS.h"

using namespace sensor
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
