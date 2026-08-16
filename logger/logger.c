#include "logger.h"

static const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB"};

Logger logger_create(const char *path)
{
  Logger logger;
  logger.path = path;
  logger.log_file_fd = -1;

  char time_str[20];
  struct tm *local = get_time();

  if (local != NULL)
  {
    strftime(time_str, sizeof(time_str), "%Y_%m_%d_%H_%M", local);
    snprintf(logger.file_name, sizeof(logger.file_name), "%swhy.log", time_str);
  }
  else
  {
    snprintf(logger.file_name, sizeof(logger.file_name), "whydefault.log");
  }

  return logger;
}

struct tm *get_time()
{
  struct timespec ts;

  if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
  {
    perror("get_time: clock_gettime");
    return NULL;
  }

  struct tm *local = localtime(&ts.tv_sec);
  if (local == NULL)
  {
    perror("get_time: localtime");
    return NULL;
  }

  return local;
}

void logger_file_create(Logger *logger)
{
  char full_path[PATH_MAX];
  snprintf(full_path, sizeof(full_path), "%s/%s", logger->path, logger->file_name);

  int fd = open(full_path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IRGRP | S_IROTH);
  if (fd == -1)
  {
    perror("logger_file_create: open");
    exit(EXIT_FAILURE);
  }

  logger->log_file_fd = fd;
}

void logger_log_data(Monitor *monitor, Config *config, Logger *logger, float cpu_percent)
{
  if (logger->log_file_fd < 0)
  {
    return;
  }

  get_formatted_timestamp(monitor->logs.timestamp, sizeof(monitor->logs.timestamp));

  format_bytes(monitor->logs.rss_buf, monitor->logs.rss_buf_size, monitor->memory.rss * config->page_size);
  format_bytes(monitor->logs.peak_buf, monitor->logs.peak_buf_size, monitor->memory.peak * 1024);
  format_bytes(monitor->logs.swap_buf, monitor->logs.swap_buf_size, monitor->memory.swap * 1024);
  format_bytes(monitor->logs.shared_buf, monitor->logs.share_buf_size, monitor->memory.shared * 1024);
  format_bytes(monitor->logs.vsize_buf, monitor->logs.vsize_buf_size, monitor->memory.vsize * 1024);

  char header[2048];
  int header_len = snprintf(header, sizeof(header),
                            "[%s]\n"
                            "%-50s %.3f %%\n"
                            "%-50s %s\n"
                            "%-50s %s\n"
                            "%-50s %s\n"
                            "%-50s %s\n"
                            "%-50s %s\n"
                            "%-50s %ld\n"
                            "%-50s %d\n"
                            "%-50s %ld\n"
                            "%-50s %s\n"
                            "%-50s %ld ticks\n"
                            "%-50s %ld ticks\n"
                            "%-50s %ld\n"
                            "%-50s %lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %'lld\n"
                            "%-50s %ld\n"
                            "%-50s %ld\n"
                            "%-50s %s\n\n"
                            "--- TRACER BUFFER ---\n",
                            monitor->logs.timestamp,
                            "CPU: ", (cpu_percent / (float)config->n_processors),
                            "RSS: ", monitor->logs.rss_buf,
                            "VM Peak: ", monitor->logs.peak_buf,
                            "VM Swap: ", monitor->logs.swap_buf,
                            "VM Shared: ", monitor->logs.shared_buf,
                            "VM Size: ", monitor->logs.vsize_buf,
                            "Threads: ", monitor->cpu.n_threads,
                            "Processor #: ", monitor->cpu.processor,
                            "Priority: ", monitor->cpu.priority,
                            "State: ", monitor->cpu.state,
                            "System Time: ", monitor->cpu.system_time.current,
                            "User Time: ", monitor->cpu.user_time.current,
                            "Nice: ", monitor->cpu.nice,
                            "Start Time: ", monitor->cpu.start_time,
                            "File Descriptor slots currently allocated: ", monitor->io.fd_size,
                            "Successfull Bytes from Read: ", monitor->io.rchar,
                            "Successfull Bytes from Write: ", monitor->io.wchar,
                            "Number of 'File Read' Syscalls: ", monitor->io.syscr,
                            "Number of 'File Write' Syscalls: ", monitor->io.syscw,
                            "Number of Bytes really fetched from Storage: ", monitor->io.read_bytes,
                            "Number of Bytes really sent to Storage: ", monitor->io.write_bytes,
                            "Cancelled Write Bytes: ", monitor->io.canceled_write_bytes,
                            "Voluntary Context Switches: ", monitor->cpu.v_ctx_switches,
                            "Non-Voluntary Context Switches: ", monitor->cpu.nv_ctx_switches,
                            "Signals queued/Signals limit: ", monitor->signals.signals);

  struct iovec iov[3];
  iov[0].iov_base = header;
  iov[0].iov_len = header_len;

  iov[1].iov_base = monitor->logs.tracer_buffer;
  iov[1].iov_len = strlen(monitor->logs.tracer_buffer);

  iov[2].iov_base = "\n========================================\n\n";
  iov[2].iov_len = 43;

  writev(logger->log_file_fd, iov, 3);
}

void logger_cleanup(Logger *logger)
{
  close(logger->log_file_fd);
  logger->file_name[0] = '\0';
  logger->path = NULL;
}

void get_formatted_timestamp(char *buf, size_t len)
{
  struct timespec ts;
  struct tm tm_info;

  clock_gettime(CLOCK_REALTIME, &ts);

  localtime_r(&ts.tv_sec, &tm_info);

  int ms = ts.tv_nsec / 1000000;

  size_t off = strftime(buf, len, "%Y-%m-%d %H:%M:%S", &tm_info);

  if (off > 0 && off < len)
  {
    snprintf(buf + off, len - off, ".%03d", ms);
  }
}

void format_bytes(char *buf, size_t buf_size, long bytes)
{
  int i = 0;
  double d_bytes = (double)bytes;

  while (d_bytes >= 1024 && i < 5)
  {
    d_bytes /= 1024;
    i++;
  }

  if (i == 0)
  {
    snprintf(buf, buf_size, "%'ld %s", bytes, suffixes[i]);
  }
  else
  {
    snprintf(buf, buf_size, "%'.3f %s", d_bytes, suffixes[i]);
  }
}