
#ifndef NOVA_SEMAPHORE_H
#define NOVA_SEMAPHORE_H

class NovaSemaphore {

public:
  NovaSemaphore(int initial_count = 0);
  virtual ~NovaSemaphore();

  void wait();
  void post();
  bool check();

private:
  void *system_resource;

};


#endif
