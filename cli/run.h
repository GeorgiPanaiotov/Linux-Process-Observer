#ifndef RUN_H
#define RUN_H

#include <time.h>
#include <unistd.h>

#include "observe.h"
#include "config.h"
#include "tracer.h"

void start_run(const char *program_name, char **c_argv);
void print_stats(Monitor *monitor);
void print_io(Monitor *monitor);
void print_status(Monitor *monitor);
void print_status_signals(Monitor *monitor);
void print_cpu_switches(Monitor *monitor);

void print_tracer(char *buffer);
void format_bytes(char *buf, size_t buf_size, long bytes);
void print_memory(Monitor *monitor, Config *config, char *rss_buf, char *peak_buf, char *swap_buf, char *shared_buf, char *vsize_buf, size_t rss_size, size_t peak_size, size_t swap_size, size_t shared_size, size_t vsize_size);

#endif