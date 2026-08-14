#ifndef CONFIG_H
#define CONFIG_H

#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct Config
{
  long hz;
  long n_processors;
  long page_size;
  struct timespec slp;
  struct timespec rem;
} Config;

Config config_create();
void p_sleep();

#endif