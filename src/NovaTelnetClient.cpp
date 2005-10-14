
#include "NovaTelnetClient.h"

#include <string.h>
#include <stdio.h>

NovaTelnetClient::NovaTelnetClient() {
  at = 0;
  len = 0;
}

int NovaTelnetClient::sendText(const char *text) {
  int result = send(text,strlen(text));
  if (result>=0) {
    char eol[2] = "\n";
    return send(eol,1);
  } else {
    return result;
  }
}

const char *NovaTelnetClient::receiveText(double timeout) {
  //printf("*** at %d len %d\n", at, len);
  if (len>0) {
    if (at!=0) {
      memmove(current,current+at,len);
      at = 0;
    }
  }
  bool has_more = false;
  for (int i=0; i<MAX_TELNET_BUFFER&&i<len; i++) {
    if (current[i]=='\n') {
      has_more =true;
      break;
    }
  }
  int add = 0;
  if (!has_more) {
    add = receive(current+len,MAX_TELNET_BUFFER-len,timeout);
  }
  //printf("[*** got %d]\n", add);
  if (add>=0) {
    len += add;
    bool found = false;
    int i;
    for (i=0; i<MAX_TELNET_BUFFER&&i<len; i++) {
      if (current[i]=='\n') {
	current[i] = '\0';
	found = true;
	break;
      }
    }
    if (found) {
      len = len-i-1;
      if (len>0) {
	at = i+1;
      } else {
	at = 0;
	len = 0;
      }
      return current;
    }
  }
  return NULL;
}


