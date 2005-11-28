#include <string.h>
#include <stdio.h>

#include "Player.h"
#include "Game.h"

#define MAX_ARG_CT (8)
#define MAX_ARG_LEN (256)

//#ifdef __WIN32__
#define for if(1) for
//#endif



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
	char buf[1000];
	const char prefix[] = "@broadcast";
	int at = 0;
	for (int i=-1; i<argc; i++) {
	  const char *txt = NULL;
	  if (i==-1) { 
	    txt = prefix;
	  } else if (i==0) { 
	    txt = getName(); 
	  } else { 
	    txt = argv[i]; 
	  }
	  if (i>=0) {
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


    // manage moves "go" (u,d,l,r)
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

    // manage firing "fire" (u,d,l,r)
    case 'f':
      if (isEmbodied()) {
	if (argc==2) {
	  switch (argv[1][0]) {
	  case 'l':
	    fire(-1,0);
	    break;
	  case 'r':
	    fire(1,0);
	    break;
	  case 'u':
	    fire(0,-1);
	    break;
	  case 'd':
	    fire(0,1);
	    break;
	  }
	}
      }
      break;

    case 'c':
      send("Current server implementation ignores password");
      {
      const char *name = "anon";
      const char *key = "no-password";
      if (argc>=2) { name = argv[1]; }
      if (argc>=3) { key = argv[2]; }
      if (argc==3||1) {
	printf("LOGIN %s %s\n", name, key);
	bool ok = false;
	if (!isEmbodied()) {
	  ok = login.apply(name,key);
	}
	if (!ok) {
	  send("@error login failed");
	} else {
	  id = login.getID();
	  setName(name);
	  send("@status login 1");
	}
      }
      }
      break;
    default:
      send("Command not understood");
      send("Known commands: \"look\" \"fire\" \"go left\" \"go right\" \"go up\" \"go down\"");
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
  Thing& thing = login.getThing();
  if (!thing.isAlive()) { return; }
  send("@move requested");
  if (dx>1) dx = 1;
  if (dx<-1) dx = -1;
  if (dy>1) dy = 1;
  if (dy<-1) dy = -1;
  if (thing.isAlive()) {
    thing.setMove(dx,dy);
  }
}


void Player::fire(int tx, int ty) {
  Thing& thing = login.getThing();
  if (!thing.isAlive()) { return; }

  send("@fire requested");

	// fire (-1, 5); target, at most
	// client (x,y)  beginning

  Game& game = Game::getGame();
  ID x = thing.getX();
  ID y = thing.getY();

  int fr = getFirerange();

  printf("life: %d, range %d", getLife(), getFirerange());

  for(int i=1; i<=fr; i++) {
    send("Fire loop");

      ID nid = game.getCell(ID(x.asInt() + i*tx),ID(y.asInt() + i*ty));
      game.setTransient(ID(x.asInt() + i*tx),ID(y.asInt() + i*ty),1,0.5);
      long int myid = nid.asInt();

      if (myid != 0) {
	if(myid >= 100) {
	  setLife(getLife() - 1000);
	  
	  if(getLife() == 0)  game.getThing(myid).setLifetime(0);
	}
	break;  // something hit
      }
  }
  send("Fire finished");

}



void Player::look() {
  send("@look begins");
  ID x, y;
  Game& game = Game::getGame();
  Thing& thing = login.getThing();
  x = thing.getX();
  y = thing.getY();
  char buf[256], buf_bar[256];
  sprintf(buf,"Location %d %d", x.asInt(), y.asInt());
  send(buf);
  sprintf(buf,"Life %d FireRange %d", (getLife()/1000), getFirerange());
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
	  if(!game.getThing(nid).isAlive()) {
	    if (x==login.getID().asInt()) {
	      ch = '%';
	    } else {
	      ch = '*';
	    }
	  } else if (x==login.getID().asInt()) {
	    ch = 'Q';  // me 
	  } else if(game.getThing(nid).isBullet()) {
	    ch = '.';
	  } else {
	    ch = 'O';  // another one
	  }
	} else if(x == 1)  {
	  ch = '#';	// piece of wall
	}
	else if(x == 2) {
	  ch = 'o';	// a bullet
	}
      }
      if (ch==' ') {
	int transient = game.getTransient(ID(xx),ID(yy));
	if (transient) {
	  ch = '=';
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


  for (long int yy=y.asInt()-dy; yy<=y.asInt()+dy; yy++) {
    for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx; xx++) {
      ID nid = game.getCell(ID(xx),ID(yy));
      long int x = nid.asInt();
      if (x>=100 && x!=login.getID().asInt()) {
	char buf[1000];
	sprintf(buf,"See %d %d %s", 
		xx,yy,
		game.getThing(nid).getName());
	send(buf);
      }
    }
  }

  send("@look ends");
}


void Player::shutdown() {
  login.shutdown();
  id = -1;
}


