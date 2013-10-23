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
 * static value that indecates if an seteted timout was reatched
 */
static char soft_timout_reatched = 0;

/**
 * signal handler seting soft_timout_reatched
 * on SIGALRM
 */
static void soft_timout_sighandler(int sigum) {
  if (sigum == SIGALRM)
    soft_timout_reatched = 1;
}

/**
 * returns wether a setted timout was reatched
 */
char soft_timouted() { return soft_timout_reatched; }

/**
 * soft timout using an sigalarm handler 
 * and a static value
 * negativ timout let the timout never expire
 */
void set_soft_timout(int64_t usec) {
  
  static char not_init = 1;

  /* init siganal handler at the first call */
  if (not_init) {

    static struct sigaction new_action;
    memset(&new_action, 0, sizeof(struct sigaction));
 
    new_action.sa_handler = soft_timout_sighandler;
    sigaction(SIGALRM, &new_action, NULL);

    not_init = 0;
  }

  /* reset timout indecator */
  soft_timout_reatched = 0;

  /* never expire if negative timout given */
  if (usec < 0) return;
  
  struct itimerval timout = { { 0, 0}, { usec / 1000000, usec % 1000000 } };

  setitimer(ITIMER_REAL, &timout, NULL);  
}


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
  struct itimerval timer = { { 0,  0 }, *time }; 
                                                       
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
    setitimer(ITIMER_REAL, &timer, NULL);  
 
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
