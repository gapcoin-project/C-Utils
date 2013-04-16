/**
 * Some WAVE creation function
 */
#ifndef __WAVE__
#define __WAVE__
#include "wave.h"

/**
 * initializes an given WAVE header with an given data size
 * sets ale other values to default
 */
void init_WAVEHeader(WAVEHeader *head, uint32_t data_size) {

  // init Head
  head->riff                = "RIFF";
  head->size                = data_size + 44 - 8;
  head->wave                = "WAVE";
  head->fmt                 = "fmt ";
  head->fmt_size            = 16;
  head->format              = WAVE_PCM;
  head->channel             = 2;
  head->sample_rate         = 44100;
  head->bits_per_sample     = 16;
  head->frame_size          = head->channel 
                              * ((head->bits_per_sample + 7) / 8);
  head->bytes_per_sec       = head->sample_rate * head->frame_size;
  head->data                = "data";
  head->data_length         = data_size;
  
 }

/**
 * Wites the given WAVE header to the given file descriptor
 */
int wave_write_header(WAVEHeader *head, int fd) {

  if(write(fd, head->riff,             4) == -1) return -1;
  if(write(fd, &head->size,            4) == -1) return -1;
  if(write(fd, head->wave,             4) == -1) return -1;
  if(write(fd, head->fmt,              4) == -1) return -1;
  if(write(fd, &head->fmt_size,        4) == -1) return -1;
  if(write(fd, &head->format,          2) == -1) return -1;
  if(write(fd, &head->channel,         2) == -1) return -1;
  if(write(fd, &head->sample_rate,     4) == -1) return -1;
  if(write(fd, &head->bytes_per_sec,   4) == -1) return -1;
  if(write(fd, &head->frame_size,      2) == -1) return -1;
  if(write(fd, &head->bits_per_sample, 2) == -1) return -1;
  if(write(fd, head->data,             4) == -1) return -1;
  if(write(fd, &head->data_length,     4) == -1) return -1;

  return 1;

} 
#endif // __WAVE__
