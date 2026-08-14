#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct Sample
{
  long previous;
  long current;
} Sample;

typedef struct CPUStats
{
  Sample user_time;
  Sample system_time;
  long long start_time;
  int processor;
  long priority;
  long nice;
  char *state;
  long n_threads;
  long v_ctx_switches;
  long nv_ctx_switches;
} CPUStats;

typedef struct SignalStats
{
  char *signals;
  uint64_t pending_thread;
  uint64_t pending_process;
  uint64_t blocked;
  uint64_t ignored;
  uint64_t caught;
} SignalStats;

typedef struct MemoryStats
{
  long rss;
  long peak;
  long shared;
  long swap;
  long vsize;
} MemoryStats;

typedef struct IOStats
{
  long long read_bytes;
  long long write_bytes;
  long long syscr;
  long long syscw;
  long long canceled_write_bytes;
  long long rchar;
  long long wchar;
  long long fd_size;
} IOStats;

Sample sample_create();
MemoryStats memory_stats_create();
CPUStats cpu_stats_create();
IOStats io_stats_create();
SignalStats signal_stats_create();
void sample_update(Sample *sample, long value);

#endif