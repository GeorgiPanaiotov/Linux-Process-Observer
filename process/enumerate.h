#ifndef ENUMERATE_H
#define ENUMERATE_H

#include "process.h"

#include <dirent.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

typedef struct ProcHandles {
  int stat_fd;
  int status_fd;
  int io_fd;
  int smaps_fd;
  int fddir_fd;
} ProcHandles;

typedef struct CgroupHandles {
  int procs_fd;
  int cpu_stat_fd;
  int mem_stat_fd;
  int mem_current_fd;
  int io_stat_fd;
} CgroupHandles;

DIR *get_proc();
void close_dir(DIR *dirp);
bool is_pid(const char *dir_name);
ProcHandles get_proc_files(pid_t pid);
CgroupHandles get_cgroup_proc_files();
void close_proc_files(ProcHandles *p_files);
void close_cgroup_proc_files(CgroupHandles *cgrp_files);
int open_file(char *path);

#endif