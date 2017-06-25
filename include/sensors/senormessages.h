#ifndef SENORMESSAGES_H
#define SENORMESSAGES_H
#include "sensor-template.h"

using namespace sensor
{
struct imuData
{
  double q[4];
  double euler[3]; // 0->y, 1->p, 2->r
  double w[3];
  double a[3];
};
}

#endif // SENORMESSAGES_H
