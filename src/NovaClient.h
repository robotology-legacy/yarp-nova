
#ifndef NOVA_CLIENT_H
#define NOVA_CLIENT_H

class NovaServer;
class NovaStream;

class NovaClient {
public:
  NovaClient();
  virtual ~NovaClient();

  void connect(const char *hostname, int port);
  void connect(NovaServer& server);

  int send(const char *data, int len);

  int receive(char *data, int len, double timeout = -1);

private:
  void *system_resource;
  NovaStream& getStream();
  friend class NovaServer;
};

#endif
