
#include <stdio.h>
#include <assert.h>

#include "Thing.h"
#include "Game.h"

#include <ace/Hash_Map_Manager.h>
#include <ace/Null_Mutex.h>

#include <fstream>
using namespace std;

char *board_init[] = {
  "###################################################################",
  "           0                                                      #",
  "#                                                                 #",
  "#                                                                 #",
  "#                                                                 #",
  "#                                                                 #",
  "#             1                                                   #",
  "#                                                                 #",
  "#                          ############################           #",
  "#                                                                 #",
  "#                          ###########   ##############           #",
  "#                                #       #                        #",
  "#                                #       #                        #",
  "#                                #########                        #",
  "#                                      2                          #",
  "#                                                                 #",
  "#                                                                 #",
  "#                                                                  ",
  "###################################################################",
  NULL
};


class hline : public ACE_Hash_Map_Manager<ID,ID,ACE_Null_Mutex> {
public:
  hline() {
  }
  hline(const hline& alt) {
  }
};

//typedef ACE_Hash_Map_Manager<ID,ID,ACE_Null_Mutex> hline;

typedef ACE_Hash_Map_Manager<ID,Thing,ACE_Null_Mutex> hthing;
typedef ACE_Hash_Map_Entry<ID,Thing> hthing_entry;

typedef ACE_Hash_Map_Manager<ID,ID,ACE_Null_Mutex> hid;
typedef ACE_Hash_Map_Iterator<ID,ID,ACE_Null_Mutex> hid_iterator;

typedef ACE_Hash_Map_Manager<ID,hline,ACE_Null_Mutex> hmatrix;
typedef ACE_Hash_Map_Entry<ID,hline> hmatrix_entry;
typedef ACE_Hash_Map_Iterator<ID,hline,ACE_Null_Mutex> hmatrix_iterator;

class Matrix {
private:
  hmatrix matrix;
public:
  ID get(ID x, ID y) {
    hmatrix_entry *entry;
    if (0==matrix.find(y,entry)) {
      ID result;
      if (0==entry->int_id_.find(x,result)) {
	return result;
      }
    }
    return ID(0);
  }

  void set(ID x, ID y, ID val) {
    hmatrix_entry *entry;
    if (matrix.find(y,entry)!=0) {
      matrix.bind(y,hline());
      int find_again = matrix.find(y,entry);
      assert(find_again==0);
    }
    entry->int_id_.rebind(x,val);
  }

  void save() {
    ofstream fout("/tmp/index.txt");
    hid ids;
    for (hmatrix_iterator it=matrix.begin(); it!=matrix.end(); it++) {
      ID y = (*it).ext_id_;
      hid& line = (*it).int_id_;
      for (hid_iterator it2=line.begin(); it2!=line.end(); it2++) {
	ID x = (*it2).ext_id_;
	ID v = (*it2).int_id_;
	printf("(%ld,%ld) = %ld\n",x.asInt(),y.asInt(),v.asInt());
	fout << "piece " << x.asInt() << " " << y.asInt() << " " << 
	  v.asInt() << endl;
	ids.rebind(v,v);
      }
    }
    for (hid_iterator it=ids.begin(); it!=ids.end(); it++) {
      ID v = (*it).ext_id_;
      printf("need state for id %ld\n", v.asInt());

      char fname[256];
      sprintf(fname, "/tmp/%ld.txt", v.asInt());

      fout << "state " << v.asInt() << " " << fname << endl;

      ofstream fout2(fname);
      fout2 << "id " << v.asInt() << endl;
      
      //fout2 << "position " << 
    }
  }
};

class Things {
public:
  hthing things;
  hid created;
  hid destroyed;
  ID at;
  static const ID theta;
  Thing null_thing;

  Things() {
    at = theta;
  }

  static bool isFluid(ID x) {
    return x.asInt()>=theta.asInt();
  }

  ID create() {
    // could also reuse
    ID result = at;
    created.bind(at,at);
    things.bind(at,Thing());
    at = at.asInt()+1;
    return result;
  }

  Thing& getThing(ID x) {
    hthing_entry *entry;
    if (0==things.find(x,entry)) {
      return entry->int_id_;
    }
    return null_thing;
  }

  void destroy(ID x) {
    // prepare for reuse
    created.unbind(at,at);
    destroyed.bind(x,x);
  }

  bool isDestroyed(ID x) {
    ID result;
    return destroyed.find(x,result)==0;
  }

  void update() {
    for (hid_iterator it=created.begin(); it!=created.end(); it++) {
      ID id = (*it).ext_id_;
      getThing(id).update();
    }
  }
};

