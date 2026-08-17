#include "status.h"

bool match_key(char **cursor, const char *key)
{
  char *c = *cursor;

  while (*key)
  {
    if (*c != *key)
      return false;
    c++;
    key++;
  }

  if (*c != ':')
    return false;
  c++;

  while (*c == ' ' || *c == '\t')
  {
    c++;
  }

  *cursor = c;
  return true;
}

void skip_to_next_line(char **cursor)
{
  while (**cursor != '\0' && **cursor != '\n')
  {
    (*cursor)++;
  }
  if (**cursor == '\n')
  {
    (*cursor)++;
  }
}

char *next_status_value(char **cursor)
{
  if (**cursor == '\0')
  {
    return NULL;
  }

  char *start = *cursor;

  while (**cursor != '\0' && **cursor != '\r' && **cursor != '\n' && **cursor != ' ' && **cursor != '\t')
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

void parse_status(MemoryStats *mem_stats, CPUStats *cpu_stats, IOStats *io_stats, SignalStats *sig_stats, char *buffer)
{
  char *cursor = buffer;

  while (*cursor != '\0')
  {
    if (match_key(&cursor, "SigBlk"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        uint64_t sigblk = 0;
        sscanf(token, "%" SCNx64, &sigblk);
        sig_stats->blocked = sigblk;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "SigPnd"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        uint64_t sigpndt = 0;
        sscanf(token, "%" SCNx64, &sigpndt);
        sig_stats->pending_thread = sigpndt;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "ShdPnd"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        uint64_t sigpndt = 0;
        sscanf(token, "%" SCNx64, &sigpndt);
        sig_stats->pending_thread = sigpndt;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "SigIgn"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        uint64_t sigign = 0;
        sscanf(token, "%" SCNx64, &sigign);
        sig_stats->ignored = sigign;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "SigCgt"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        uint64_t sigcgt = 0;
        sscanf(token, "%" SCNx64, &sigcgt);
        sig_stats->ignored = sigcgt;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "VmPeak"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long vmpeak = atol(token);
        mem_stats->peak = vmpeak;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "VmSwap"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long vmswap = atol(token);
        mem_stats->swap = vmswap;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "VmSize"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long vmsize = atol(token);
        mem_stats->vsize = vmsize;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "RssShmem"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long rssshmem = atol(token);
        mem_stats->shared = rssshmem;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "SigQ"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        sig_stats->signals = token;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "FDSize"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long long fdsize = atoll(token);
        io_stats->fd_size = fdsize;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "voluntary_ctxt_switches"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long vctxs = atol(token);
        cpu_stats->v_ctx_switches = vctxs;
      }
      skip_to_next_line(&cursor);
    }
    else if (match_key(&cursor, "nonvoluntary_ctxt_switches"))
    {
      char *token = next_status_value(&cursor);
      if (token)
      {
        long nvctxs = atol(token);
        cpu_stats->nv_ctx_switches = nvctxs;
      }
      skip_to_next_line(&cursor);
    }
    else
    {
      skip_to_next_line(&cursor);
    }
  }
}