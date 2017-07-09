#ifndef SENORMESSAGES_H
#define SENORMESSAGES_H
#include "sensor-template.h"

namespace sensor
{
struct imuData
{
  float q[4];
  float euler[3]; // 0->y, 1->p, 2->r
  float w[3];
  float a[3];
  float m[3];
};
}

#endif // SENORMESSAGES_H
