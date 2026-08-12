#ifndef IO_H
#define IO_H

#include "sample.h"

#include <stdio.h>

typedef enum
{
  IO_RCHAR = 2,
  IO_WCHAR = 4,
  IO_SYSCR = 6,
  IO_SYSCW = 8,
  IO_READ_BYTES = 10,
  IO_WRITE_BYTES = 12,
  IO_CANCELLED = 14,
} IOField;

void parse_io(IOStats *io_stats, char *buffer);
char *next_io_field(char **cursor);

#endif