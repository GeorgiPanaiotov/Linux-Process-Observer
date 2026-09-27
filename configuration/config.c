#include "config.h"

Config config_create()
{
  Config conf;

  long hz = sysconf(_SC_CLK_TCK);
  long p_size = sysconf(_SC_PAGESIZE);
  long n_processors = sysconf(_SC_NPROCESSORS_ONLN);

  conf.hz = hz;
  conf.n_processors = n_processors;
  conf.page_size = p_size;

  setlocale(LC_NUMERIC, "en_US.UTF-8");

  struct timespec slp = {
      .tv_sec = 0,
      .tv_nsec = 50 * 1000 * 1000};
  struct timespec rem = {0};

  conf.slp = slp;
  conf.rem = rem;

  return conf;
}

void p_sleep(Config *config)
{
  int ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &config->slp, &config->rem);
  while (ret == EINTR)
  {
    config->slp = config->rem;
    ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &config->slp, &config->rem);
  }

  if (ret != 0)
  {
    perror("p_sleep: clock_nanosleep");
    exit(EXIT_FAILURE);
  }
}
