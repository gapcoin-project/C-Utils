/**
 * Some BMP creation function
 */
#ifndef __BMP__
#define __BMP__
#include "bmp.h"

/**
 * initializes an given BMP with an given width and height
 * uses uncompressd 24 Bit color 
 *
 * NOTE it onely initializes the Header and dos not allocaate the data
 */
void init_RGB_BMPHeader(BMPHeader *head, uint32_t width, uint32_t height) {

  // init Head
  head->bfType[0]       = (uint8_t) 'B';
  head->bfType[1]       = (uint8_t) 'M';
  head->bfSize          = 54 + (width * height * 3);
  head->bfReserved      = 0;
  head->bfOffByte       = 54;
  head->biSize          = 40;
  head->biWidth         = width;
  head->biHeight        = height;
  head->biPlanes        = 1;
  head->biBitCount      = 24;           /* use 24 Bit color deep */
  head->biCompression   = BI_RGB;
  head->biSizeImage     = width * height * 3;
  head->biXPelsPerMeter = 0;
  head->biYPelsPerMeter = 0;
  head->biClrUsed       = 0;
  head->biClrImportant  = 0;
  
 }

/**
 * Wites the header form the given BMP to the given file descriptor
 */
int bmp_write_header(BMPHeader *head, int fd) {

  if(write(fd, &head->bfType,          sizeof(uint16_t)) == -1) return -1;
  if(write(fd, &head->bfSize,          sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->bfReserved,      sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->bfOffByte,       sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biSize,          sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biWidth,         sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biHeight,        sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biPlanes,        sizeof(uint16_t)) == -1) return -1;
  if(write(fd, &head->biBitCount,      sizeof(uint16_t)) == -1) return -1;
  if(write(fd, &head->biCompression,   sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biSizeImage,     sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biXPelsPerMeter, sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biYPelsPerMeter, sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biClrUsed,       sizeof(uint32_t)) == -1) return -1;
  if(write(fd, &head->biClrImportant,  sizeof(uint32_t)) == -1) return -1;

  return 1;

} 
#endif // __BMP__
