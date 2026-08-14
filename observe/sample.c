#include "sample.h"

Sample sample_create()
{
  Sample sample;
  sample.current = 0;
  sample.previous = 0;

  return sample;
}

MemoryStats memory_stats_create()
{
  MemoryStats ms;
  ms.peak = 0;
  ms.rss = 0;
  ms.shared = 0;
  ms.swap = 0;
  ms.vsize = 0;

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
  cs.nv_ctx_switches = 0;
  cs.v_ctx_switches = 0;

  return cs;
}

SignalStats signal_stats_create()
{
  SignalStats ss;
  ss.blocked = 0;
  ss.caught = 0;
  ss.ignored = 0;
  ss.pending_process = 0;
  ss.pending_thread = 0;
  ss.signals = NULL;

  return ss;
}

IOStats io_stats_create()
{
  IOStats is;
  is.canceled_write_bytes = 0;
  is.rchar = 0;
  is.read_bytes = 0;
  is.syscr = 0;
  is.syscw = 0;
  is.wchar = 0;
  is.write_bytes = 0;
  is.fd_size = 0;

  return is;
}

void sample_update(Sample *sample, long value)
{
  sample->previous = sample->current;
  sample->current = value;
}
