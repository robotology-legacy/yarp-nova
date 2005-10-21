#ifndef LOGIN_INC
#define LOGIN_INC

#include "Thing.h"

class Login {
public:
  Login();
  bool apply(const char *name, const char *key);
  ID getID() {
    return base_id;
  }
private:
  ID base_id;
};

#endif
