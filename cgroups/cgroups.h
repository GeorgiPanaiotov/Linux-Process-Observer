#ifndef CGROUPS_H
#define CGROUPS_H

#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

void create_cgroup(pid_t pid);
void enable_cgroup_controllers(void);
void cleanup_cgroup(void);

#endif
