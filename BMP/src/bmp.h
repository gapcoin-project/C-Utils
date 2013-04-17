/**
 * Header for some BMP creation functions
 */
#ifndef __BMP_H__
#define __BMP_H__
#include <inttypes.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

/**
 * The header structur 
 */
typedef struct {
  uint8_t  bfType[2];       /* BM */
  uint32_t bfSize;          /* bmp size */
  uint32_t bfReserved;      /* 0 */
  uint32_t bfOffByte;       /* off set off img data from begining */
  uint32_t biSize;          /* 40 ( Header size)*/
  uint32_t biWidth;         /* img width */
  uint32_t biHeight;        /* img height */
  uint16_t biPlanes;        /* 1 (not needed) */
  uint16_t biBitCount;      /* color deep (1, 4, 8, 16, 24, 32) */
  #define BI_RGB       0     /* not compressed */
  #define BI_RLE8      1     /* langs compressd for 8bbp */
  #define BI_REL4      2     /* langs compressd for 4bbp */
  #define BI_BITFIELDS 3     /* uncompressd with color table */
  uint32_t biCompression;   /* compression one of the BI_* */
  uint32_t biSizeImage;     /* img data in Byte */
  uint32_t biXPelsPerMeter; /* 0 */
  uint32_t biYPelsPerMeter; /* 0 */
  uint32_t biClrUsed;       /* num of entries of the color table */
  uint32_t biClrImportant;  /* 0 or number of used collors in img */
} BMPHeader;


/**
 * The BMP structur
 */
typedef struct {
  BMPHeader head;
  char     *body;
 } BMP;

// functions
void init_RGB_BMPHeader(BMPHeader *head, uint32_t width, uint32_t height);
int bmp_write_header(BMPHeader *head, int fd);
#endif // __BMP_H__
