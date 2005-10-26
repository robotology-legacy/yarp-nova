#ifndef GAME_H
#define GAME_H

#include "Nova.h"

#include "Thing.h"

class Game : public NovaThread {
public:
  Game();

  virtual ~Game();

  void setMaze(const char *fname);

  void update();

  static Game& getGame();

  void setCell(ID x, ID y, ID val);
  ID getCell(ID x, ID y);

  Thing& newThing();
  void killThing(ID x);

  bool isThing(ID id);
  Thing& getThing(ID id);

  virtual void main();

  void save(bool force = false);
  void load();

  void wait();
  void post();

private:
  void *system_resource;
  void init();

};

#endif
