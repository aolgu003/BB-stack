#ifndef IMUTEST_H
#define IMUTEST_H
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "halmock.h"

class imuTest : public ::testing::Test
{
protected:
  virtual void SetUp();
  virtual void TearDown();

protected:

};

#endif // IMUTEST_H
