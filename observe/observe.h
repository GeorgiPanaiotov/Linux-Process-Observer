#ifndef OBSERVE_H
#define OBSERVE_H

#include "enumerate.h"
#include "stats.h"
#include "io.h"
#include "status.h"
#include "fd.h"

#define BUFFER_8KB_SIZE 8192
#define BUFFER_32KB_SIZE (32 * 1024)

typedef struct Buffer
{
  char *data;
  size_t size;
  size_t capacity;
} Buffer;

typedef struct Logs
{
  char rss_buf[32];
  char peak_buf[32];
  char swap_buf[32];
  char shared_buf[32];
  char vsize_buf[32];
  char tracer_buffer[BUFFER_32KB_SIZE + 1];
  char timestamp[32];
  char caught_sig_buf[512];
  char blocked_sig_buf[512];
  char pending_t_sig_buf[512];
  char pending_p_sig_buf[512];
  char ignored_sig_buf[512];
} Logs;

typedef struct Monitor
{
  Process process;

  CPUStats cpu;
  MemoryStats memory;
  IOStats io;
  SignalStats signals;

  ProcHandles proc_files;

  Buffer stat_buffer;
  Buffer status_buffer;
  Buffer io_buffer;
  FDBuffer fd_buffer;

  Logs logs;
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
