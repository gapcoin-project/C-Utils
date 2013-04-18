#include <stdio.h>
#include <stdlib.h>
#include "../src/debug.h"


int main() {

  DBG_MSG("Test Message");
  WARN_MSG("Test Message");
  ERR_MSG("Test Message");

  return 0;
}
