#include <stdio.h>
#include <stdlib.h>
#include "Point.h"
#include <sys/types.h>
#include <inttypes.h>

int main() {
  Point p, p2;
  u_int64_t l;
  p.x = 1;
  p.y = 2;
  l = POINT_TO_UINT(p);
  UINT_TO_POINT(l, p2)

  printf("(%" PRIu32 ", %" PRIu32 ") => %" PRIu64 ": High: %08x Low: %08x  (%" PRIu32 ", %" PRIu32 ") \n", p.x, p.y, l, (u_int32_t) (l >> 32), (u_int32_t) l, p2.x, p2.y);



  return 0;
}
