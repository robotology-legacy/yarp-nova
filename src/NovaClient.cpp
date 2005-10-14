
#include <assert.h>

#include "NovaClient.h"
#include "NovaStream.h"

#define SYS(x) (*((NovaClientHelper *)(x)))













/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// This example is from the ACE Programmers Guide.
////  Chapter: "IPC SAP" (Interprocess Communication Mechanisms in ACE).
//// For details please see the guide at
//// http://www.cs.wustl.edu/~schmidt/ACE.html
////  AUTHOR: Umar Syyid (usy...@hns.com)
//// and Ambreen Ilyas (ambr...@bitsmart.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Example 2
#include <ace/SOCK_Connector.h>
#include <ace/INET_Addr.h>
#include <ace/Log_Msg.h>
#define SIZE_BUF 128
#define NO_ITERATIONS 5


class NovaClientHelper : public NovaStream {
public:
  void connect(const char *hostname, int port) {
    remote_addr_.set(hostname);
    remote_addr_.set_port_number(port);
    data_buf_=new char[SIZE_BUF];
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

  //Uses a stream component to send data to the remote host.
  int send_to_server(){
    // Send data to server
    ACE_OS::sprintf(data_buf_,"Hello from Client");
    for(int i=0;i<NO_ITERATIONS; i++){
      if (send
	  (data_buf_, ACE_OS::strlen(data_buf_)+1) == -1){
	ACE_ERROR_RETURN ((LM_ERROR,"(%P|%t) %p\n","send_n"),0);
	break;
      }
    }
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
  char *data_buf_;
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
  assert(0==1);
}

NovaStream& NovaClient::getStream() {
  assert(system_resource!=NULL);
  return SYS(system_resource);
}

