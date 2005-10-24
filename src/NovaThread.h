
#ifndef NOVA_THREAD_H
#define NOVA_THREAD_H

class NovaThread {

public:
  NovaThread();
  virtual ~NovaThread();

  virtual void main();

  void begin();
  void end(double grace_period = -1);
  bool isEnding();

private:
  void *system_resource;

};


#endif

