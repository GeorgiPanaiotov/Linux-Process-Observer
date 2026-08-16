#ifndef PROCESS_H
#define PROCESS_H

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/wait.h>

#include "tracer.h"

typedef struct Process {
  pid_t process_pid;
  pid_t parent_pid;
  const char *p_name;
  bool running;

  char **argv;
} Process;

Process process_create(const char *p_name, char **argv);
pid_t process_spawn(Process *process);
void process_wait(Process *process);
void process_cleanup(Process *process);
void update_process_running_status(Process *process);

#endif