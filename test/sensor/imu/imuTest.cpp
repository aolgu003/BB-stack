#include "imuTest.h"
#include "imu/mpu9250/mpu9250.h"

void imuTest::SetUp()
{
}

void imuTest::TearDown(){}

using ::testing::Return;
using ::testing::_;
using ::testing::SetArrayArgument;
using ::testing::NotNull;
using ::testing::AtLeast;

TEST_F(imuTest, MPU9250_START_UP_SUCCESS_TEST)
{
  sensor::HALMock hwMock;
  EXPECT_CALL(hwMock, open())
      .Times(AtLeast(1));

  EXPECT_CALL(hwMock, readI2CByte(_))
      .Times(AtLeast(1))
      .WillOnce(Return(sensor::mpu9150::WHO_AM_I_MPU9150));
  EXPECT_CALL(hwMock, writeByte(_,_))
      .Times(AtLeast(1));

  EXPECT_NO_THROW(
  {sensor::MPU9250 myIMU(&hwMock);}
        );
}

TEST_F(imuTest, IMU_FAILURE_TEST)
{
  sensor::HALMock hwMock;
  EXPECT_CALL(hwMock, open())
      .Times(1);

  EXPECT_CALL(hwMock, readI2CByte(_))
      .Times(1)
      .WillOnce(Return(sensor::mpu9150::WHO_AM_I_AK8975A));

  EXPECT_ANY_THROW(
  {sensor::MPU9250 myIMU(&hwMock);}
        );
}

// TODO remove redundant code (move to setup?)
// add readI@CBlock call for calibration phase and readData phase
TEST_F(imuTest, MPU9250_READ_DATA)
{
  sensor::HALMock hwMock;

  EXPECT_CALL(hwMock, open())
      .Times(AtLeast(1));

  EXPECT_CALL(hwMock, readI2CByte(_))
      .Times(AtLeast(1))
      .WillOnce(Return(sensor::mpu9150::WHO_AM_I_MPU9150));

  EXPECT_CALL(hwMock, writeByte(_,_))
      .Times(AtLeast(1));

  uint8_t accelData[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  EXPECT_CALL(hwMock, readI2CBlock( sensor::mpu9150::ACCEL_XOUT_H, _, 6 ))
      .WillOnce(SetArrayArgument<1>(accelData, accelData+6) )
      .Times(AtLeast(1));

  uint8_t gyroData[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  EXPECT_CALL(hwMock, readI2CBlock( sensor::mpu9150::GYRO_XOUT_H, _, 6 ))
      .WillOnce(SetArrayArgument<1>(gyroData, gyroData+6) )
      .Times(AtLeast(1));

  sensor::MPU9250 myIMU(&hwMock);
  sensor::imuData imuOutput = myIMU.readData();

  EXPECT_EQ(imuOutput.a[0], -2);
  EXPECT_EQ(imuOutput.w[0], -2);

}
