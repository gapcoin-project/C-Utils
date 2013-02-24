/**
 * Some BMP creation function
 */
#ifndef BMP
#define BMP
#include "bmp.h"

/**
 * initializes an given BMP with an given width and height
 * uses uncompressd 24 Bit color 
 *
 * NOTE it onely initializes the Header and dos not allocaate the data
 */
void init_RGB_BMPHeader(BMPHeader *head, u_int32_t width, u_int32_t height) {

  // init Head
  head->bfType[0]       = (u_int8_t) 'B';
  head->bfType[1]       = (u_int8_t) 'M';
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

  if(write(fd, &head->bfType,          sizeof(u_int16_t)) == -1) return -1;
  if(write(fd, &head->bfSize,          sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->bfReserved,      sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->bfOffByte,       sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biSize,          sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biWidth,         sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biHeight,        sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biPlanes,        sizeof(u_int16_t)) == -1) return -1;
  if(write(fd, &head->biBitCount,      sizeof(u_int16_t)) == -1) return -1;
  if(write(fd, &head->biCompression,   sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biSizeImage,     sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biXPelsPerMeter, sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biYPelsPerMeter, sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biClrUsed,       sizeof(u_int32_t)) == -1) return -1;
  if(write(fd, &head->biClrImportant,  sizeof(u_int32_t)) == -1) return -1;

  return 1;

} 
#endif // end of BMP
