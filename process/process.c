#include "process.h"

Process process_create(const char *p_name, char **argv)
{
  Process process;

  process.parent_pid = getpid();
  process.process_pid = -1;
  process.running = false;
  process.p_name = p_name;

  process.argv = argv;

  return process;
}

pid_t process_spawn(Process *process)
{
  pid_t c_pid = fork();
  if (c_pid == -1)
  {
    perror("process_create: fork");
    exit(EXIT_FAILURE);
  }
  if (c_pid == 0)
  {
    tracer_create(c_pid);
    int p_status = execvp(process->p_name, process->argv);

    if (p_status == -1)
    {
      perror("process_create: execvp");
      exit(EXIT_FAILURE);
    }
  }

  process->process_pid = c_pid;
  process->running = true;
  return c_pid;
}

void process_wait(Process *process)
{
  int status;
  waitpid(process->process_pid, &status, 0);
}

void process_cleanup(Process *process)
{
  process->argv = NULL;
  process->p_name = NULL;
  process->parent_pid = -1;
  process->process_pid = -1;
  process->running = false;
}

void update_process_running_status(Process *process)
{
  int status;
  pid_t ret = waitpid(process->process_pid, &status, WNOHANG);

  if (ret == 0)
  {
    process->running = true;
  }
  else if (ret == process->process_pid)
  {
    process->running = false;
  }
  else
  {
    perror("update_process_running_status: waitpid");
  }
}
