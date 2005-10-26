
#include "NovaTelnetClient.h"

#include <string.h>
#include <stdio.h>

#define DBG if(0) 

NovaTelnetClient::NovaTelnetClient() {
  at = 0;
  len = 0;
}

int NovaTelnetClient::sendText(const char *text) {
  DBG printf("Sending [%s]\n", text);
  int result = send(text,strlen(text));
  DBG printf("result %d\n", result);
  if (result>=0) {
    char eol[3] = "\x0d\x0a";
    return send(eol,2);
  } else {
    return result;
  }
}

const char *NovaTelnetClient::receiveText(double timeout) {
  DBG printf("*** at %d len %d\n", at, len);
  if (len>0) {
    if (at!=0) {
      memmove(current,current+at,len);
      at = 0;
    }
  }
  bool has_more = false;
  int add = 0;
  int sub_add = 0;
  while (!has_more && add>=0) {
    DBG printf("bing\n");
    for (int i=0; i<MAX_TELNET_BUFFER&&i<len; i++) {
      if (current[i]=='\x0d'||current[i]=='\x0a') {
	has_more = true;
	break;
      }
    }
    int prev_len = len;
    if (!has_more) {
      add = receive(current+len,MAX_TELNET_BUFFER-len,timeout);
      if (add>=0) {
	len += add;
      }
    }

    DBG { 
      printf("[*** got %d]\n", add);
      if (add>=0) {
	for (int i=0; i<add;i++) {
	  char *stuff = current+prev_len;
	  printf("[%0x %d]\n", stuff[i], stuff[i]);
	}
      }
    }

    if (add==0) {
      has_more = true;
    }
  }

  if (add>=0) {
    bool found = false;
    int i;
    for (i=0; i<MAX_TELNET_BUFFER&&i<len; i++) {
      if (current[i]=='\x0d' || current[i]=='\x0a') {
	if (current[i]=='\x0a') {
	  found = true;
	}
	current[i] = '\0';
	if (found) {
	  break;
	}
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


