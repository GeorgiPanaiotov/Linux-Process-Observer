#include "logger.h"

Logger logger_create(const char *path)
{
  Logger logger;
  logger.path = path;
  logger.log_file_fd = -1;

  char time_str[20];
  struct tm *local = get_time();

  if (local != NULL)
  {
    strftime(time_str, sizeof(time_str), "%Y_%m_%d_%H_%M", local);
    snprintf(logger.file_name, sizeof(logger.file_name), "%swhy.log", time_str);
  }
  else
  {
    snprintf(logger.file_name, sizeof(logger.file_name), "whydefault.log");
  }

  return logger;
}

struct tm *get_time()
{
  struct timespec ts;

  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
  {
    perror("get_time: clock_gettime");
    return NULL;
  }

  struct tm *local = localtime(&ts.tv_sec);
  if (local == NULL)
  {
    perror("get_time: localtime");
    return NULL;
  }

  return local;
}

void logger_file_create(Logger *logger)
{
  char full_path[PATH_MAX];
  snprintf(full_path, sizeof(full_path), "%s/%s", logger->path, logger->file_name);
  printf("%s\n", full_path);

  int fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);
  if (fd == -1)
  {
    perror("logger_file_create: open");
    exit(EXIT_FAILURE);
  }

  logger->log_file_fd = fd;
}
