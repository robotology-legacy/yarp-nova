#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "NovaSemaphore.h"
#include "NovaThread.h"
#include "NovaTime.h"
#include "NovaClient.h"
#include "NovaTelnetClient.h"

int count = 0;

class Thread1 : public NovaThread {
public:
  virtual void main() {
    for (int i=0; i<300 && !isEnding(); i++) {
      count++;
      printf("Count is %d\n", count);
      NovaTime::sleep(1);
    }
  }
};

void testThread() {
  Thread1 t1;
  count = 0;
  assert(count==0);
  t1.begin();
  NovaTime::sleep(4);
  t1.end(2);
  /* YARP currently has a not-so-nice implementation of this */
  assert(count>=3&&count<=5);
}

void testSemaphore() {
  NovaSemaphore s1(1);
  assert(s1.check()==1);
  assert(s1.check()==0);
  s1.post();
  assert(s1.check()==1);
  assert(s1.check()==0);
}

void testClient() {
  printf("Client tests assume a local echo server running on port 2007\n");
  NovaClient client;
  client.connect("localhost",2007);
  char buf[256] = "Bozo\n";
  char buf2[256] = "Gonzo";
  printf("Send...\n");
  client.send(buf,strlen(buf)+1);
  //printf("Sleep...\n");
  //NovaTime::sleep(4);
  printf("Receive...\n");
  int ct = client.receive(buf2,sizeof(buf2),2);
  printf("Got %d: [%s]\n", ct, buf2);
}

void testTelnetClient() {
  printf("Client tests assume a local echo server running on port 2007\n");
  NovaTelnetClient client;
  client.connect("localhost",2007);
  char buf[256] = "Bozo\n";
  char buf2[256] = "Gonzo";
  printf("Send...\n");
  client.sendText("Bozo");
  //printf("Sleep...\n");
  //NovaTime::sleep(4);
  while (1) {
    printf("Receive...\n");
    const char *str = client.receiveText();
    if (str!=NULL) {
      printf(">>> got [%s]\n", str);
    }
    //printf("Got %d: [%s]\n", ct, buf2);
  }
}

int main() {
  printf("Hello world\n");
  //testSemaphore();
  //testThread();
  //testClient();
  testTelnetClient();

  return 0;
}

