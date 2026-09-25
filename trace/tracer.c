#include "tracer.h"

void write_tracefs(const char *path, const char *value, int flags)
{
  int fd = open(path, flags);
  if (fd == -1)
  {
    perror("write_tracefs: open");
    exit(EXIT_FAILURE);
  }

  ssize_t bw = write(fd, value, strlen(value));
  if (bw == -1)
  {
    perror("write_tracefs: write");
    close(fd);
    exit(EXIT_FAILURE);
  }

  close(fd);
}

void tracer_create(pid_t pid)
{
  char str[32];
  snprintf(str, sizeof(str), "%d\n", pid);
  write_tracefs("/sys/kernel/tracing/current_tracer", "nop\n", O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/set_event_pid", str, O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/options/event-fork", "1", O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/events/syscalls/enable", "1\n", O_WRONLY | O_TRUNC);

  write_tracefs("/sys/kernel/tracing/tracing_on", "1\n", O_WRONLY | O_TRUNC);
}

void tracer_cleanup()
{
  write_tracefs("/sys/kernel/tracing/tracing_on", "0", O_WRONLY | O_TRUNC);

  write_tracefs("/sys/kernel/tracing/current_tracer", "nop", O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/events/syscalls/enable", "0", O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/set_event_pid", "\n", O_WRONLY | O_TRUNC);
  write_tracefs("/sys/kernel/tracing/options/event-fork", "0", O_WRONLY | O_TRUNC);
}

int tracer_open_pipe()
{
  int trace_fd = open("/sys/kernel/tracing/trace_pipe", O_RDONLY | O_NONBLOCK);
  if (trace_fd == -1)
  {
    perror("tracer_open_pipe: open");
    tracer_cleanup();
    exit(EXIT_FAILURE);
  }

  return trace_fd;
}

void read_tracer(char *buffer, size_t buffer_size)
{
  int tracer_fd = tracer_open_pipe();
  ssize_t read_buffer;
  ssize_t read_buffer_total = 0;

  while (read_buffer_total < (ssize_t)buffer_size)
  {
    read_buffer = read(tracer_fd, buffer + read_buffer_total, READ_CHUNK_SIZE - read_buffer_total);
    if (read_buffer <= 0)
    {
      break;
    }
    read_buffer_total += read_buffer;
  }

  if (read_buffer == -1)
  {
    if (errno != EAGAIN && errno != EWOULDBLOCK)
    {
      perror("read_tracer: read");
      tracer_cleanup();
      exit(EXIT_FAILURE);
    }
  }

  buffer[read_buffer_total] = '\0';

  close(tracer_fd);
}

void exhaust_pipe(char *buffer, size_t buffer_size)
{
  ssize_t n;
  int tracer_fd = tracer_open_pipe();

  for (;;)
  {
    n = read(tracer_fd, buffer, buffer_size - 1);

    if (n > 0)
    {
      buffer[n] = '\0';
      continue;
    }

    if (n == -1 && errno == EAGAIN)
    {
      break;
    }

    if (n == 0)
    {
      break;
    }

    perror("exhaust_pipe: read");
    break;
  }
  close(tracer_fd);
}
