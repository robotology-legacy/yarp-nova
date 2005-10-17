
#include "NovaThread.h"

#include <yarp/YARPThread.h>
#include <assert.h>

class YARPNovaThread : public YARPThread {
public:
  NovaThread& nova;

  YARPNovaThread(NovaThread& the_nova) :
    nova(the_nova) 
  {}

  virtual void Body() {
    nova.main();
  }
};

#define SYS(x) (*((YARPNovaThread *)(x)))

NovaThread::NovaThread() {
  system_resource = NULL;
  system_resource = new YARPNovaThread(*this);
  assert(system_resource!=NULL);
}

NovaThread::~NovaThread() {
  if (system_resource!=NULL) {
    delete &SYS(system_resource);
    system_resource = NULL;
  }
}

void NovaThread::main() {
}

void NovaThread::begin() {
  SYS(system_resource).Begin();
}


void NovaThread::end(double grace_period) {
  int millisec = (int)(grace_period*1000);
  if (millisec<0) {
    millisec = -1;
  }
  SYS(system_resource).End(millisec);
}


bool NovaThread::isEnding() {
  int result = SYS(system_resource).IsTerminated();
//  printf("[%d]\n", result);
  return (result!=0);
}

