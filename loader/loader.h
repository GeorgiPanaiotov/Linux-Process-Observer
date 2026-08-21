#ifndef LOADER_H
#define LOADER_H

#include <fcntl.h>
#include <unistd.h>
#include <linux/bpf.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <stdio.h>
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <string.h>
#include <errno.h>
#include <sys/sysinfo.h>

int load_ebpf_program(const char *path);
int attach_to_all_cpus(int prog_fd, int event_id);
void attach_event_program(int *program_fd, int *event_fd);

#endif