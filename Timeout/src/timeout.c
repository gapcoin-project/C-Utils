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
 * Timer function
 * which cancels the process with pid after time
 */
static inline void timer(struct timeval *time, pid_t pid, uint8_t *finished) {

  struct timeval now, end, res;
  gettimeofday(&now, NULL);

  end = now;
  end.tv_sec  += time->tv_sec;
  end.tv_usec += time->tv_usec;

  /**
   * as long as diference is not negative and caller process
   * is running yield
   */
  while (!timeval_subtract(&res, &end, &now) &&
         *finished != 1) {

         sched_yield();
         gettimeofday(&now, NULL);
  }
  
  /** if process isn't finisehd kill it */
  if (*finished != 1) {
    int ret = kill(pid, SIGTERM);

    if (ret == -1)
      perror("failed to kill timouted process");
  }
  
  /* wait for process to be finised */
	waitpid(pid, NULL, 0);

}

/**
 * Process an function wih an agressive Timout
 * th given funktion pointer will pe brocessed in
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

    func(buffer + 1, args);
    *finished = 1;
    exit(1);
	
  /* old process */
  } else if ( pid > 0 )
    timer(time, pid, finished);
	else if ( pid == -1 )
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
