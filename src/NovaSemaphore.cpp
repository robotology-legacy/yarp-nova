
#include "NovaSemaphore.h"

#include <yarp/YARPSemaphore.h>
#include <assert.h>

#define SYS(x) (*((YARPSemaphore *)(x)))

NovaSemaphore::NovaSemaphore(int initial_count) {
  system_resource = NULL;
  system_resource = new YARPSemaphore(initial_count);
  assert(system_resource!=NULL);
}


NovaSemaphore::~NovaSemaphore() {
  if (system_resource!=NULL) {
    delete &SYS(system_resource);
    system_resource = NULL;
  }
}


void NovaSemaphore::wait() {
  SYS(system_resource).Wait();
}


void NovaSemaphore::post() {
  SYS(system_resource).Post();
}


bool NovaSemaphore::check() {
  int result = SYS(system_resource).Wait(0);
  return (result!=0);
}

