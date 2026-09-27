#ifndef RUN_H
#define RUN_H

#include <stdio.h>

#include "cgroups.h"
#include "observe.h"
#include "config.h"
#include "logger.h"
#include "tracer.h"

void start_run(const char *program_name, char **c_argv);

#endif
