#ifndef RUN_H
#define RUN_H

#include <time.h>

#include "observe.h"
#include "config.h"

void start_run(const char *program_name, char **c_argv);
void print_stats(Monitor *monitor);
void print_io(Monitor *monitor);
void print_status(Monitor *monitor);
void print_status_signals(Monitor *monitor);
void format_bytes(char *buf, size_t buf_size, long bytes);

#endif