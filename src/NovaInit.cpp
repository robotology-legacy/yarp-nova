
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

#include <ace/Init_ACE.h>

#include "NovaInit.h"


static void shutdown(int x) {
  printf("Shutting down Nova...\n");
  NovaInit::fini();
  exit(0);
}

void NovaInit::init() {
  ACE_Init_ACE::init();
  signal(SIGPIPE, SIG_IGN);
  signal(SIGTERM, shutdown);
  signal(SIGINT, shutdown);
}

void NovaInit::fini() {
  ACE_Init_ACE::fini();
}

