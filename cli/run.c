#include "run.h"

void start_run(const char *program_name, char **c_argv)
{
  Monitor monitor = monitor_create(program_name, c_argv);
  Config config = config_create();
  Logger logger = logger_create(".");

  struct timespec prev;
  struct timespec curr;

  // read_tracer(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));
  clock_gettime(CLOCK_MONOTONIC, &curr);
  prev = curr;

  // monitor.logs.tracer_buffer[BUFFER_8KB_SIZE] = '\0';

  logger_file_create(&logger);

  // int ers_prog_fd = load_ebpf_program("./ebpf/bpf_sys_read_enter.bin");
  // if (ers_prog_fd == -1)
  // {
    // perror("start_run: load_ebpf_program");
    // exit(EXIT_FAILURE);
  // }

  // int ers_event_fd = load_program_event(807);
  // if (ers_event_fd == -1)
  // {
    // perror("start_run: load_program_event");
    // exit(EXIT_FAILURE);
  // }

  // attach_event_program(&ers_prog_fd, &ers_event_fd);

  while (monitor.process.running)
  {
    fd_buffer_reset(&monitor.fd_buffer);
    // read_tracer(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));
    read_fd_contents(&monitor.proc_files.fddir_fd, monitor.process.process_pid, &monitor.fd_buffer);
    monitor_loop(&monitor);

    clock_gettime(CLOCK_MONOTONIC, &curr);
    float elapsed = (curr.tv_sec - prev.tv_sec) + (curr.tv_nsec - prev.tv_nsec) / 1e9;
    float cpu_percent = get_cpu_percent(elapsed, &monitor.cpu.user_time, &monitor.cpu.system_time, config.hz);

    logger_log_data(&monitor, &config, &logger, cpu_percent);

    prev = curr;
    p_sleep(&config);
  }

  // tracer_cleanup();

  // exhaust_pipe(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));
  logger_log_data(&monitor, &config, &logger, 0);

  logger_cleanup(&logger);
  monitor_wait_process(&monitor.process);
  monitor_cleanup(&monitor);
}

void print_memory(Monitor *monitor, Config *config, char *rss_buf, char *peak_buf, char *swap_buf, char *shared_buf, char *vsize_buf, size_t rss_size, size_t peak_size, size_t swap_size, size_t shared_size, size_t vsize_size)
{
  format_bytes(rss_buf, rss_size, (monitor->memory.rss * config->page_size));
  printf("%-50s %s\n", "RSS: ", rss_buf);
  format_bytes(peak_buf, peak_size, (monitor->memory.peak * 1024));
  printf("%-50s %s\n", "VM Peak: ", peak_buf);
  format_bytes(swap_buf, swap_size, (monitor->memory.swap * 1024));
  printf("%-50s %s\n", "VM Swap: ", swap_buf);
  format_bytes(shared_buf, shared_size, (monitor->memory.shared * 1024));
  printf("%-50s %s\n", "VM Shared: ", shared_buf);
  format_bytes(vsize_buf, vsize_size, (monitor->memory.vsize * 1024));
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
  printf("%-50s %'lld\n", "Successfull Bytes from Read: ", monitor->io.rchar);
  printf("%-50s %'lld\n", "Successfull Bytes from Write: ", monitor->io.wchar);
  printf("%-50s %'lld\n", "Number of 'File Read' Syscalls: ", monitor->io.syscr);
  printf("%-50s %'lld\n", "Number of 'File Write' Syscalls: ", monitor->io.syscw);
  printf("%-50s %'lld\n", "Number of Bytes really fetched from Storage: ", monitor->io.read_bytes);
  printf("%-50s %'lld\n", "Number of Bytes really sent to Storage: ", monitor->io.write_bytes);
  printf("%-50s %'lld\n", "Cancelled Write Bytes: ", monitor->io.canceled_write_bytes);
  printf("\n");
}

void print_status(Monitor *monitor)
{
  print_cpu_switches(monitor);
}

void print_cpu_switches(Monitor *monitor)
{
  printf("%-50s %ld\n", "Voluntary Context Switches: ", monitor->cpu.v_ctx_switches);
  printf("%-50s %ld\n", "Non-Voluntary Context Switches: ", monitor->cpu.nv_ctx_switches);
  printf("\n");
}
