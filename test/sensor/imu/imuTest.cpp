#include "imuTest.h"

void imuTest::SetUp(){}
void imuTest::TearDown(){}

TEST_F(imuTest, MPU9250_TEST)
{
  sensor::HALMock hwMock;
  sensor::MPU9250 myIMU(&hwMock);
}
