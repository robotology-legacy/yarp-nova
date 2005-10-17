#include <string.h>
#include <stdio.h>

#include "Player.h"
#include "Game.h"

#define MAX_ARG_CT (8)
#define MAX_ARG_LEN (256)

void Player::apply(int argc, const char *argv[]) {

  if (argc>=1) {
    switch(argv[0][0]) {
    case 'l':
      reply("should look around");
      Game::getGame().look(id,*this);
      break;
    case 'g':
      if (argc==2) {
	switch (argv[1][0]) {
	case 'l':
	  reply("go left");
	  Game::getGame().move(id,*this,-1,0);
	  break;
	case 'r':
	  reply("go right");
	  Game::getGame().move(id,*this,1,0);
	  break;
	case 'u':
	  reply("go up");
	  Game::getGame().move(id,*this,0,-1);
	  break;
	case 'd':
	  reply("go down");
	  Game::getGame().move(id,*this,0,1);
	  break;
	}
      }
      break;
    case 'c':
      reply("should connect [con username passwd]");
      if (argc==3) {
	printf("LOGIN %s %s\n", argv[1], argv[2]);
      }
      break;
    case 'b':
      reply("should build");
      if (argc==2) {
	id = atol(argv[1]);
	printf("Building %d\n", id.asInt());
      }      
      break;
    case 'd':
      reply("should disconnect");
      break;
    case 'h':
      reply("should give help");
      break;
    }
  }
}


void Player::apply(const char *command) {
  const char *argv[MAX_ARG_CT];
  char buf[MAX_ARG_CT][MAX_ARG_LEN];

  int at = 0;
  int sub_at = 0;
  for (int i=0; i<strlen(command)+1; i++) {
    if (at<MAX_ARG_CT) {
      char ch = command[i];
      if (ch>=32||ch=='\0') {
	if (ch==' ') {
	  ch = '\0';
	}
	if (sub_at<MAX_ARG_LEN) {
	  buf[at][sub_at] = ch;
	  sub_at++;
	}
      }
      if (ch == '\0') {
	if (sub_at>1) {
	  at++;
	}
	sub_at = 0;
      } 
    }
  }
  for (int i=0; i<MAX_ARG_CT; i++) {
    argv[i] = buf[i];
    buf[i][MAX_ARG_LEN-1] = '\0';
  }

  reply("you said...");
  for (int i=0; i<at; i++) {
    char txt[MAX_ARG_LEN+100];
    sprintf(txt,"%d \"%s\"", i, buf[i]);
    reply(txt);
  }
  apply(at,argv);
}

