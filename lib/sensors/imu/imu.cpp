#include "imu.h"
#include "BlackLibWrapper/blacklibwrapper.h"
#include "mpu9250.h"

namespace sensor
{
template<class IMU>
AHRS<IMU>::AHRS(HALInterface *hwInterface)
{
  myIMU = IMU(hwInterface);
}

template<class IMU>
ahrsData AHRS<IMU>::readData()
{
}

}
