#include "run.h"

static const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB"};

void start_run(const char *program_name, char **c_argv)
{
  Monitor monitor = monitor_create(program_name, c_argv);
  Config config = config_create();

  char rss_buf[32];

  struct timespec prev;
  struct timespec curr;

  clock_gettime(CLOCK_MONOTONIC, &curr);
  prev = curr;

  while (monitor.process.running)
  {
    // CLEARS ENTIRE SCREEN
    // printf("\033[2J\033[H");

    // REDRAWS THE SCREEN
    // printf("\033[H");
    monitor_loop(&monitor);

    clock_gettime(CLOCK_MONOTONIC, &curr);
    float elapsed = (curr.tv_sec - prev.tv_sec) + (curr.tv_nsec - prev.tv_nsec) / 1e9;

    float cpu_percent = get_cpu_percent(elapsed, &monitor.cpu.user_time, &monitor.cpu.system_time, config.hz);
    printf("%-50s %.3f %%\n", "CPU", (cpu_percent / (float)config.n_processors));
    format_bytes(rss_buf, sizeof(rss_buf), (monitor.memory.rss.current * config.page_size));
    printf("%-50s %s\n", "RSS: ", rss_buf);

    print_stats(&monitor);
    print_io(&monitor);
    prev = curr;
    sleep(1);
  }

  monitor_wait_process(&monitor.process);
  monitor_cleanup(&monitor);
}

void print_stats(Monitor *monitor)
{
  printf("%-50s %ld\n", "Threads: ", monitor->cpu.n_threads);
  printf("%-50s %d\n", "Processor #: ", monitor->cpu.processor);
  printf("%-50s %ld\n", "Priority: ", monitor->cpu.priority);
  printf("%-50s %s\n", "State: ", monitor->cpu.state);
  printf("%-50s %ld ticks\n", "System Time: ", monitor->cpu.system_time.current);
  printf("%-50s %ld ticks\n", "User Time: ", monitor->cpu.user_time.current);
  printf("%-50s %ld\n", "Nice: ", monitor->cpu.nice);
  printf("%-50s %lld\n", "Start Time: ", monitor->cpu.start_time);
  printf("\n");
}

void print_io(Monitor *monitor)
{
  printf("\n");
  printf("%-50s %'lld\n", "Successfull Bytes from Read: ", monitor->io.rchar.current);
  printf("%-50s %'lld\n", "Successfull Bytes from Write: ", monitor->io.wchar.current);
  printf("%-50s %'lld\n", "Number of 'File Read' Syscalls: ", monitor->io.syscr.current);
  printf("%-50s %'lld\n", "Number of 'File Write' Syscalls: ", monitor->io.syscw.current);
  printf("%-50s %'lld\n", "Number of Bytes really fetched from Storage: ", monitor->io.read_bytes.current);
  printf("%-50s %'lld\n", "Number of Bytes really sent to Storage: ", monitor->io.write_bytes.current);
  printf("%-50s %'lld\n", "Cancelled Write Bytes: ", monitor->io.canceled_write_bytes.current);
  printf("\n");
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