/**
 *  A realocable shared memory which updates its size 
 *  in the current process during access
 */
#ifndef __GROWING-SHARED-MEMORY_H__
#define __GROWING-SHARED-MEMORY_H__

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../../Dynamic-Array/src/dynamic-array.h"

/**
 * union to acces one Page
 */
typedef union {
  int     *ids;
  uint8_t *ptr;
  void    *v_ptr;
} Page;

/**
 * the local page address array
 */
DEF_ARY(Page, PageAry);

/**
 * the Growing Shared Memory structur
 */
typedef struct {
  PageAry  pages;
  uint64_t length; /* the usabel length */
  uint64_t n_pages;
  uint64_t page_size;
  uint64_t n_i_pages; /* number of Pages per memory segement */
  uint64_t id_size;
} GSMemory;

/**
 * return the shmid at the given index
 */
#define gsm_get_shmid(gsm, i)                                            \
  ARY_AT((gsm)->pages, i / (gsm)->n_i_pages).ids[i % (gsm)->n_i_pages]

/**
 * returns the shem id which should indecate the end of the shimds
 * (the shemid after the last shemid)
 */
#define gsm_end_id(gsm) gsm_get_shmid(gsm, (gsm)->n_pages)

/**
 * updates the GSMemory if needed
 */
#define gsm_update(gsm)                                           \
do {                                                              \
  /* as long as there is a new id */                              \
  while (gsm_end_id(gsm) != -1) {                                 \
    Page page;                                                    \
    page.v_ptr = shmat(gsm_end_id(gsm), 0, 0);                    \
                                                                  \
    ARY_ADD((gsm)->pages, page);                                  \
    (gsm)->n_pages++;                                             \
                                                                  \
    /* the id after last marks the end */                         \
    (gsm)->id_size = ((gsm)->n_pages + 1) * sizeof(Page);         \
    (gsm)->length  = (gsm)->n_pages * (gsm)->page_size -          \
                     (gsm)->id_size;                              \
  }                                                               \
} while (0)

/**
 * grows the GSMemory about one page
 */
#define gsm_grow(gms)                                                         \
do {                                                                          \
  gsm_end_id(gsm) = shmget(IPC_PRIVATE, (gsm)->page_size, IPC_CREAT|0666);    \
                                                                              \
  if (gsm_end_id(gsm) == -1) {                                                \
    perror("failed to grow GSMemory");                                        \
    exit(EXIT_FAILURE);                                                       \
  }                                                                           \
                                                                              \
  gsm_update(gms);                                                            \
  gsm_end_id(gsm) = -1;                                                       \
} while (0)


/**
 * calculates the page containing the given index
 */
#define gsm_p_num(gsm, i) ((i + (gms)->id_size) / (gsm)->page_size)

/**
 * calcutaest the possition the the actual page for the given index
 */
#define gsm_p_index(gsm, i) ((i + (gms)->id_size) % (gsm)->page_size)

/**
 * writes one byte to the array
 * Note: byte should be a uint8_t
 */
#define gsm_write_byte(gsm, byte, i)                                          \
do {                                                                          \
  while ((gsm)->length < i)                                                   \
    gsm_grow(gsm);                                                            \
                                                                              \
  ARY_AT((gsm)->pages, gsm_p_num(gsm, i)).ptr[gsm_p_index(gsm, i)] = byte;    \
                                                                              \
} while (0)

/**
 * writes up to n bytes from ptr at the given possition
 * Note: ptr has to be an uint8_t pointer
 */
static inline void gsm_write(GSMemory *gsm, 
                             uint8_t *ptr, 
                             size_t n, 
                             uint64_t i) {
  
  size_t j;
  for (j = 0; j < n; j++, i++)
    gsm_write_byte(gsm, ptr[i], i);

}

/**
 * initializes a Growing Shared Memory with a given start length
 * Note this should be done bevor fork
 */
static inline void gsm_init(GSMemory *gsm, uint64_t len) {
                                                                              
  gsm->page_size = sysconf(_SC_PAGESIZE);                                   
  gsm->n_pages   = 1;                                                       
  gsm->id_size   = 2 * sizeof(Page);                                        
  gsm->length    = gsm->page_size  - gsm->id_size;                      
  gsm->n_i_pages = gsm->page_size / sizeof(Page);                         
                                                                              
  Page start;                                                                 
                                                                              
  int shmid    = shmget(IPC_PRIVATE, gsm->page_size, IPC_CREAT|0666);       
  start.v_ptr  = shmat(shmid, 0, 0);                                          
                                                                              
  start.ids[0] = shmid;                                                       
  start.ids[1] = -1;                                                          
                                                                              
  ARY_ADD(gsm->pages, start);                                                 
                                                                              
  while (gsm->length < len)                                                 
    gsm_grow(gsm);                                                            
} 





#endif /* __GROWING-SHARED-MEMORY_H__ */
