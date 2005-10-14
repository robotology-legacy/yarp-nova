
#ifndef NOVA_TELNET_CLIENT_H
#define NOVA_TELNET_CLIENT_H

#include "NovaClient.h"

#define MAX_TELNET_BUFFER 512

class NovaTelnetClient : public NovaClient {

public:
  NovaTelnetClient();

  int sendText(const char *text);
  const char *receiveText(double timeout = -1);

private:
  char current[MAX_TELNET_BUFFER];
  int at, len;
};

#endif
