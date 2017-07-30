#include "mpu9250.h"
#include <bitset>

//TODO Write calibration code
//TODO Write calibration parameter saving
//TODO Write calibration parameter loading
//TODO Write readData function
namespace sensor {
  MPU9250::MPU9250(HALInterface* hwSelection):
    hwInterface_(hwSelection),
    aScale_(2000.0/32768.0),
    gScale_(16.0/32768.0)
  {
    std::cout << ".....Verifying IMU Connected....." << std::endl;
    verifyIMUConnected();
    std::cout << ".....IMU Connected.....\n" << std::endl;

    // Calibrate gyro
    std::cout << ".....Reseting IMU ....." << std::endl;
    resetIMU();
    std::cout << "\n.....Setting up Calibration ....." << std::endl;
    calibrationSetup();
    std::cout << "\n.....Collecting Calibrating Data ....." << std::endl;
    collectCalibrationData();
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

    initIMU();
    usleep(2000000);
    //initIMU();
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
    std::cout << std::hex << accelOutput[0] << std::endl;
    std::cout << std::hex << accelOutput[1] << std::endl;
    std::cout << std::hex << accelOutput[2] << std::endl;

    output.a[0] = ((float)accelOutput[0])*aScale_-accelBias_[0];
    output.a[1] = ((float)accelOutput[1])*aScale_-accelBias_[1];
    output.a[2] = ((float)accelOutput[2])*aScale_-accelBias_[2];
    std::cout << "---- Processed Data: " << std::endl;
    std::cout << output.a[0] << std::endl;
    std::cout << output.a[1] << std::endl;
    std::cout << output.a[2] << std::endl;

    hwInterface_->readI2CBlock( mpu9150::GYRO_XOUT_H, &rawData[0], 6);  // Read the six raw data registers sequentially into data array
    int16_t gyroOutput[3];
    gyroOutput[0] = (int16_t)(((int16_t)rawData[0] << 8) | rawData[1]) ;  // Turn the MSB and LSB into a signed 16-bit value
    gyroOutput[1] = (int16_t)(((int16_t)rawData[2] << 8) | rawData[3]) ;
    gyroOutput[2] = (int16_t)(((int16_t)rawData[4] << 8) | rawData[5]) ;
    output.w[0] = ((float) gyroOutput[0])*gScale_-gyroBias_[0];
    output.w[1] = ((float) gyroOutput[1])*gScale_-gyroBias_[1];
    output.w[2] = ((float) gyroOutput[2])*gScale_-gyroBias_[2];
    std::cout << "GYRO Data \n";
    std::cout << output.w[0] << std::endl;
    std::cout << output.w[1] << std::endl;
    std::cout << output.w[2] << std::endl;
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
     c = c | (0x03 << 3); // Set full scale range for the gyro

    // c =| 0x00; // Set Fchoice for the gyro to 11 by writing its inverse to bits 1:0 of GYRO_CONFIG
     hwInterface_->writeByte(mpu9150::GYRO_CONFIG, c ); // Write new GYRO_CONFIG value to register

    // Set accelerometer full-scale range configuration
     c = hwInterface_->readI2CByte(mpu9150::ACCEL_CONFIG); // get current ACCEL_CONFIG register value
    // c = c & ~0xE0; // Clear self-test bits [7:5]
     c = c & ~0x18;  // Clear AFS bits [4:3]
     c = c | (0x03 << 3); // Set full scale range for the gyro

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
      hwInterface_->writeByte(mpu9150::SMPLRT_DIV, 0x00);  // Set sample rate to 200hz
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
    uint8_t c = mpu9150::FIFO_EN_GYROX|mpu9150::FIFO_EN_GYROY|mpu9150::FIFO_EN_GYROZ | mpu9150::FIFO_EN_ACCEL;
    hwInterface_->writeByte(mpu9150::FIFO_EN, c);
    // 6 bytes per sample. 200hz. wait 0.4 seconds
    usleep(400000);

    // At end of sample accumulation, turn off FIFO sensor read
    hwInterface_->writeByte(mpu9150::FIFO_EN, 0x00);

    // read FIFO sample count and log number of samples
    uint8_t data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    hwInterface_->readI2CBlock( mpu9150::FIFO_COUNTH, &data[0], 2 );
    int16_t fifo_count = ((uint16_t)data[0] << 8) | data[1];
    numSamples_ = fifo_count/12;
  }

