
#ifndef NOVA_SERVER_H
#define NOVA_SERVER_H

#include "NovaClient.h"

class NovaServer {
public:
  NovaServer();
  virtual ~NovaServer();

  void begin(int port);

  void accept(NovaClient& client);

private:
  void *system_resource;
};

#endif
