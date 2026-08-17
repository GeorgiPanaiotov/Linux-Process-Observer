#ifndef STATUS_H
#define STATUS_H

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>

#include "sample.h"

void parse_status(MemoryStats *mem_stats, CPUStats *cpu_stats, IOStats *io_stats, SignalStats *sig_stats, char *buffer);
bool match_key(char **cursor, const char *key);
void skip_to_next_line(char **cursor);
char *next_status_value(char **cursor);

#endif