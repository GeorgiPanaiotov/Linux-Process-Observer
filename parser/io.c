#include "io.h"

void parse_io(IOStats *io_stats, char *buffer)
{
  char *cursor = buffer;
  for (int field = 1;; field++)
  {
    char *token = next_io_field(&cursor);
    if (token == NULL)
    {
      break;
    }

    switch (field)
    {
    case IO_RCHAR:
      long long rch = atoll(token);
      io_stats->rchar = rch;
      break;
    case IO_WCHAR:
      long long wch = atoll(token);
      io_stats->wchar = wch;
      break;
    case IO_SYSCR:
      long long syscr = atoll(token);
      io_stats->syscr = syscr;
      break;
    case IO_SYSCW:
      long long syscw = atoll(token);
      io_stats->syscw = syscw;
      break;
    case IO_READ_BYTES:
      long long rb = atoll(token);
      io_stats->read_bytes = rb;
      break;
    case IO_WRITE_BYTES:
      long long wb = atoll(token);
      io_stats->write_bytes = wb;
      break;
    case IO_CANCELLED:
      long long cn = atoll(token);
      io_stats->canceled_write_bytes = cn;
      break;
    default:
      break;
    }
  }
}

char *next_io_field(char **cursor)
{
  if (**cursor == '\0')
  {
    return NULL;
  }

  char *start = *cursor;

  while (**cursor != '\0' && **cursor != ' ' && **cursor != '\n' && **cursor != '\r')
  {
    (*cursor)++;
  }

  if (**cursor != '\0')
  {
    **cursor = '\0';
    (*cursor)++;
  }

  return start;
}