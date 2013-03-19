#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "../src/bmp.h"

int main() {
  BMP bmp;
  init_RGB_BMPHeader(&bmp.head, 30, 30);
  bmp.body = (char *) malloc(sizeof(char) * 30 * 30 * 3);

  int fd = open("test.bmp", O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  if(fd == -1) {
    printf("failed to open test.bmp\n");
    exit(1);
  }

  if(bmp_write_header(&bmp.head, fd) == -1) {
    printf("in main() while writing header to file\n");
    exit(1);
  }

  if(write(fd, bmp.body, 30 * 30 * 3) == -1) {
    printf("in main() while writing body to file\n");
    exit(1);
  }
   
  return 0;
}
