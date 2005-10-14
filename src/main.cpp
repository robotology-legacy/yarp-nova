#include <stdio.h>
#include <assert.h>

#include "NovaSemaphore.h"
#include "NovaThread.h"
#include "NovaTime.h"

int count = 0;

class Thread1 : public NovaThread {
public:
  virtual void main() {
    for (int i=0; i<300 && !isEnding(); i++) {
      count++;
      printf("Count is %d\n", count);
      NovaTime::sleep(1);
    }
  }
};

void testThread() {
  Thread1 t1;
  count = 0;
  assert(count==0);
  t1.begin();
  NovaTime::sleep(4);
  t1.end(2);
  /* YARP currently has a not-so-nice implementation of this */
  assert(count>=3&&count<=5);
}

void testSemaphore() {
  NovaSemaphore s1(1);
  assert(s1.check()==1);
  assert(s1.check()==0);
  s1.post();
  assert(s1.check()==1);
  assert(s1.check()==0);
}

int main() {
  printf("Hello world\n");
  testSemaphore();
  testThread();
  return 0;
}

