#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>

#include "Nova.h"
#include "Thing.h"

class Replier {
public:
  virtual void send(const char *msg) = 0;
};

class Player : public Replier {
public:
  Player() : mutex(1) {}

  void setReplier(Replier *n_replier) {
    mutex.wait();
    replier = n_replier;
    mutex.post();
  }
  void apply(const char *command);

  void apply(int argc, const char *argv[]);

  virtual void send(const char *msg) {
    reply(msg);
  }

private:
  Replier *replier;
  NovaSemaphore mutex;
  void reply(const char *msg) {
    mutex.wait();
    if (replier!=NULL) {
      replier->send(msg);
    }
    mutex.post();
  }


  ID id;
};

#endif
