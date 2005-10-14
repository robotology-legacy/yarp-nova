

#include <assert.h>

#include "NovaServer.h"
#include "NovaStream.h"






////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// This example is from the ACE Programmers Guide.
////  Chapter: "IPC SAP" (Interprocess Communication Mechanisms in ACE).
//// For details please see the guide at
//// http://www.cs.wustl.edu/~schmidt/ACE.html
////  AUTHOR: Umar Syyid (usy...@hns.com)
//// and Ambreen Ilyas (ambr...@bitsmart.com)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Example 1
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/Log_Msg.h"
#define SIZE_DATA 18
#define SIZE_BUF 1024

class NovaServerHelper{

public:
NovaServerHelper (int port):
 server_addr_(port),peer_acceptor_(server_addr_){
 data_buf_= new char[SIZE_BUF];
 }

//Handle the connection once it has been established.
//Here the connection is handled by reading SIZE_DATA amount of data
//from the remote and then closing the connection
//stream down.
int handle_connection(){
  // Read data from client
 if(new_stream_.recv_n (data_buf_, SIZE_DATA, 0)==-1)
  ACE_ERROR ((LM_ERROR, "%p\n", "Error in recv"));

 ACE_DEBUG((LM_DEBUG,"Server recieved %s \n",data_buf_));

 // Close new endpoint
 if (new_stream_.close () == -1)
  ACE_ERROR ((LM_ERROR, "%p\n", "close"));
 return 0;
}

//Use the acceptor component peer_acceptor_ to accept the connection
//into the underlying stream new_stream_. After the connection has been
//established call the handle_connenction() method.
int accept_connections (){
  int result = peer_acceptor_.get_local_addr (server_addr_);
  if (result!=0) {
    ACE_ERROR_RETURN ((LM_ERROR,"%p\n","Error in get_local_addr"),1);
    return result;
  }

  ACE_DEBUG ((LM_DEBUG,"Starting server at port %d\n",
	      server_addr_.get_port_number ()));
  return result;
}

int accept_connections (NovaStream& nova_stream){

    //ACE_Time_Value timeout (ACE_DEFAULT_TIMEOUT);
    if (peer_acceptor_.accept
	(nova_stream.getStream(), &nova_stream.getAddr())== -1){
      ACE_ERROR ((LM_ERROR, "%p\n", "accept"));
      return -1;
    }
    else {
      ACE_DEBUG((LM_DEBUG,
		 "Connection established with remote %s:%d\n",
		 nova_stream.getAddr().get_host_name(),nova_stream.getAddr().get_port_number()));
      //Handle the connection
      //handle_connection();
      return 0;
    }
}

private:
 char *data_buf_;
 ACE_INET_Addr server_addr_;
 ACE_INET_Addr client_addr_;
 ACE_SOCK_Acceptor peer_acceptor_;
 ACE_SOCK_Stream new_stream_;
 ACE_HANDLE newhandle;

};



#define SYS(x) (*((NovaServerHelper *)(x)))


NovaServer::NovaServer() {
  system_resource = NULL;
}


NovaServer::~NovaServer() {
  if (system_resource!=NULL) {
    delete &SYS(system_resource);
    system_resource = NULL;
  }
}


int NovaServer::begin(int port) {
  if (system_resource!=NULL) {
    delete &SYS(system_resource);
    system_resource = NULL;
  }
  assert(system_resource==NULL);
  system_resource = new NovaServerHelper(port);
  return SYS(system_resource).accept_connections();
}


void NovaServer::accept(NovaClient& client) {
  assert(system_resource!=NULL);
  SYS(system_resource).accept_connections(client.getStream());
}


