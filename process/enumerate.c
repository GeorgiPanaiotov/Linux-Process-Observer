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
  char fd_path[PATH_MAX];
  snprintf(stat_path, sizeof(stat_path), "/proc/%d/stat", pid);
  snprintf(status_path, sizeof(status_path), "/proc/%d/status", pid);
  snprintf(io_path, sizeof(io_path), "/proc/%d/io", pid);
  snprintf(fd_path, sizeof(fd_path), "/proc/%d/fd/", pid);

  p_files.io_fd = open_file(io_path);
  p_files.stat_fd = open_file(stat_path);
  p_files.status_fd = open_file(status_path);
  p_files.fddir_fd = open_file(fd_path);

  return p_files;
}

void close_proc_files(ProcHandles *p_files)
{
  close(p_files->io_fd);
  close(p_files->smaps_fd);
  close(p_files->stat_fd);
  close(p_files->status_fd);
  close(p_files->fddir_fd);
}

CgroupHandles get_cgroup_proc_files()
{
  CgroupHandles cgrp_files = {0};

  cgrp_files.procs_fd = open_file("/sys/fs/cgroup/lbx/cgroup.procs");
  cgrp_files.cpu_stat_fd = open_file("/sys/fs/cgroup/lbx/cpu.stat");
  cgrp_files.mem_current_fd = open_file("/sys/fs/cgroup/lbx/memory.current");
  cgrp_files.mem_stat_fd = open_file("/sys/fs/cgroup/lbx/memory.stat");
  cgrp_files.io_stat_fd = open_file("/sys/fs/cgroup/lbx/io.stat");

  return cgrp_files;
}

void close_cgroup_proc_files(CgroupHandles *cgrp_files)
{
  close(cgrp_files->procs_fd);
  close(cgrp_files->cpu_stat_fd);
  close(cgrp_files->mem_stat_fd);
  close(cgrp_files->mem_current_fd);
  close(cgrp_files->io_stat_fd);
}