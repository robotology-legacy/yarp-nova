
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Nova.h"

#include <ace/Hash_Map_Manager.h>
#include <ace/Lock.h>
#include <ace/Null_Mutex.h>

NovaSemaphore client_mutex(1);

class ClientThread;
void addClient(ClientThread& client);
void removeClient(ClientThread& client);
void broadcast(const char *str);

class ClientThread : public NovaThread {
private:

  NovaTelnetClient client;
  bool active;
  int id;
  NovaSemaphore mutex;

public:

  ClientThread(int nid) : mutex(1) {
    active = false;
    id = nid;
  }

  int getID() {
    return id;
  }

  bool isActive() {
    return active;
  }

  void accept(NovaServer& server) {
    client_mutex.wait();
    active = true;
    client_mutex.post();
    server.accept(client);
    begin();
  }

  virtual void main() {
    addClient(*this);
    bool done = false;
    while (!done) {
      const char *str = client.receiveText();
      if (str!=NULL) {
	//mutex.wait();
	//client.sendText(str);
	//mutex.post();
	broadcast(str);
      } else {
	done = 1;
      }
    }
    removeClient(*this);
    client_mutex.wait();
    active = false;
    client_mutex.post();
  }

  void send(const char *str) {
    mutex.wait();
    client.sendText(str);
    mutex.post();
  }
};


#define MAX_CLIENTS 100


class ClientID {
public:
  int x;
  ClientID() { x = 0; }
  ClientID(int nx) { x = nx; }
  bool operator == (const ClientID& foo) const {
    return x == foo.x;
  }
  int hash() const {
    return x;
  }
};


class ClientManager {
private:
  ClientThread *clients[MAX_CLIENTS];
  ACE_Hash_Map_Manager<ClientID,int,ACE_Null_Mutex> map;
  int client_ct;

public:

  void addClient(ClientThread& client) {
    client_mutex.wait();
    map.bind(ClientID(client.getID()),client.getID());
    client_ct++;
    client_mutex.post();
    client.send("");
    client.send("Hello and welcome to the chat server");
    client.send("Whatever you type is copied to everyone logged in");
    char buf[256];
    sprintf(buf,"Number of people logged in: %d", client_ct);
    client.send(buf);
  }
  
  void removeClient(ClientThread& client) {
    client_mutex.wait();
    map.unbind(ClientID(client.getID()));
    client_ct--;
    client_mutex.post();
  }

  void broadcast(const char *str) {
    client_mutex.wait();
    ACE_Hash_Map_Iterator<ClientID,int,ACE_Null_Mutex> it(map);
    for (it=map.begin(); it!=map.end(); it++) {
      int id = (*it).int_id_;
      printf("Say %d %s\n", id, str);
      assert(clients[id]!=NULL);
      clients[id]->send(str);
    }
    client_mutex.post();
  }


  void runServer(int port) {
    NovaInit::init();
    client_ct = 0;

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
	  clients[i] = new ClientThread(i);
	  clients[i]->accept(server);
	  printf("Started server [%d]\n", i);
	  break;
	}
      }
    }
  }
};


ClientManager man;

void addClient(ClientThread& client) {
  man.addClient(client);
}

void removeClient(ClientThread& client) {
  man.removeClient(client);
}

void broadcast(const char *str) {
  man.broadcast(str);
}


int main() {
  man.runServer(9999);
  return 0;
}