const ID Things::theta = 100;

Matrix game_matrix;
Things game_things;
Game *the_game = NULL;


void Game::init() {
  Matrix& m = game_matrix;
  for (int i=0; board_init[i]!=NULL; i++) {
    const char *line = board_init[i];
    printf("%s\n",line);
    for (int j=0; j<strlen(line); j++) {
      ID v = 0;
      char ch = line[j];
      if (ch=='#') { v = 1; }
      if (ch>='0'&&ch<='9') { 
	v = game_things.create();
	game_things.getThing(v).set(j,i,v);
      }
      if (v.asInt()!=0) {
	m.set(j,i,v);
      }
    }
  }
  begin();
}

void Game::update() {
  //Matrix& m = game_matrix;
  //printf("m[0][0] = %d\n", m.get(0,0).asInt());
  //printf("m[5][10] = %d\n", m.get(5,10).asInt());
  //m.set(5,10,15);
  //printf("m[5][10] = %d\n", m.get(5,10).asInt());
  game_things.update();
}

Game& Game::getGame() {
  //return the_game;
  if (the_game==NULL) { the_game = new Game; }
  return *the_game;
}

Thing& Game::getThing(ID id) {
  Thing& thing = game_things.getThing(id);
}



/*
void Game::move(ID id, Replier& replier, int dx, int dy) {
  replier.send("Yo. Requesting move.");
  Thing& thing = game_things.getThing(id);
  if (dx>1) dx = 1;
  if (dx<-1) dx = -1;
  if (dy>1) dy = 1;
  if (dy<-1) dy = -1;
  thing.setMove(dx,dy);

  // should be separate
  ID x, y;
  x = thing.getX();
  y = thing.getY();
  game_matrix.set(x,y,0);
  thing.applyMove();
  x = thing.getX();
  y = thing.getY();
  game_matrix.set(x,y,id);
}

void Game::look(ID id, Replier& replier) {
  replier.send("Yo.");
  ID x, y;
  Thing& thing = game_things.getThing(id);
  x = thing.getX();
  y = thing.getY();
  char buf[256], buf_bar[256];
  sprintf(buf,"OBJ %d at %d %d\n", id.asInt(), x.asInt(), y.asInt());
  replier.send(buf);
  int dx = 10, dy = 5;
 
  int at = 0;
  for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx+2; xx++) {
    buf_bar[at] = '+';
    at++;
  }
  buf_bar[at] = '\0';
  replier.send(buf_bar);

  for (long int yy=y.asInt()-dy; yy<=y.asInt()+dy; yy++) {
    at = 0;
    buf[at] = '+';
    at++;
    for (long int xx=x.asInt()-dx; xx<=x.asInt()+dx; xx++) {
      char ch = ' ';
      ID nid = game_matrix.get(ID(xx),ID(yy));
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
    replier.send(buf);
  }
  replier.send(buf_bar);
}
*/

void Thing::set(ID n_x, ID n_y, ID n_id) {
  Matrix& m = game_matrix;

  ID prev = m.get(n_x,n_y);

  ID next = n_id;
  if (next.asInt() == -1) {
    if (id.asInt() == -1) {
      next = game_things.create();
    } else {
      next = id;
    }
  }
  
  if (game_things.isFluid(prev)) {
    game_things.destroy(prev);
  }
  x = n_x;
  y = n_y;
  id = n_id;
  game_matrix.set(x,y,id);
}

void Thing::applyMove() {
  Game& game = Game::getGame();

  if (dx!=0 || dy!=0) {

    ID x2 = x.asInt()+dx;
    ID y2 = y.asInt()+dy;
    
    if (game.getCell(x2,y2)==0) {
      game.setCell(x,y,0);    
      x = x2;
      y = y2;
      game.setCell(x,y,id);
      printf("Implemented move for %ld\n", id.asInt());
    } else {
      printf("Ignored blocked move\n");
    }

    dx = dy = 0;
  }
}
  


ID Game::getCell(ID x, ID y) {
  return game_matrix.get(x,y);
}

void Game::setCell(ID x, ID y, ID val) {
  game_matrix.set(x,y,val);
}


void Game::main() {
  while (1) {
    wait();
    update();
    post();
    NovaTime::sleep(1);
  }
}


void Game::save(bool force) {
  // incremental or full save
  // for now, always full

  wait();
  game_matrix.save();
  post();
}

void Game::load() {
}

void Game::wait() {
  mutex.wait();
}

void Game::post() {
  mutex.post();
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
