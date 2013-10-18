/**
 *  Implementaion of an fork, shared memory and sigterm based timout
 */
#ifndef __TIMEOUT__
#define __TIMEOUT__

#include "timeout.h"

/* exit by error */
#define perror(MSG)           \
  do {                        \
    perror(MSG);              \
    exit(EXIT_FAILURE);       \
  } while(0)

/**
 * Process an function wih an agressive Timout
 * the given funktion pointer will pe brocessed in
 * a seperate process so if it will be canceld 
 * by timout the kernle frees unfreed memory
 *
 * time is the time to process func
 * func is an function pointer which gets
 *      a pointer to a shared memory buffer
 *      and a pointer to own args
 * shm_size is the size that buffer should have
 * args ar given as second parameter to func
 *
 * it returns the pointer to the shared memory
 *
 * Note: any changes on process memory in func
 *       will be discard so use the shared memory
 *       for return values and other memory changes
 */
void *with_timeout_do(struct timeval *time,
                      void (*func)(void *, void *),
                      size_t shm_size, void *args) {

  /* timer value */                                     
  struct itimerval __timer_val__ = { { 0,  0 }, *time }; 
                                                       
  /** 
   * create and map shared memory into own addresspace 
   */
  uint8_t *buffer = mmap(NULL, 
                         shm_size + 1, 
                         PROT_WRITE | PROT_READ, 
                         MAP_ANONYMOUS | MAP_SHARED,
                         -1,
                         0);

  if (buffer == MAP_FAILED)
    perror("failed to map shared memory");

  /* using first byte for sharing status */
  uint8_t *finished = buffer;
  *finished = 0;


  /* create new process */
	pid_t pid = fork();

  /* new process */
	if ( pid == 0 ) {

    /* reset timer siganl handler */                  
    signal(SIGALRM, SIG_DFL);                        
                                                    
    /* start timer */                              
    setitimer(ITIMER_REAL, &__timer_val__, NULL);  
 
    func(buffer + 1, args);
    *finished = 1;
    exit(1);
	
  /* old process */
  } else if (pid > 0)
    waitpid(pid, NULL, 0);
	else if (pid == -1)
		perror("fork failed");

  
  /* return value */
  void *ret = NULL;

  /* copy return if timeout was not reatched */
  if (*finished == 1) {
    
    ret = malloc(shm_size);
    memcpy(ret, buffer + 1, shm_size);

  }

  /**
   * unmap shared memory
   */
  if (munmap(buffer, shm_size + 1) == -1)
    perror("failed to unmap shared memory");

  return (void *) ret;

}                      


  

#endif /* __TIMEOUT__ */
