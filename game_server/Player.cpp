#include <string.h>
#include <stdio.h>

#include "Player.h"
#include "Game.h"

#define MAX_ARG_CT (8)
#define MAX_ARG_LEN (256)

void Player::apply(int argc, const char *argv[]) {
  Game& game = Game::getGame();
  game.wait();

  if (argc>=1) {
    switch(argv[0][0]) {
    case 'l':
      if (isEmbodied()) {
	look();
	//Game::getGame().look(id,*this);
      }
      break;
    case 'g':
      if (isEmbodied()) {
	if (argc==2) {
	  switch (argv[1][0]) {
	  case 'l':
	    move(-1,0);
	    //Game::getGame().move(id,*this,-1,0);
	    break;
	  case 'r':
	    move(1,0);
	    //Game::getGame().move(id,*this,1,0);
	    break;
	  case 'u':
	    move(0,-1);
	    //Game::getGame().move(id,*this,0,-1);
	    break;
	  case 'd':
	    move(0,1);
	    //Game::getGame().move(id,*this,0,1);
	    break;
	  }
	}
      }
      break;
    case 'c':
      reply("This will eventually be a login, somewhat pretend for now");
      {
      const char *name = "default";
      const char *key = "default";
      if (argc>=2) { name = argv[1]; }
      if (argc>=3) { key = argv[2]; }
      if (argc==3||1) {
	printf("LOGIN %s %s\n", argv[1], argv[2]);
	bool ok = login.apply(name,key);
	if (!ok) {
	  reply("failed");
	} else {
	  id = login.getID();
	  reply("logged in");
	}
      }
      }
      break;
    case 's':
      reply("Forcing save");
      Game::getGame().save();
      break;
      /*
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
      */
    case 'h':
      reply("should give help");
      break;
    default:
      reply("Command not understood");
      reply("Known commands: \"look\" \"go left\" \"go right\" \"go up\" \"go down\"");
      break;
    }
  }

  game.post();
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

  /*
  reply("you said...");
  for (int i=0; i<at; i++) {
    char txt[MAX_ARG_LEN+100];
    sprintf(txt,"%d \"%s\"", i, buf[i]);
    reply(txt);
  }
  */
  apply(at,argv);
}




void Player::move(int dx, int dy) {
  reply("Move requested");
  Game& game = Game::getGame();
  Thing& thing = game.getThing(id);
  if (dx>1) dx = 1;
  if (dx<-1) dx = -1;
  if (dy>1) dy = 1;
  if (dy<-1) dy = -1;
  thing.setMove(dx,dy);

  /*
  // should be separate
  ID x, y;
  x = thing.getX();
  y = thing.getY();
  game.setCell(x,y,0);
  thing.applyMove();
  x = thing.getX();
  y = thing.getY();
  game.setCell(x,y,id);
  */
}

void Player::look() {
  reply("Looking around...");
  ID x, y;
  Game& game = Game::getGame();
  Thing& thing = game.getThing(id);
  x = thing.getX();
  y = thing.getY();
  char buf[256], buf_bar[256];
  sprintf(buf,"Location is currently %d %d", x.asInt(), y.asInt());
  reply(buf);
  int dx = 10, dy = 5;
 
  int at = 0;
  for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx+2; xx++) {
    buf_bar[at] = '+';
    at++;
  }
  buf_bar[at] = '\0';
  reply(buf_bar);

  for (long int yy=y.asInt()-dy; yy<=y.asInt()+dy; yy++) {
    at = 0;
    buf[at] = '+';
    at++;
    for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx; xx++) {
      char ch = ' ';
      ID nid = game.getCell(ID(xx),ID(yy));
      long int x = nid.asInt();
      if (x!=0) {
	if (x>=100) {
	  ch = 'O';
	} else {
	  ch = 'X';
	}
      }
      buf[at] = ch;
      at++;
    }
    buf[at] = '+';
    at++;
    buf[at] = '\0';
    reply(buf);
  }
  reply(buf_bar);
}


