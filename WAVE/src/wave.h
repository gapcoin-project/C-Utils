/**
 * Header for some BMP creation functions
 */
#ifndef __WAVE_H__
#define __WAVE_H__
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <inttypes.h>

/**
 * The header structur 
 */
typedef struct {
  char     *riff;              /* 'RIFF' */
  uint32_t size;               /* File size - 8 */
  char     *wave;              /* 'WAV'E */
  char     *fmt;               /* 'fmt ' */
  uint32_t fmt_size;           /* fmt-Header size (16 Bytes)*/
  uint16_t format;             /* format tag */
  #define  WAVE_PCM  0x0001    /* PCM WAVE-Format */
  uint16_t channel;            /* channels 1=mono, 2=stereo*/
  uint16_t sample_rate;        /* samples per sec */
  uint16_t bytes_per_sec;      /* sample_rate * frame_size */
  uint16_t frame_size;         /* channels * ((bits/sample + 7) / 8) */
  uint16_t bits_per_sample;    /* bits/sample per channels */
  char     *data;              /* 'data' */
  uint32_t data_length;        /* data length */
} WAVEHeader;


/**
 * The WAVE structur
 */
typedef struct {
  WAVEHeader head;
  char     *body;
 } WAVE;

// functions
void init_WAVEHeader(WAVEHeader *head, uint32_t data_size);
int wave_write_header(WAVEHeader *head, int fd);
#endif // __WAVE_H__
