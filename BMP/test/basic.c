#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "bmp.h"

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
      printf("in main() while writing header to filei\n");
      exit(1);
    }

   if(write(fd, bmp.body, 30 * 30 * 3) == -1) {
      printf("in main() while writing body to filei\n");
      exit(1);
    }
    
}
