#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>
#include <locale.h>

typedef struct Config
{
  long hz;
  long n_processors;
  long page_size;
} Config;

Config config_create();

#endif