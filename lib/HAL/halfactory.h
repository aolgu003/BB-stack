#ifndef HALFACTORY_H
#define HALFACTORY_H
#include "HALInterface.h"
#include "blacklibwrapper.h"

namespace sensor {
  class HALFactory
  {
  public:
    static HALInterface *makeHALInstance( hwInterface halSelection, int i2cdevice, unsigned int i2caddress );

  };
}

#endif // HALFACTORY_H
