/**
 * converts any fiel to RGB-BMP
 */
#include <inttypes.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../src/wave.h"

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("%s <file>\n", argv[0]);
    exit(1);
  }

  struct stat st;
  stat(argv[1], &st);
  uint32_t f_size = st.st_size;

  int f = open(argv[1], O_RDONLY, 0);
  char *f_data  = mmap(NULL, f_size, PROT_READ, MAP_SHARED, f, 0);

  WAVEHeader wave;
  init_WAVEHeader(&wave, f_size);

  char buffer[256];
  sprintf(buffer, "%s.wav", argv[1]);
  
  int fd = open(buffer, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
  if(fd == -1) {
    printf("failed to open %s\n", buffer);
    exit(1);
  }

  if(wave_write_header(&wave, fd) == -1) {
    printf("in main() while writing header to file\n");
    exit(1);
  }

  if(write(fd, f_data, f_size) == -1) {
    printf("in main() while writing body to file\n");
    exit(1);
  }
   
  return 0;
}

