#include "imuTest.h"
#include "imu/mpu9250/mpu9250.h"

void imuTest::SetUp(){}
void imuTest::TearDown(){}

using ::testing::Return;
using ::testing::_;
using ::testing::SetArrayArgument;
using ::testing::NotNull;

TEST_F(imuTest, MPU9250_START_UP_SUCCESS_TEST)
{
  sensor::HALMock hwMock;
  EXPECT_CALL(hwMock, open())
      .Times(1);

  EXPECT_CALL(hwMock, readI2CByte(_))
      .Times(2)
      .WillOnce(Return(sensor::mpu9150::WHO_AM_I_MPU9150))
      .WillOnce(Return(sensor::mpu9150::WHO_AM_I_AK8975A));
  EXPECT_CALL(hwMock, writeByte(_,_))
      .Times(14);
  EXPECT_CALL(hwMock, writeByte(sensor::mpu9150::FIFO_EN, 0x70))
      .Times(1);

  uint8_t data[6] = {0x00,0x06};
  EXPECT_CALL(hwMock, readI2CBlock(sensor::mpu9150::FIFO_COUNTH, NotNull(), 2))
    .Times(1)
    .WillOnce(SetArrayArgument<1>(data, data+6));

  int16_t fifo_count = ((uint16_t)data[0] << 8) | data[1];
  int16_t numSamples = fifo_count/6;
  EXPECT_EQ(numSamples, 1);

  data[0] = 0x00;
  data[1] = 0x06;
  data[2] = 0x00;
  data[3] = 0x06;
  data[4] = 0x00;
  data[5] = 0x06;
  EXPECT_CALL(hwMock, readI2CBlock(sensor::mpu9150::FIFO_R_W, NotNull(), 6))
    .Times(numSamples)
    .WillOnce(SetArrayArgument<1>(data, data+6));

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
