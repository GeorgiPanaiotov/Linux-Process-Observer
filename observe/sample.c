#include "sample.h"

Sample sample_create()
{
  Sample sample;
  sample.current = 0;
  sample.previous = 0;

  return sample;
}

SampleLL samplell_create()
{
  SampleLL sample;
  sample.current = 0;
  sample.previous = 0;

  return sample;
}

MemoryStats memory_stats_create()
{
  MemoryStats ms;
  Sample new_sample = sample_create();
  ms.peak = new_sample;
  ms.rss = new_sample;
  ms.shared = new_sample;
  ms.swap = new_sample;

  return ms;
}

CPUStats cpu_stats_create()
{
  CPUStats cs;
  Sample new_sample = sample_create();
  cs.n_threads = 0;
  cs.nice = 0;
  cs.priority = 0;
  cs.processor = 0;
  cs.start_time = 0;
  cs.state = NULL;
  cs.system_time = new_sample;
  cs.user_time = new_sample;

  return cs;
}

IOStats io_stats_create()
{
  IOStats is;
  SampleLL new_sample = samplell_create();
  is.canceled_write_bytes = new_sample;
  is.rchar = new_sample;
  is.read_bytes = new_sample;
  is.syscr = new_sample;
  is.syscw = new_sample;
  is.wchar = new_sample;
  is.write_bytes = new_sample;

  return is;
}

void sample_update(Sample *sample, long value)
{
  sample->previous = sample->current;
  sample->current = value;
}

void samplell_update(SampleLL *sample, long long value)
{
  sample->previous = sample->current;
  sample->current = value;
}
