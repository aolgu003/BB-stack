#include "imuTest.h"

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

  uint8_t data[6] = {0x01,0};
  EXPECT_CALL(hwMock, readI2CBlock(sensor::mpu9150::FIFO_COUNTH, NotNull(), 2))
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
