/* simulate.c - Simulation Script */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MARSS_START_SIM() asm("csrs	0x800,zero")
#define MARSS_STOP_SIM() asm("csrs	0x801,zero")

#define BUG_ON(err)                                                            \
  if (err) {                                                                   \
    fprintf(stderr, "Error occured on execv: %d\n", (err));                    \
    exit(1);                                                                   \
  }

static void
usage(const char* prog)
{
  printf("%s <absolute-path-of-command>\n", prog);
}

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    usage(argv[0]);
    exit(1);
  }

  pid_t pid;
  int err;

  switch (pid = fork()) {
    case 0:
      /* Child enters simulation mode and execs the command */
      /* Note :
      Performance stats generated at the end of simualtion
      will also include stats for execve
      */
      MARSS_START_SIM();
      err = execv(argv[1], &argv[1]);
      BUG_ON(err);
      break;
    case -1: /* Fail */
      fprintf(stderr, "%s\n", "fork() error");
      exit(1);
      break;
    default: /* Parent */
      wait(&err);
      /* Parent waits for child to complete command execution and then enters
       * emulation mode*/
      MARSS_STOP_SIM();
  }
  return 0;
}