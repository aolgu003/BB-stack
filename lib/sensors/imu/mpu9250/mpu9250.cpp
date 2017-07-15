#include "mpu9250.h"

//TODO Write calibration code
//TODO Write calibration parameter saving
//TODO Write calibration parameter loading
//TODO Write readData function
namespace sensor {
  MPU9250::MPU9250(HALInterface* hwSelection):
    IMU::IMU( ),
    hwInterface_(hwSelection)
  {
    verifyIMUConnected();

    // Calibrate gyro
    calibrationSetup();
  }

  imuData MPU9250::readData()
  {

  }

  void MPU9250::verifyIMUConnected()
  {
    try {

      hwInterface_->open();

      uint8_t IAmMPU;
      IAmMPU = hwInterface_->readI2CByte(mpu9150::WHO_AM_I_MPU9150); //IMU
      if (IAmMPU != mpu9150::WHO_AM_I_MPU9150)
        throw std::runtime_error("MPU who am I failed");

      uint8_t IAmAK;
      IAmAK = hwInterface_->readI2CByte(mpu9150::WHO_AM_I_AK8975A); //MAG
      if (IAmAK != mpu9150::WHO_AM_I_AK8975A)
        throw std::runtime_error("AK who am I failed");
    }
    catch (std::runtime_error err) {
      std::cerr << err.what() << std::endl;
      hwInterface_->close();
      throw err;
    }
  }

  void MPU9250::calibrationSetup()
  {
    try {
      hwInterface_->writeByte(mpu9150::PWR_MGMT_1, 0x01);
      hwInterface_->writeByte(mpu9150::PWR_MGMT_2, 0x00);

      hwInterface_->
    }
  }
}


