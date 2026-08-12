#include "stats.h"

void parse_stats(MemoryStats *mem_stats, CPUStats *cpu_stats, char *buffer)
{
  char *cursor = buffer;

  for (int field = 1;; field++)
  {
    char *token = next_stat_field(&cursor);
    if (token == NULL)
    {
      break;
    }

    switch (field)
    {
    case STAT_RSS:
      long rss = atol(token);
      sample_update(&mem_stats->rss, rss);
      break;
    case STAT_UTIME:
      long u_time = atol(token);
      sample_update(&cpu_stats->user_time, u_time);
      break;
    case STAT_STIME:
      long s_time = atol(token);
      sample_update(&cpu_stats->system_time, s_time);
      break;
    case STAT_STARTTIME:
      long long start_time = atoll(token);
      cpu_stats->start_time = start_time;
      break;
    case STAT_PROCESSOR:
      int processor = atoi(token);
      cpu_stats->processor = processor;
      break;
    case STAT_PRIORITY:
      long priority = atol(token);
      cpu_stats->priority = priority;
      break;
    case STAT_NICE:
      long nice = atol(token);
      cpu_stats->nice = nice;
      break;
    case STAT_STATE:
      cpu_stats->state = get_state(*token);
      break;
    case STAT_NUMTHREADS:
      long n_threads = atol(token);
      cpu_stats->n_threads = n_threads;
      break;
    default:
      break;
    }
  }
}

char *next_stat_field(char **cursor)
{
  if (**cursor == '\0')
  {
    return NULL;
  }

  char *start = *cursor;

  if (*start == '(')
  {
    start++;

    while (**cursor && **cursor != ')')
    {
      (*cursor)++;
    }
    if (**cursor == ')')
    {
      (*cursor)++;
    }

    if (**cursor == ' ')
    {
      **cursor = '\0';
      (*cursor)++;
    }

    return start;
  }

  while (**cursor && **cursor != ' ')
  {
    (*cursor)++;
  }
  if (**cursor == ' ')
  {
    **cursor = '\0';
    (*cursor)++;
  }

  return start;
}

char *get_state(char c)
{
  if (c == 'R') return "Running";
  if (c == 'S') return "Sleeping in an uninteruptible wait";
  if (c == 'D') return "Waiting in uninteruptible disk sleep";
  if (c == 'Z') return "Zombie";
  if (c == 'T') return "Stopped";
  if (c == 't') return "Tracing Stop";
  if (c == 'X') return "Dead";
  if (c == 'x') return "Dead";
  if (c == 'K') return "Wakekill";
  if (c == 'W') return "Waking";
  if (c == 'W') return "Paging";
  if (c == 'P') return "Parked";
  if (c == 'I') return "Idle";
  return "Unknown";
}
