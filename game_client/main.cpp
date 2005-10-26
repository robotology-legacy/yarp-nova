#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "Nova.h"

#include "simio.h"

#define real_printf printf
#define printf cprintf


#define MAX_BUF 256
#define IN_BUFS 4

class Msg {
public:
  char out[MAX_BUF];
  char in[IN_BUFS][MAX_BUF];
  int out_idx;
  int in_idx;

  Msg() {
    out_idx = 0;
    out[out_idx] = '\0';
    for (int i=0; i<IN_BUFS; i++) {
      in[i][0] = '\0';
    }
  }

  void add(char ch) {
      if (out_idx<MAX_BUF-2) {
	out[out_idx] = ch;
	out_idx++;
	out[out_idx] = '\0';
      }
  }

  void reset() {
    out[0] = '\0';
    out_idx = 0;
  }
  
  const char *getOut() {
    return out;
  }

  void setIn(const char *msg) {
    strncpy(in[in_idx], msg, MAX_BUF);
    in_idx = (in_idx+1)%IN_BUFS;
  }

  const char *getIn(int hist) {
    int idx = in_idx-hist-1+IN_BUFS*2;
    idx = idx % IN_BUFS;
    if (idx<0) { idx=0; }
    return in[idx];
  }

} msg;

class ClientThread : public NovaThread {
public:
  NovaTelnetClient client;
  NovaSemaphore mutex;
  bool active;
  bool ready;
  bool login;
  bool stop;

  void setStop() {
    stop = true;
  }

  ClientThread() : mutex(1) {
    active = false;
    ready = false;
    login = false;
    stop = false;
  }

  void connect(const char *server, int port) {
    client.connect(server,port);
    active = true;
  }

  void look() {
    mutex.wait();
    client.sendText("look");
    mutex.post();
  }

  void go(const char *dir) {
    char buf[256];
    sprintf(buf,"go %s", dir);
    mutex.wait();
    if (active) {
      client.sendText(buf);
    }
    mutex.post();
  }

  void say(const char *txt) {
    mutex.wait();
    char buf[1000];
    sprintf(buf,"say %s", txt);
    client.sendText(buf);
    mutex.post();
  }

  virtual void main() {
    bool done = false;
    while (!done && !stop) {
      const char *str = client.receiveText();
      if (str!=NULL) {
	printf("        %s\n", str);
	autorefresh();
	//client.sendText(str);
	if (str[0] == '@') {
	  if (strcmp(str,"@status ready 1")==0) {
	    ready = true;
	    printf("*** READY!\n");
	    mutex.wait();
	    client.sendText("connect foo bar");
	    mutex.post();
	  }
	  if (strcmp(str,"@status login 1")==0) {
	    login = true;
	    printf("*** LOGGED IN!\n");
	    mutex.wait();
	    client.sendText("look");
	    mutex.post();
	  }
	  if (str[1] == 'b' && str[2]=='r') {
	    msg.setIn(str);
	  }
	  if (strcmp(str,"@look begins")==0) {
	    clrscr();
	    printf("\n\n[Hit 'q' to end, arrow keys to move]\n\n");
	    for (int i=2; i>=0; i--) {
	      printf("%s\n",msg.getIn(i));
	    }
	    printf("\n%s\n\n",msg.getOut());
	  }
	}
      }
    }
    active = false;
    printf("EXITING\n");
  }
};

ClientThread t;


class UpdateThread : public NovaThread {
public:
  bool stop;

  UpdateThread() {
    stop = false;
  }

  void setStop() {
    stop = true;
  }

  virtual void main() {
    while (!stop) {
      autorefresh();
      NovaTime::sleep(0.25);
      t.look();
    }
  }
} update_thread;


void stop(int x) {
  clrscr();
  printf("Stopping...\n");
  autorefresh();
  t.setStop();
  NovaTime::sleep(1);
  update_thread.setStop();
  NovaTime::sleep(1);
  deinitconio();
  exit(0);
}

int main() {
  signal(SIGKILL,stop);
  signal(SIGINT,stop);
  signal(SIGTERM,stop);
  signal(SIGPIPE,stop);
  int kill = 0;
  initconio();
  setautorefresh(1);
  clrscr();
  t.connect("localhost",9999);
  t.begin();
  update_thread.begin();
  int mode = 0;
  while (!kill) {
    int key = waitkey();
    //printf("KEY is %d\n", key);
    switch (key) {
    case 'q':
      if (mode==0) {
	kill = 1;
      }
      break;
    case 27:
      if (mode==0) mode++;
      else mode = 0;
      break;
    case 91:
      if (mode==1) mode++;
      else mode = 0;
      break;
    case 67:
      if (mode==2) {
	t.go("right");
      }
      mode = 0;
      break;
    case 68:
      if (mode==2) {
	t.go("left");
      }
      mode = 0;
      break;
    case 65:
      if (mode==2) {
	t.go("up");
      }
      mode = 0;
      break;
    case 66:
      if (mode==2) {
	t.go("down");
      }
      mode = 0;
      break;
    case 10:
    case 13:
      t.say(msg.getOut());
      msg.reset();
      mode = 0;
      break;
    default:
      if (mode==0) {
	msg.add((char)key);
      }
      mode = 0;
      break;
    }
    //NovaTime::sleep(10);
  }
  stop(0);
  return 0;
}
