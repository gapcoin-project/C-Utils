#include "../../Test/src/test.h"
#include "../src/string.h"

#include <stdlib.h>
#include <time.h>

BEVOR_ALL() {
  
  srand(time(NULL));
  return NULL;
}

TEST(test_base64) {

  uint64_t byte_len = 100000;
  uint64_t b64_len = b64_enc_len(byte_len);
  
  (void) args;
  char *b64       = malloc(sizeof(char) * (b64_len + 10));
  uint8_t *bytes  = malloc(sizeof(uint8_t) * (byte_len + 10));
  uint8_t *decode = malloc(sizeof(uint8_t) * (byte_len + 10));

  int j;
  for (j = 0; j < 1000; j++) {
    
    TEST_MSG3("\r%d   ", 1000 - j);
    uint64_t len = (rand() % byte_len) + 1;

 
    uint64_t i;
    for (i = 0; i < len; i++)
      bytes[i] = (uint8_t) (rand() % 256);
 
    to_b64(bytes, len, b64);
    b64_to_byte(b64, decode);

    for (i = 0; i < len; i++) {
      if (bytes[i] != decode[i]) {

        int b1 = (unsigned char) bytes[i], 
            b2 = (unsigned char) bytes[i + 1], 
            b3 = (unsigned char) bytes[i + 2],
            d1 = (unsigned char) decode[i], 
            d2 = (unsigned char) decode[i + 1], 
            d3 = (unsigned char) decode[i + 2],
            c1 = (unsigned char) b64[(i/3)*4 - 4], 
            c2 = (unsigned char) b64[(i/3)*4 - 3], 
            c3 = (unsigned char) b64[(i/3)*4 - 2], 
            c4 = (unsigned char) b64[(i/3)*4 - 1],
            c5 = (unsigned char) b64[(i/3)*4], 
            c6 = (unsigned char) b64[(i/3)*4+1], 
            c7 = (unsigned char) b64[(i/3)*4+2], 
            c8 = (unsigned char) b64[(i/3)*4+3];

        ASSERT_FAIL("\n[DD] origin: 0x%x 0x%x 0x%x\n"
                    "[DD] decode: 0x%x 0x%x 0x%x\n[DD] base64: 0x%x 0x%x 0x%x "
                    "0x%x 0x%x 0x%x 0x%x 0x%x\nlen: %lu\ni: %lu\n", 
                    b1, b2, b3, d1, d2, d3, c1, c2, c3, c4, c5, c6, c7, c8, len, i);

      }
    }
  }

  TEST_MSG3("\r                 \r");

  free(bytes);
  free(decode);
  free(b64);

  return NULL;
}
