//// Based on an example from the ACE Programmers Guide.
//// Chapter: "IPC SAP" (Interprocess Communication Mechanisms in ACE).
//// original AUTHOR: Umar Syyid (usy...@hns.com)
//// and Ambreen Ilyas (ambr...@bitsmart.com)


#include <assert.h>

#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>
#include <ace/Time_Value.h>

#include "NovaClient.h"
#include "NovaStream.h"
#include "NovaServer.h"

#define SYS(x) (*((NovaClientHelper *)(x)))


class NovaClientHelper : public NovaStream {
public:
  void connect(const char *hostname, int port) {
    remote_addr_.set(hostname);
    remote_addr_.set_port_number(port);
    connect_to_server();
  }

  //Uses a connector component connector_ to connect to a remote machine
  //and pass the connection into a stream component client_stream_
  int connect_to_server(){
    // Initiate blocking connection with server.
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Starting connect to %s:%d\n",
		remote_addr_.get_host_name(),remote_addr_.get_port_number()));
    if (connector_.connect (client_stream_, remote_addr_) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","connection failed"),-1);
    else
      ACE_DEBUG ((LM_DEBUG,"(%P|%t) connected to %s\n",
		  remote_addr_.get_host_name ()));
    return 0;
  }

  int send(const char *data, int len) {
    return client_stream_.send_n(data, len, 0);
  }

  int receive(char *data, int len, double timeout = -1) {
    if (timeout<=-0.5) {
      return client_stream_.recv(data, len, 0);
    } else {
      ACE_Time_Value ace_timeout(0,(long int)(timeout*1e6));
      return client_stream_.recv(data, len, 0, &ace_timeout);
    }
  }
  
  //Close down the connection properly.
  int close(){
    if (client_stream_.close () == -1)
      ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","close"),-1);
    else
      return 0;
  }

private:
  ACE_SOCK_Connector connector_;
};




NovaClient::NovaClient() {
  system_resource = NULL;
  system_resource = new NovaClientHelper;
}

NovaClient::~NovaClient() {
  if (system_resource!=NULL) {
    delete &SYS(system_resource);
    system_resource = NULL;
  }
}

void NovaClient::connect(const char *hostname, int port) {
  assert(system_resource!=NULL);
  SYS(system_resource).connect(hostname,port);
}


int NovaClient::send(const char *data, int len) {
  assert(system_resource!=NULL);
  return SYS(system_resource).send(data,len);
}

int NovaClient::receive(char *data, int len, double timeout) {
  assert(system_resource!=NULL);
  return SYS(system_resource).receive(data,len,timeout);
}

void NovaClient::connect(NovaServer& server) {
  server.accept(*this);
}

NovaStream& NovaClient::getStream() {
  assert(system_resource!=NULL);
  return SYS(system_resource);
}

