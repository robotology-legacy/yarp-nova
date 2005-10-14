
// this header file depends on ACE, but does not need to be released
// in binary distribution of library

#ifndef NOVA_STREAM_H
#define NOVA_STREAM_H

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"


class NovaStream {
public:
  ACE_SOCK_Stream client_stream_;
  ACE_INET_Addr remote_addr_;

  ACE_SOCK_Stream& getStream() { return client_stream_; }
  ACE_INET_Addr& getAddr() { return remote_addr_; }
};

#endif
