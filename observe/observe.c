#include "observe.h"

Monitor monitor_create(const char *p_name, char **argv)
{
  Monitor monitor;
  monitor.process = process_create(p_name, argv);
  monitor.memory = memory_stats_create();
  monitor.cpu = cpu_stats_create();
  monitor.io = io_stats_create();
  monitor.signals = signal_stats_create();
  monitor.stat_buffer = buffer_create();
  monitor.status_buffer = buffer_create();
  monitor.io_buffer = buffer_create();

  monitor_start_process(&monitor.process);

  monitor.proc_files = get_proc_files(monitor.process.process_pid);

  return monitor;
}

void monitor_start_process(Process *process)
{
  process->process_pid = process_spawn(process);
}

void monitor_wait_process(Process *process)
{
  process_wait(process);
}

void monitor_cleanup(Monitor *monitor)
{
  close_proc_files(&monitor->proc_files);
  process_cleanup(&monitor->process);

  monitor->cpu = (CPUStats){};
  monitor->memory = (MemoryStats){};
  monitor->proc_files = (ProcHandles){};
  monitor->process = (Process){};
  monitor->io = (IOStats){};
  monitor->signals = (SignalStats){};

  buffer_cleanup(&monitor->stat_buffer);
  buffer_cleanup(&monitor->status_buffer);
  buffer_cleanup(&monitor->io_buffer);
}

void load_and_read_process_data(Monitor *mon, Buffer *buf, int fd)
{
  ssize_t read_buffer;
  buf->size = 0;
  lseek(fd, 0, SEEK_SET);

  while ((read_buffer = read(fd, buf->data + buf->size, READ_BUFFER_SIZE)) > 0)
  {
    if (read_buffer == -1)
    {
      perror("load_and_read_process_data: read");
      monitor_cleanup(mon);
      exit(EXIT_FAILURE);
    }
    buf->size += read_buffer;
    if (buf->size + READ_BUFFER_SIZE >= buf->capacity)
    {
      buffer_grow(buf);
    }
  }

  if (read_buffer == -1)
  {
    perror("load_and_read_process_data: read");
    monitor_cleanup(mon);
    exit(EXIT_FAILURE);
  }

  buf->data[buf->size] = '\0';
}

float get_cpu_percent(float delta_time, Sample *user_time, Sample *system_time, long hz)
{
  float elapsed = delta_time;
  if (delta_time <= 0)
  {
    elapsed = delta_time;
  }
  long delta = (user_time->current - user_time->previous) + (system_time->current - system_time->previous);
  float cpu_seconds = (float)delta / hz;
  float cpu_percent = cpu_seconds / elapsed * 100.0f;
  return cpu_percent;
}

void monitor_loop(Monitor *monitor)
{
  load_and_read_process_data(monitor, &monitor->stat_buffer, monitor->proc_files.stat_fd);
  parse_stats(&monitor->memory, &monitor->cpu, monitor->stat_buffer.data);

  load_and_read_process_data(monitor, &monitor->status_buffer, monitor->proc_files.status_fd);
  parse_status(&monitor->memory, &monitor->cpu, &monitor->io, &monitor->signals, monitor->status_buffer.data);

  load_and_read_process_data(monitor, &monitor->io_buffer, monitor->proc_files.io_fd);
  parse_io(&monitor->io, monitor->io_buffer.data);

  update_process_running_status(&monitor->process);
}

Buffer buffer_create()
{
  Buffer buf;
  buf.capacity = BUFFER_CAPACITY;
  buf.size = 0;
  buf.data = malloc(buf.capacity + 1);

  if (buf.data == NULL)
  {
    perror("buffer_create: malloc");
    exit(EXIT_FAILURE);
  }

  buf.data[buf.capacity] = '\0';
  return buf;
}

void buffer_cleanup(Buffer *buf)
{
  free(buf->data);
  buf->capacity = 0;
  buf->size = 0;
}

void buffer_grow(Buffer *buf)
{
  buf->capacity *= 2;
  char *temp_buf = realloc(buf->data, buf->capacity + 1);
  if (temp_buf == NULL)
  {
    perror("buffer_grow: realloc");
    exit(EXIT_FAILURE);
  }

  buf->data = temp_buf;
  buf->data[buf->capacity] = '\0';
}