  void MPU9250::calculateBias()
  {
    uint8_t data[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
    int32_t gyro_sum[3] = {0};
    int32_t accel_sum[3] = {0};
    int16_t accel_temp[3] = {0, 0, 0}, gyro_temp[3] = {0, 0, 0};

    for (int i = 0; i < numSamples_; i++)
    {
        hwInterface_->readI2CBlock( mpu9150::FIFO_R_W, &data[0], 12 );
        accel_temp[0] = (int16_t) (((int16_t)data[0] << 8) | data[1]  ) ;  // Form signed 16-bit integer for each sample in FIFO
        std::cout << "Raw Data x: " << std::dec << (int) data[0] << std::endl;
        std::cout << std::dec << (int) data[1] << std::endl;
        std::cout << "Accel temp x: " << std::hex << (int16_t) accel_temp[0] << std::endl;


        accel_temp[1] = (int16_t) (((int16_t)data[2] << 8) | data[3]  ) ;
        accel_temp[2] = (int16_t) (((int16_t)data[4] << 8) | data[5]  ) ;
        gyro_temp[0]  = (int16_t) (((int16_t)data[6] << 8) | data[7]  ) ;
        gyro_temp[1]  = (int16_t) (((int16_t)data[8] << 8) | data[9]  ) ;
        gyro_temp[2]  = (int16_t) (((int16_t)data[10] << 8) | data[11]) ;

        accel_sum[0] += (int32_t) accel_temp[0]; // Sum individual signed 16-bit biases to get accumulated signed 32-bit biases
        accel_sum[1] += (int32_t) accel_temp[1];
        accel_sum[2] += (int32_t) accel_temp[2];

        gyro_sum[0]  += (int32_t) gyro_temp[0];
        gyro_sum[1]  += (int32_t) gyro_temp[1];
        gyro_sum[2]  += (int32_t) gyro_temp[2];
        std::cout << "--------------------" << std::endl;
    }
    uint16_t accelOffsets[3], gyroOffsets[3];


    std::cout << "Num samples: " << numSamples_ << std::endl;
    // average out the samples
    gyroOffsets[0] = (int16_t) (gyro_sum[0]/(int32_t)numSamples_);
    gyroOffsets[1] = (int16_t) (gyro_sum[1]/(int32_t)numSamples_);
    gyroOffsets[2] = (int16_t) (gyro_sum[2]/(int32_t)numSamples_);
    std::cout << "Ascale: " << aScale_ << std::endl;

    uint16_t  gyrosensitivity  = 131;   // = 131 LSB/degrees/sec
    uint16_t  accelsensitivity = 16384;  // = 16384 LSB/g

    gyroBias_[0] = (float) gyroOffsets[0]/(float) gyrosensitivity;
    gyroBias_[1] = (float) gyroOffsets[1]/(float) gyrosensitivity;
    gyroBias_[2] = (float) gyroOffsets[2]/(float) gyrosensitivity;

    accelOffsets[0] = (int16_t) (accel_sum[0]/(int32_t)numSamples_);
    accelOffsets[1] = (int16_t) (accel_sum[1]/(int32_t)numSamples_);
    accelOffsets[2] = (int16_t) (accel_sum[2]/(int32_t)numSamples_);

    std::cout << "accel offset x: " << accelOffsets[0] << std::endl;
    std::cout << "accel offset y: " << accelOffsets[1] << std::endl;
    std::cout << "accel offset z: " << accelOffsets[2] << std::endl;
    accelBias_[0] = (float) accelOffsets[0]/(float) accelsensitivity;
    accelBias_[1] = (float) accelOffsets[1]/(float) accelsensitivity;
    accelBias_[2] = (float) accelOffsets[2]/(float) accelsensitivity;
  }
}


