#ifndef GAME_H
#define GAME_H

#include "Player.h"

class Game {
public:
  Game();

  void update();

  static Game& getGame();

  void look(ID id, Replier& replier);

  void move(ID id, Replier& replier, int dx, int dy);

};

#endif
