#include "run.h"

void start_run(const char *program_name, char **c_argv)
{
	enable_cgroup_controllers();
	create_cgroup(getpid());

	Monitor monitor = monitor_create_cgroup_process(program_name, c_argv);
  	Config config = config_create();
  	Logger logger = logger_create(".");

  	struct timespec prev;
  	struct timespec curr;

  	read_tracer(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));
  	clock_gettime(CLOCK_MONOTONIC, &curr);
  	prev = curr;

  	monitor.logs.tracer_buffer[BUFFER_8KB_SIZE] = '\0';

  	logger_file_create(&logger);

	monitor_start_process(&monitor.process);

	// while (monitor.process.running)
  	// {
    	read_tracer(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));

    	clock_gettime(CLOCK_MONOTONIC, &curr);

    	prev = curr;
    	p_sleep(&config);
  	// }
	load_and_read_process_data(&monitor, &monitor.cpu_stat_buffer, monitor.cgroup_files.cpu_stat_fd);

	printf("%s\n", monitor.cpu_stat_buffer.data);
	monitor_wait_process(&monitor.process);
	close_cgroup_proc_files(&monitor.cgroup_files);
  	tracer_cleanup();
	buffer_cleanup(&monitor.cpu_stat_buffer);

  	exhaust_pipe(monitor.logs.tracer_buffer, sizeof(monitor.logs.tracer_buffer));
  	logger_cleanup(&logger);
  	monitor_cleanup(&monitor);
	cleanup_cgroup();
}