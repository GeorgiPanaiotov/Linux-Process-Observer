#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>

typedef struct Logger
{
  const char *path;
  char file_name[64];
  int log_file_fd;
} Logger;

Logger logger_create(const char *path);
struct tm *get_time();
void logger_file_create(Logger *logger);

#endif