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
    case 's':
      {
	char buf[256];
	const char prefix[] = "@broadcast";
	int at = 0;
	for (int i=0; i<argc; i++) {
	  const char *txt = (i>0)?argv[i]:prefix;
	  if (i>0) {
	    if (at<sizeof(buf)-2) {
	      buf[at] = ' ';
	      at++;
	    }
	  }
	  for (int j=0; j<strlen(txt); j++)
	    if (at<sizeof(buf)-2) {
	      buf[at] = txt[j];
	      at++;
	    }
	}
	buf[at] = '\0';
	broadcast(buf);
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
      send("This will eventually require username/password, but not yet");
      {
      const char *name = "default";
      const char *key = "default";
      if (argc>=2) { name = argv[1]; }
      if (argc>=3) { key = argv[2]; }
      if (argc==3||1) {
	printf("LOGIN %s %s\n", name, key);
	bool ok = login.apply(name,key);
	if (!ok) {
	  send("@error login failed");
	} else {
	  id = login.getID();
	  send("@status login 1");
	  send("Log in successful");
	}
      }
      }
      break;
      /*
    case 'v':
      send("Forcing save");
      Game::getGame().save();
      break;
    case 'h':
      send("should give help");
      break;
      */
    default:
      send("Command not understood");
      send("Known commands: \"look\" \"go left\" \"go right\" \"go up\" \"go down\"");
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

  apply(at,argv);
}




void Player::move(int dx, int dy) {
  send("Move requested");
  Thing& thing = login.getThing();
  if (dx>1) dx = 1;
  if (dx<-1) dx = -1;
  if (dy>1) dy = 1;
  if (dy<-1) dy = -1;
  thing.setMove(dx,dy);
}


void Player::look() {
  send("@look begins");
  ID x, y;
  Game& game = Game::getGame();
  Thing& thing = login.getThing();
  x = thing.getX();
  y = thing.getY();
  char buf[256], buf_bar[256];
  sprintf(buf,"Location is currently %d %d", x.asInt(), y.asInt());
  send(buf);
  int dx = 10, dy = 5;
 
  int at = 0;
  for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx+2; xx++) {
    buf_bar[at] = ':';
    at++;
  }
  buf_bar[at] = '\0';
  send(buf_bar);

  for (long int yy=y.asInt()-dy; yy<=y.asInt()+dy; yy++) {
    at = 0;
    buf[at] = ':';
    at++;
    for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx; xx++) {
      char ch = ' ';
      ID nid = game.getCell(ID(xx),ID(yy));
      long int x = nid.asInt();
      if (x!=0) {
	if (x>=100) {
	  if (x==login.getID().asInt()) {
	    ch = 'Q';
	  } else {
	    ch = 'O';
	  }
	} else {
	  ch = '#';
	}
      }
      buf[at] = ch;
      at++;
    }
    buf[at] = ':';
    at++;
    buf[at] = '\0';
    send(buf);
  }
  send(buf_bar);
  send("@look ends");
}


void Player::shutdown() {
  login.shutdown();
  id = -1;
}


