
#include <stdio.h>
#include <assert.h>

#include "Thing.h"
#include "Game.h"

#include "Matrix.h"
#include "Things.h"



class GameHelper {
public:
  Matrix game_matrix;
  Things game_things;
  NovaSemaphore game_mutex;

  GameHelper() : game_mutex(1) {
  }
};

#define SYS(x) (*((GameHelper*)(x)))

Game *the_game = NULL;


Game::Game() {
  system_resource = new GameHelper();
  init();
}

Game::~Game() {
  delete &SYS(system_resource);
}

void Game::init() {
  setMaze("maze.txt");
  begin();
}


void Game::setMaze(const char *fname) {
  Matrix& m = SYS(system_resource).game_matrix;
  ifstream fin(fname);
  int y = 0;
  printf("Initializing maze...\n");
  while (!fin.eof()) {
    char buf[2000];
    fin.getline(buf,sizeof(buf));
    for (int x=0; x<strlen(buf); x++) {
      char ch = buf[x];
      ID v = 0;
      if (ch=='+' || ch=='-' || ch=='|') { v = 1; }
      if (ch>='0'&&ch<='9') { 
	v = SYS(system_resource).game_things.create();
	SYS(system_resource).game_things.getThing(v).set(x,y,v);
      }
      if (v.asInt()!=0) {
	m.set(x,y,v);
      }
    }
    y++;
  }
  printf("Maze initialized\n");
}

void Game::update() {
  SYS(system_resource).game_things.update();
}

Game& Game::getGame() {
  //return the_game;
  if (the_game==NULL) { the_game = new Game; }
  return *the_game;
}

Thing& Game::getThing(ID id) {
  Thing& thing = SYS(system_resource).game_things.getThing(id);
}

bool Game::isThing(ID id) {
  return SYS(system_resource).game_things.isThing(id);
}

Thing& Game::newThing() {
  ID id = -1;
  for (int i=100; i<400; i++) {
    if (!isThing(ID(i))) {
      id = i;
      break;
    }
  }  
  if (id.asInt()!=-1) {
    ID xx = -1;
    ID yy = -1;
    for (int r=1; r<20 && xx.asInt()==-1; r++) {
      for (int n=0; n<r && xx.asInt()==-1; n++) {
	ID x = r-n;
	ID y = n;
	if (SYS(system_resource).game_matrix.get(x,y).asInt()==0) {
	  xx = x;
	  yy = y;
	}
      }
    }
    if (xx.asInt()!=-1) {
      printf("Make new at %ld %ld\n", xx.asInt(), yy.asInt());
      SYS(system_resource).game_things.create(id);
      SYS(system_resource).game_things.getThing(id).set(xx,yy,id);
      SYS(system_resource).game_matrix.set(xx,yy,id);
    }
  }
  if (id.asInt()==-1) {
    return Thing::NOTHING;
  }
  return getThing(id);
}


void Game::killThing(ID x) {
  wait();
  Thing& thing = SYS(system_resource).game_things.getThing(x);
  SYS(system_resource).game_matrix.set(thing.getX(),thing.getY(),0);
  SYS(system_resource).game_things.destroy(x);
  post();
}



ID Game::getCell(ID x, ID y) {
  return SYS(system_resource).game_matrix.get(x,y);
}

void Game::setCell(ID x, ID y, ID val) {
  SYS(system_resource).game_matrix.set(x,y,val);
}


void Game::main() {
  while (1) {
    wait();
    update();
    post();
    NovaTime::sleep(0.25);
  }
}


void Game::save(bool force) {
  // incremental or full save
  // for now, always full

  wait();
  SYS(system_resource).game_matrix.save();
  post();
}

void Game::load() {
}

void Game::wait() {
  SYS(system_resource).game_mutex.wait();
}

void Game::post() {
  SYS(system_resource).game_mutex.post();
}


class GameDown {
public:
  GameDown() {};
  ~GameDown() {
    if (the_game!=NULL) {
      delete the_game;
    }
  }
} game_down;
