/**
 * Header for some BMP creation functions
 */
#ifndef __BMP_H__
#define __BMP_H__
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * The header structur 
 */
typedef struct {
  u_int8_t  bfType[2];       /* BM */
  u_int32_t bfSize;          /* bmp size */
  u_int32_t bfReserved;      /* 0 */
  u_int32_t bfOffByte;       /* off set off img data from begining */
  u_int32_t biSize;          /* 40 ( Header size)*/
  u_int32_t biWidth;         /* img width */
  u_int32_t biHeight;        /* img height */
  u_int16_t biPlanes;        /* 1 (not needed) */
  u_int16_t biBitCount;      /* color deep (1, 4, 8, 16, 24, 32) */
  #define BI_RGB       0     /* not compressed */
  #define BI_RLE8      1     /* langs compressd for 8bbp */
  #define BI_REL4      2     /* langs compressd for 4bbp */
  #define BI_BITFIELDS 3     /* uncompressd with color table */
  u_int32_t biCompression;   /* compression one of the BI_* */
  u_int32_t biSizeImage;     /* img data in Byte */
  u_int32_t biXPelsPerMeter; /* 0 */
  u_int32_t biYPelsPerMeter; /* 0 */
  u_int32_t biClrUsed;       /* num of entries of the color table */
  u_int32_t biClrImportant;  /* 0 or number of used collors in img */
} BMPHeader;


/**
 * The BMP structur
 */
typedef struct {
  BMPHeader head;
  char     *body;
 } BMP;

// functions
void init_RGB_BMPHeader(BMPHeader *head, u_int32_t width, u_int32_t height);
int bmp_write_header(BMPHeader *head, int fd);
#endif // __BMP_H__
