
#ifndef THING_H
#define THING_H

#include <math.h>

#include "ID.h"


class Thing {
private:
  ID id;
  ID x, y;
  int dx, dy;

public:
  Thing() { id = -1; x = y = 0; dx = dy = 0; }

  static Thing NOTHING;

  void set(ID n_x, ID n_y, ID n_id = -1);

  void setMove(int ndx, int ndy) {
    dx = ndx;
    dy = ndy;
  }

  ID getID() { return id; }
  ID getX() { return x; }
  ID getY() { return y; }

  void applyMove();
  
  void update() {
    applyMove();
  }
};

#endif
