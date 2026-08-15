#ifndef TRACER_H
#define TRACER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <errno.h>

void write_tracefs(const char *path, const char *value, int flags);
void tracer_create(pid_t pid);
void tracer_cleanup();
int tracer_open_pipe();
void read_tracer();

#endif