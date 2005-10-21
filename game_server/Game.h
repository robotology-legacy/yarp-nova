#ifndef GAME_H
#define GAME_H

#include "Nova.h"

#include "Player.h"
#include "Thing.h"

class Game : public NovaThread {
public:
  Game();

  void update();

  static Game& getGame();
  
  Thing& getThing(ID id);

  ID getCell(ID x, ID y);

  void setCell(ID x, ID y, ID val);

  virtual void main();

  void save(bool force = false);
  void load();

  void wait();
  void post();

};

#endif
