#ifndef IMUTEST_H
#define IMUTEST_H
#include <gtest/gtest.h>
#include "imu.h"

class imuTest : public ::testing::Test
{
protected:
  virtual void SetUp();
  virtual void TearDown();

protected:

};

#endif // IMUTEST_H
