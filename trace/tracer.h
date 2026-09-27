#ifndef TRACER_H
#define TRACER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <poll.h>
#include <errno.h>

#define READ_CHUNK_SIZE 8192

void write_trace_settings(const char *path, const char *value, int flags);
void tracer_create(pid_t pid);
void tracer_cleanup();
int tracer_open_pipe();
void read_tracer(char *buffer, size_t buffer_size);
void exhaust_pipe(char *buffer, size_t buffer_size);

#endif