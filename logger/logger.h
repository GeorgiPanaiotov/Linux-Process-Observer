#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <unistd.h>
#include <string.h>

#include "observe.h"
#include "config.h"

typedef struct Logger
{
  const char *path;
  char file_name[64];
  int log_file_fd;
} Logger;

Logger logger_create(const char *path);
struct tm *get_time();
void logger_file_create(Logger *logger);
void logger_log_data(Monitor *monitor, Config *config, Logger *logger, float cpu_percent);
void logger_cleanup(Logger *logger);
void get_formatted_timestamp(char *buf, size_t len);
void format_bytes(char *buf, size_t buf_size, long bytes);

#endif