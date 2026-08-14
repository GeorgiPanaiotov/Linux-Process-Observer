#include "run.h"

static const char *suffixes[] = {"B", "KB", "MB", "GB", "TB", "PB"};

void start_run(const char *program_name, char **c_argv)
{
  Monitor monitor = monitor_create(program_name, c_argv);
  Config config = config_create();

  char rss_buf[32];
  char peak_buf[32];
  char swap_buf[32];
  char shared_buf[32];
  char vsize_buf[32];

  struct timespec prev;
  struct timespec curr;

  clock_gettime(CLOCK_MONOTONIC, &curr);
  prev = curr;

  struct timespec slp = {
      .tv_sec = 0,
      .tv_nsec = 50 * 1000 * 1000};
  struct timespec rem;

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
    printf("%-50s %.3f %%\n", "CPU: ", (cpu_percent / (float)config.n_processors));

    print_memory(&monitor, &config, rss_buf, peak_buf, swap_buf, shared_buf, vsize_buf, sizeof(rss_buf), sizeof(peak_buf), sizeof(swap_buf), sizeof(shared_buf), sizeof(vsize_buf));

    print_stats(&monitor);
    print_io(&monitor);
    print_status(&monitor);

    prev = curr;
    int ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &slp, &rem);
    while (ret == EINTR)
    {
      slp = rem;
      ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &slp, &rem);
    }

    if (ret != 0)
    {
      perror("start_run: clock_nanosleep");
      exit(EXIT_FAILURE);
    }
  }

  monitor_wait_process(&monitor.process);
  monitor_cleanup(&monitor);
}

void print_memory(Monitor *monitor, Config *config, char *rss_buf, char *peak_buf, char *swap_buf, char *shared_buf, char *vsize_buf, size_t rss_size, size_t peak_size, size_t swap_size, size_t shared_size, size_t vsize_size)
{
  format_bytes(rss_buf, rss_size, (monitor->memory.rss.current * config->page_size));
  printf("%-50s %s\n", "RSS: ", rss_buf);
  format_bytes(peak_buf, peak_size, (monitor->memory.peak.current * 1024));
  printf("%-50s %s\n", "VM Peak: ", peak_buf);
  format_bytes(swap_buf, swap_size, (monitor->memory.swap.current * 1024));
  printf("%-50s %s\n", "VM Swap: ", swap_buf);
  format_bytes(shared_buf, shared_size, (monitor->memory.shared.current * 1024));
  printf("%-50s %s\n", "VM Shared: ", shared_buf);
  format_bytes(vsize_buf, vsize_size, (monitor->memory.vsize.current * 1024));
  printf("%-50s %s\n", "VM Size: ", vsize_buf);
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
  printf("%-50s %'lld\n", "File Descriptor slots currently allocated: ", monitor->io.fd_size);
  printf("%-50s %'lld\n", "Successfull Bytes from Read: ", monitor->io.rchar.current);
  printf("%-50s %'lld\n", "Successfull Bytes from Write: ", monitor->io.wchar.current);
  printf("%-50s %'lld\n", "Number of 'File Read' Syscalls: ", monitor->io.syscr.current);
  printf("%-50s %'lld\n", "Number of 'File Write' Syscalls: ", monitor->io.syscw.current);
  printf("%-50s %'lld\n", "Number of Bytes really fetched from Storage: ", monitor->io.read_bytes.current);
  printf("%-50s %'lld\n", "Number of Bytes really sent to Storage: ", monitor->io.write_bytes.current);
  printf("%-50s %'lld\n", "Cancelled Write Bytes: ", monitor->io.canceled_write_bytes.current);
  printf("\n");
}

void print_status(Monitor *monitor)
{
  print_status_signals(monitor);
}

void print_status_signals(Monitor *monitor)
{
  printf("%-50s %s\n", "Signals queued/Signals limit: ", monitor->signals.signals);
  print_hex_signals("Signals Pending Thread: ", monitor->signals.pending_thread);
  print_hex_signals("Signals Pending Process: ", monitor->signals.pending_process);
  print_hex_signals("Signals Blocked: ", monitor->signals.blocked);
  print_hex_signals("Signals Ignored: ", monitor->signals.ignored);
  print_hex_signals("Signals Caught: ", monitor->signals.caught);
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