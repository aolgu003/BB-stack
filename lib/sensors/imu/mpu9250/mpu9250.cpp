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
    collectCalibrationData();
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

      //Wait for PLL to steady
      usleep(200000);

      hwInterface_->writeByte(mpu9150::INT_ENABLE, 0x00);  // Disable all interrupts
      hwInterface_->writeByte(mpu9150::FIFO_EN, 0x00);     // Disable FIFO
      hwInterface_->writeByte(mpu9150::PWR_MGMT_1, 0x00);  // Turn on internal clock source
      hwInterface_->writeByte(mpu9150::I2C_MST_CTRL, 0x00);// Disable I2C master
      hwInterface_->writeByte(mpu9150::USER_CTRL, 0x00);   // Disable FIFO and I2C master
      hwInterface_->writeByte(mpu9150::USER_CTRL, 0x0C);   // Reset FIFO and DMP
      usleep(15000);

      // Configure MPU9250 gyro and accelerometer for bias calculation
      hwInterface_->writeByte(mpu9150::CONFIG, 0x01);      // Set low-pass filter to 188 Hz
      hwInterface_->writeByte(mpu9150::SMPLRT_DIV, 0x04);  // Set sample rate to 200hz
      // Set gyro full-scale to 250 degrees per second, maximum sensitivity
      hwInterface_->writeByte(mpu9150::GYRO_CONFIG, 0x00);
      // Set accelerometer full-scale to 2 g, maximum sensitivity
      hwInterface_->writeByte(mpu9150::ACCEL_CONFIG, 0x00);

    } catch (std::runtime_error err) {
      std::cerr << err.what() << std::endl;
      throw err;
    }
  }

  void MPU9250::collectCalibrationData()
  {
    // Configure FIFO to capture gyro data for bias calculation
    hwInterface_->writeByte(mpu9150::USER_CTRL, 0x40);   // Enable FIFO

    // Enable gyro sensors for FIFO (max size 512 bytes in MPU-9250)
    uint8_t c = mpu9150::FIFO_EN_GYROX|mpu9150::FIFO_EN_GYROY|mpu9150::FIFO_EN_GYROZ;
    hwInterface_->writeByte(mpu9150::FIFO_EN, c);
    // 6 bytes per sample. 200hz. wait 0.4 seconds
    usleep(400000);

    // At end of sample accumulation, turn off FIFO sensor read
    hwInterface_->writeByte(mpu9150::FIFO_EN, 0x00);

    // read FIFO sample count and log number of samples
    uint8_t data[6] = {0,0,0,0,0,0};
    hwInterface_->readI2CBlock( mpu9150::FIFO_COUNTH, &data[0], 2 );
    int16_t fifo_count = ((uint16_t)data[0] << 8) | data[1];
    int samples = fifo_count/6;
  }
}


