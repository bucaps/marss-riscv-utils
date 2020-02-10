/*
 * simulate.c - Wrapper for simulating executables inside full system images
 * 		Must be compiled and run inside the guest environment
 *
 * Copyright (c) 2018-2019 Gokturk Yuksek
 *
 * MARSS-RISCV : Micro-Architectural System Simulator for RISC-V
 *
 * Copyright (c) 2017-2019 Gaurav Kothari {gkothar1@binghamton.edu}
 * State University of New York at Binghamton
 *
 * Copyright (c) 2018-2019 Parikshit Sarnaik {psarnai1@binghamton.edu}
 * State University of New York at Binghamton
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
	fprintf(stderr, "Usage: %s <path-to-execuable>\n", prog);
}

int
main(int argc, const char* argv[])
{
  pid_t pid;
  int err;

  if (argc < 2) {
    usage(argv[0]);
    exit(1);
  }

  /* Parent switches to simulation mode */
  MARSS_START_SIM();

  switch (pid = fork()) {
    case 0:
      /* Child execs the command in simulation mode */
      /* Note: Performance stats generated at the end of simualtion
       *       will also include stats for execve
       */
      err = execv(argv[1], &argv[1]);
      BUG_ON(err);
      break;
    case -1: /* Fail */
      fprintf(stderr, "%s\n", "fork() error");
      exit(1);
      break;
    default: /* Parent */
      /*
       * Parent waits for child to complete command execution
       * and then stops the simulation mode
       */ 
      wait(&err);
  }

  /* Parent switches to emulation mode */
  MARSS_STOP_SIM();
  return 0;
}
