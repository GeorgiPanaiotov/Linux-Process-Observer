#ifndef SAMPLE_H
#define SAMPLE_H

#include <stdlib.h>
#include <stdint.h>

typedef struct Sample
{
  long previous;
  long current;
} Sample;

typedef struct SampleLL
{
  long long previous;
  long long current;
} SampleLL;

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
} CPUStats;

typedef struct SignalStats
{
  long queued;
  long limit;
  uint64_t pending_thread;
  uint64_t pending_process;
  uint64_t blocked;
  uint64_t ignored;
  uint64_t caught;
} SignalStats;

typedef struct MemoryStats
{
  Sample rss;
  Sample peak;
  Sample shared;
  Sample swap;
  Sample vsize;
} MemoryStats;

typedef struct IOStats
{
  SampleLL read_bytes;
  SampleLL write_bytes;
  SampleLL syscr;
  SampleLL syscw;
  SampleLL canceled_write_bytes;
  SampleLL rchar;
  SampleLL wchar;
  long long fd_size;
} IOStats;

Sample sample_create();
SampleLL samplell_create();
MemoryStats memory_stats_create();
CPUStats cpu_stats_create();
IOStats io_stats_create();
SignalStats signal_stats_create();
void sample_update(Sample *sample, long value);
void samplell_update(SampleLL *sample, long long value);

#endif