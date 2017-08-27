#include "halfactory.h"


namespace sensor {
  HALInterface *HALFactory::makeHALInstance(hwInterface halSelection, int i2cdevice, unsigned int i2caddress)
  {
    switch (halSelection) {
    case BlackLibInterface:
      return new BlackLibWrapper( (BlackLib::i2cName) i2cdevice, i2caddress);
    default:
      break;
    }
  }

}
