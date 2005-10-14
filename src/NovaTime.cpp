
#include "NovaTime.h"

#include <yarp/YARPTime.h>

double NovaTime::now() {
  return YARPTime::GetTimeAsSeconds();
}

void NovaTime::sleep(double seconds) {
  YARPTime::DelayInSeconds(seconds);
}
