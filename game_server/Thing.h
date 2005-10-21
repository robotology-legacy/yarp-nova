
#ifndef THING_H
#define THING_H

#include <math.h>

/*
class Location {
public:
  Location() {
    x = y = 0;
  }

  Location(int nx, int ny) {
    x = nx;
    y = ny;
  }

  float getDistance(const Location& loc) {
    return sqrt((x-loc.x)*(x-loc.x)+(y-loc.y)*(y-loc.y));
  }

private:
  int x, y;
};
*/

class ID {
public:
  long int x;
  ID() { x = -1; }
  ID(int nx) { x = nx; }
  bool operator == (const ID& alt) const {
    return x == alt.x;
  }
  long int hash() const {
    return x;
  }
  long int asInt() const {
    return x;
  }
  bool isValid() {
    return x!=-1;
  }
};


class Thing {
private:
  ID id;
  ID x, y;
  int dx, dy;

public:
  Thing() { id = -1; x = y = 0; dx = dy = 0; }

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
