#ifndef FD_H
#define FD_H

#include <dirent.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define FDBUFFER_SIZE (256 * 1024)

typedef struct FDBuffer
{
  char *data;
  size_t offset;
} FDBuffer;

void read_fd_contents(int *fd, pid_t pid, FDBuffer *buf);
FDBuffer fdbuffer_create();
void fdbuffer_cleanup(FDBuffer *buf);
void fd_buffer_reset(FDBuffer *buf);

#endif