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

  return conf;
}