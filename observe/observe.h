#ifndef OBSERVE_H
#define OBSERVE_H

#include "enumerate.h"
#include "stats.h"
#include "io.h"
#include "status.h"

#define BUFFER_CAPACITY 8192

#define READ_BUFFER_SIZE 8192

typedef struct Buffer {
  char *data;
  size_t size;
  size_t capacity;
} Buffer;

typedef struct Monitor {
  Process process;

  CPUStats cpu;
  MemoryStats memory;
  IOStats io;
  SignalStats signals;

  ProcHandles proc_files;

  Buffer stat_buffer;
  Buffer status_buffer;
  Buffer io_buffer;
} Monitor;

Monitor monitor_create(const char *p_name, char **argv);
void monitor_start_process(Process *process);
void monitor_wait_process(Process *process);
void monitor_cleanup(Monitor *monitor);
void monitor_loop(Monitor *monitor);

Buffer buffer_create();
void buffer_cleanup(Buffer *buf);
void buffer_grow(Buffer *buf);

void load_and_read_process_data(Monitor *mon, Buffer *buf, int fd);

float get_cpu_percent(float delta_time, Sample *user_time, Sample *system_time, long hz);

#endif