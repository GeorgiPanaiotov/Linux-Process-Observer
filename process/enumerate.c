#include "enumerate.h"

DIR *get_proc()
{
  DIR *dir = opendir("/proc");
  if (dir == NULL)
  {
    printf("Unable to open proc!\n");
    perror("get_proc: opendir");
    exit(EXIT_FAILURE);
  }
  return dir;
}

void close_dir(DIR *dirp)
{
  int dir_status = closedir(dirp);

  if (dir_status == -1)
  {
    perror("close_dir: closedir");
    exit(EXIT_FAILURE);
  }
}

bool is_pid(const char *dir_name)
{
  while (*dir_name)
  {
    if (!isdigit((unsigned char)*dir_name))
    {
      return false;
    }
    ++dir_name;
  }
  return true;
}

int open_file(char *path)
{
  int fd = open(path, O_RDONLY);

  if (fd == -1)
  {
    perror("open_file: open");
    exit(EXIT_FAILURE);
  }

  return fd;
}

ProcHandles get_proc_files(pid_t pid)
{
  ProcHandles p_files = {0};

  char stat_path[PATH_MAX];
  char status_path[PATH_MAX];
  char io_path[PATH_MAX];
  snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
  snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
  snprintf(io_path, sizeof(io_path), "/proc/%d/io", pid);

  p_files.io_fd = open_file(io_path);
  p_files.stat_fd = open_file(stat_path);
  p_files.status_fd = open_file(status_path);

  return p_files;
}

void close_proc_files(ProcHandles *p_files)
{
  close(p_files->io_fd);
  close(p_files->smaps_fd);
  close(p_files->stat_fd);
  close(p_files->status_fd);
}
