#include "fd.h"

void read_fd_contents(int *fd, pid_t pid, FDBuffer *buf)
{
  int dup_fd = dup(*fd);
  if (dup_fd == -1)
  {
    return;
  }

  lseek(dup_fd, 0, SEEK_SET);

  DIR *dir = fdopendir(dup_fd);
  if (dir == NULL)
  {
    close(dup_fd);
    return;
  }

  struct dirent *dir_ent;
  errno = 0;

  while ((dir_ent = readdir(dir)) != NULL)
  {
    if (strcmp(dir_ent->d_name, ".") == 0 || strcmp(dir_ent->d_name, "..") == 0)
    {
      continue;
    }

    if (dir_ent->d_type == DT_LNK || dir_ent->d_type == DT_UNKNOWN)
    {
      char path[PATH_MAX];
      char file_path[PATH_MAX];

      snprintf(path, sizeof(path), "/proc/%d/fd/%s", pid, dir_ent->d_name);

      errno = 0;
      ssize_t len = readlink(path, file_path, sizeof(file_path) - 1);

      if (len != -1)
      {
        file_path[len] = '\0';

        if (strcmp(file_path, "/dev/pts/14") != 0)
        {
          if (buf->offset < FDBUFFER_SIZE)
          {
            size_t remaining = FDBUFFER_SIZE - buf->offset;

            int written = snprintf(buf->data + buf->offset, remaining, "%s\n", file_path);

            if (written > 0 && (size_t)written < remaining)
            {
              buf->offset += written;
            }
          }
        }
      }
      else
      {
        errno = 0;
      }
    }
  }

  closedir(dir);
}
FDBuffer fdbuffer_create()
{
  FDBuffer buf;
  buf.data = malloc(FDBUFFER_SIZE + 1);

  if (buf.data == NULL)
  {
    perror("fdbuffer_create: malloc");
    exit(EXIT_FAILURE);
  }

  buf.data[FDBUFFER_SIZE] = '\0';
  buf.offset = 0;

  return buf;
}

void fdbuffer_cleanup(FDBuffer *buf)
{
  free(buf->data);
  buf->offset = 0;
}

void fd_buffer_reset(FDBuffer *buf)
{
  buf->offset = 0;
  buf->data[0] = '\0';
}