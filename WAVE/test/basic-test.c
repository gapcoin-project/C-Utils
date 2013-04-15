#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>
#include "../src/wave.h"

int main(int argc, char *argv[]) {
  
  if (argc != 2) {
    printf ("%s <wave length>\n", argv[0]);
    exit(1);
  }

  uint32_t length = atoi(argv[1]);
  WAVEHeader head;

  srand(time(NULL));
  init_WAVEHeader(&head, length);

  char *data = malloc(sizeof(char) * length);

  uint32_t i; 
  for (i = 0; i < length; i++)
    data[i] = (char) rand();

  int fd = open("test.wav", O_CREAT|O_WRONLY, 0777);
  wave_write_header(&head, fd);
  write(fd, data, length);
  close(fd);

  return 0;
}
