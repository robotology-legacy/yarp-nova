
#include <stdio.h>
#include <stdlib.h>

#include "Nova.h"



class ClientThread : public NovaThread {
private:

  NovaTelnetClient client;
  bool active;

public:

  ClientThread() {
    active = false;
  }

  bool isActive() {
    return active;
  }

  void accept(NovaServer& server) {
    active = true;
    server.accept(client);
    begin();
  }

  virtual void main() {
    bool done = false;
    while (!done) {
      const char *str = client.receiveText();
      if (str!=NULL) {
	client.sendText(str);
      } else {
	done = 1;
      }
    }
    active = false;
  }
};


#define MAX_CLIENTS 100
ClientThread *clients[MAX_CLIENTS];


void runServer(int port) {
  NovaInit::init();
  
  for (int i=0; i<MAX_CLIENTS; i++) {
    clients[i] = NULL;
  }
  NovaServer server;
  if (server.begin(port)!=0) {
    printf("Cannot start server up; may need to wait a little while\n");
    printf("if it was recently killed\n");
    exit(1);
  }
  while (1) {
    printf("Waiting...\n");
    for (int i=0; i<MAX_CLIENTS; i++) {
      if (clients[i]!=NULL) {
	if (!clients[i]->isActive()) {
	  delete clients[i];
	  clients[i] = NULL;
	}
      }
      if (clients[i] == NULL) {
	clients[i] = new ClientThread;
	clients[i]->accept(server);
	printf("Started server [%d]\n", i);
	break;
      }
    }
  }
}

int main() {
  runServer(9999);
  return 0;
}

