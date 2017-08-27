#include "mpu9250.h"
#include <bitset>

//TODO Write calibration code
//TODO Write calibration parameter saving
//TODO Write calibration parameter loading
//TODO Write readData function
namespace sensor {
MPU9250::MPU9250(int i2cdevice, unsigned int i2caddress):
  aScale_(2.0/32768.0),
  gScale_(250.0/32768.0)

{
  HALInterface* myHALInterface =
      HALFactory::makeHALInstance(hwInterface::BlackLibInterface, i2cdevice, i2caddress );

  startupIMU();
}

MPU9250::MPU9250(HALInterface* hwSelection):
    hwInterface_(hwSelection),
    aScale_(2.0/32768.0),
    gScale_(250.0/32768.0)
  {
    startupIMU();
  }

  imuData MPU9250::readData()
  {
    imuData output;
    uint8_t rawData[6];  // x/y/z accel register data stored here
    hwInterface_->readI2CBlock( mpu9150::ACCEL_XOUT_H, &rawData[0], 6);  // Read the six raw data registers into data array
    int16_t accelOutput[3];
    accelOutput[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    accelOutput[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    accelOutput[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
    std::cout << "Acell Data \n";
    std::cout << "---- Raw data: \n";
    std::cout << (float) accelOutput[0] << std::endl;
    std::cout << (float) accelOutput[1] << std::endl;
    std::cout << (float) accelOutput[2] << std::endl;

    output.a[0] = ((float)(accelOutput[0]-accelOffset_[0]))*aScale_;
    output.a[1] = ((float)(accelOutput[1]-accelOffset_[1]))*aScale_;
    output.a[2] = ((float)(accelOutput[2]-accelOffset_[2]))*aScale_;
    std::cout << "---- Processed Data: " << std::endl;
    std::cout << output.a[0] << std::endl;
    std::cout << output.a[1] << std::endl;
    std::cout << output.a[2] << std::endl;

    hwInterface_->readI2CBlock( mpu9150::GYRO_XOUT_H, &rawData[0], 6);  // Read the six raw data registers sequentially into data array
    int16_t gyroOutput[3];
    gyroOutput[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gyroOutput[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gyroOutput[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
    output.w[0] = ((float) (gyroOutput[0] - gyroOffset_[0]))*gScale_;
    output.w[1] = ((float) (gyroOutput[1] - gyroOffset_[1]))*gScale_;
    output.w[2] = ((float) (gyroOutput[2] - gyroOffset_[2]))*gScale_;
    std::cout << "---------------" << std::endl;
    std::cout << "GYRO Data \n";
    std::cout << "---- Raw data: \n";
    std::cout << std::hex << gyroOutput[0] << std::endl;
    std::cout << std::hex << gyroOutput[1] << std::endl;
    std::cout << std::hex << gyroOutput[2] << std::endl;

    std::cout << "---- Processed Data: " << std::endl;
    std::cout << output.w[0] << std::endl;
    std::cout << output.w[1] << std::endl;
    std::cout << output.w[2] << std::endl;
    std::cout << "---------------" << std::endl;

    return output;
  }

  void MPU9250::startupIMU()
  {
    std::cout << ".....Verifying IMU Connected....." << std::endl;
    verifyIMUConnected();
    std::cout << ".....IMU Connected.....\n" << std::endl;

    // Calibrate gyro
    std::cout << ".....Reseting IMU ....." << std::endl;
    resetIMU();
    usleep(2000000);

    std::cout << ".....Initializing IMU ....." << std::endl;
    initIMU();
    usleep(2000000);

    std::cout << "\n..... Calculating IMU bias ....." << std::endl;
    calculateBias();

    std::cout << "\n\n.....Calibration complete....." << std::endl;
    std::cout << "--------- Accel Bias ------------\n";
    std::cout << std::dec << accelBias_[0] << std::endl;
    std::cout << std::dec << accelBias_[1] << std::endl;
    std::cout << std::dec << accelBias_[2] << std::endl;

    std::cout << "--------- gyro Bias ------------\n";
    std::cout << std::dec << gyroBias_[0] << std::endl;
    std::cout << std::dec << gyroBias_[1] << std::endl;
    std::cout << std::dec << gyroBias_[2] << std::endl;
    std::cout << "---------------" << std::endl;

    initIMU();
  }

  void MPU9250::verifyIMUConnected()
  {
    try {
      std::cout << "Opening\n";
      hwInterface_->open();
      std::cout << "Opened\n";

      uint8_t IAmMPU;
      IAmMPU = hwInterface_->readI2CByte(0x75); //IMU
      std::cout << "Who is mpu?\n";
      std::cout << "I am: " << std::hex << (int) IAmMPU << std::endl;

      if (IAmMPU != 0x71)
        throw std::runtime_error("MPU who am I failed");

      /*uint8_t IAmAK;
      hwInterface_->setDeviceAddress(0x76);
      IAmAK = hwInterface_->readI2CByte(0x00); //MAG
      std::cout << "Who is AK?\n";
      std::cout << "I am: " << std::hex << (int) IAmAK << std::endl;

      if (IAmAK != mpu9150::WHO_AM_I_AK8975A)
        throw std::runtime_error("AK who am I failed");*/
    }
    catch (std::runtime_error err) {
      std::cerr << err.what() << std::endl;
      hwInterface_->close();
      throw err;
    }
  }

  void MPU9250::resetIMU()
  {
    // reset device
    hwInterface_->writeByte(mpu9150::PWR_MGMT_1, 0x80); // Write a one to bit 7 reset bit; toggle reset device
    usleep(100000);
  }

  void MPU9250::initIMU()
  {
    // Initialize MPU9250 device
    // wake up device
     hwInterface_->writeByte(mpu9150::PWR_MGMT_1, 0x00); // Clear sleep mode bit (6), enable all sensors
     usleep(100000); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt

    // get stable time source
     hwInterface_->writeByte(mpu9150::PWR_MGMT_1, 0x01);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001

    // Configure Gyro and Accelerometer
    // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively;
    // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
    // Maximum delay is 4.9 ms which is just over a 200 Hz maximum rate
     hwInterface_->writeByte(mpu9150::CONFIG, 0x03);

    // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
     hwInterface_->writeByte(mpu9150::SMPLRT_DIV, 0x04);  // Use a 200 Hz rate; the same rate set in CONFIG above

    // Set gyroscope full scale range
    // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
     uint8_t c = hwInterface_->readI2CByte(mpu9150::GYRO_CONFIG); // get current GYRO_CONFIG register value
    // c = c & ~0xE0; // Clear self-test bits [7:5]
     c = c & ~0x02; // Clear Fchoice bits [1:0]
     c = c & ~0x18; // Clear AFS bits [4:3]
     c = c | (0x00 << 3); // Set full scale range for the gyro

    // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
     hwInterface_->writeByte(mpu9150::GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

    // Set accelerometer full-scale range configuration
     c = hwInterface_->readI2CByte(mpu9150::ACCEL_CONFIG); // get current ACCEL_CONFIG register value
    // c = c & ~0xE0; // Clear self-test bits [7:5]
     c = c & ~0x18;  // Clear AFS bits [4:3]
     c = c | (0x00 << 3); // Set full scale range for the gyro

     hwInterface_->writeByte(mpu9150::ACCEL_CONFIG, c); // Write new ACCEL_CONFIG register value

    // Set accelerometer sample rate configuration
    // It is possible to get a 4 kHz sample rate from the accelerometer by choosing 1 for
    // accel_fchoice_b bit [3]; in this case the bandwidth is 1.13 kHz
     c = hwInterface_->readI2CByte(mpu9150::ACCEL_CONFIG2); // get current ACCEL_CONFIG2 register value
     c = c & ~0x0F; // Clear accel_fchoice_b (bit 3) and A_DLPFG (bits [2:0])
     c = c | 0x03;  // Set accelerometer rate to 1 kHz and bandwidth to 41 Hz
     hwInterface_->writeByte(mpu9150::ACCEL_CONFIG2, c); // Write new ACCEL_CONFIG2 register value

    // The accelerometer, gyro, and thermometer are set to 1 kHz sample rates,
    // but all these rates are further reduced by a factor of 5 to 200 Hz because of the SMPLRT_DIV setting

     // Configure Interrupts and Bypass Enable
     // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips
     // can join the I2C bus and all can be controlled by the Arduino as master
      hwInterface_->writeByte(mpu9150::INT_PIN_CFG, 0x22);
      hwInterface_->writeByte(mpu9150::INT_ENABLE, 0x01);  // Enable data ready (bit 0) interrupt
  }

  void MPU9250::calculateBias()
  {
    uint8_t data[6] = {0,0,0,0,0,0};
    int32_t gyro_sum[3] = {0};
    int32_t accel_sum[3] = {0};
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};
    numSamples_ = 200;
    for (int i = 0; i < numSamples_; i++)
    {
      hwInterface_->readI2CBlock( mpu9150::ACCEL_XOUT_H, &data[0], 6);  // Read the six raw data registers into data array
      int16_t accelOutput[3];
      accelOutput[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
      accelOutput[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]) ;
      accelOutput[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]) ;
      accel_sum[0] += (int32_t) accelOutput[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
      accel_sum[1] += (int32_t) accelOutput[1];
      accel_sum[2] += (int32_t) accelOutput[2];
      std::cout << "gyro calib output x:  " << accelOutput[0] << std::endl;
      std::cout << "gyro calib output y:  " << accelOutput[1] << std::endl;
      std::cout << "gyro calib output z:  " << accelOutput[2] << std::endl;

      hwInterface_->readI2CBlock( mpu9150::GYRO_XOUT_H, &data[0], 6);  // Read the six raw data registers sequentially into data array
      int16_t gyroOutput[3];
      gyroOutput[0] = (int16_t)(((int16_t)data[0] << 8) | data[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
      gyroOutput[1] = (int16_t)(((int16_t)data[2] << 8) | data[3]) ;
      gyroOutput[2] = (int16_t)(((int16_t)data[4] << 8) | data[5]) ;
      std::cout << "gyro calib output x:  " << gyroOutput[0] << std::endl;
      std::cout << "gyro calib output y:  " << gyroOutput[1] << std::endl;
      std::cout << "gyro calib output z:  " << gyroOutput[2] << std::endl;

      gyro_sum[0]  += (int32_t) gyroOutput[0];
      gyro_sum[1]  += (int32_t) gyroOutput[1];
      gyro_sum[2]  += (int32_t) gyroOutput[2];
      std::cout << "----------------" << std::endl;
    }
    uint16_t accelOffsets[3], gyroOffsets[3];


    std::cout << "Num samples: " << std::dec << numSamples_ << std::endl;
    accelOffset_[0] = (int16_t) (accel_sum[0]/(int32_t)numSamples_);
    accelOffset_[1] = (int16_t) (accel_sum[1]/(int32_t)numSamples_);
    accelOffset_[2] = (int16_t) (accel_sum[2]/(int32_t)numSamples_);
    std::cout << "Ascale: " << aScale_ << std::endl;

    std::cout << "accel offset x: " << std::hex << accelOffsets[0] << std::endl;
    std::cout << "accel offset y: " << std::hex << accelOffsets[1] << std::endl;
    std::cout << "accel offset z: " << std::hex << accelOffsets[2] << std::endl;
    accelBias_[0] = (float) accelOffsets[0]*(float) aScale_;
    accelBias_[1] = (float) accelOffsets[1]*(float) aScale_;
    accelBias_[2] = (float) accelOffsets[2]*(float) aScale_;

    // average out the samples
    gyroOffset_[0] = (int16_t) (gyro_sum[0]/(int32_t)numSamples_);
    gyroOffset_[1] = (int16_t) (gyro_sum[1]/(int32_t)numSamples_);
    gyroOffset_[2] = (int16_t) (gyro_sum[2]/(int32_t)numSamples_);
    std::cout << "Gscale: " << gScale_ << std::endl;


    std::cout << "gyro offset x: " << std::hex << gyroOffset_[0] << std::endl;
    std::cout << "gyro offset y: " << std::hex << gyroOffset_[1] << std::endl;
    std::cout << "gyro offset z: " << std::hex << gyroOffset_[2] << std::endl;

    gyroBias_[0] = (float) gyroOffsets[0]*(float) gScale_;
    gyroBias_[1] = (float) gyroOffsets[1]*(float) gScale_;
    gyroBias_[2] = (float) gyroOffsets[2]*(float) gScale_;
  }
}


