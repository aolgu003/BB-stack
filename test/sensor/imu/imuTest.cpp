#include "imuTest.h"

void imuTest::SetUp(){}
void imuTest::TearDown(){}

TEST_F(imuTest, constructorTest)
{
  sensor::AHRS myAHRS(sensor::AHRS::select_imu("MPU9250") );

  EXPECT_NO_THROW(sensor::AHRS myAHRS(sensor::AHRS::select_imu("MPU9250") ););
}
