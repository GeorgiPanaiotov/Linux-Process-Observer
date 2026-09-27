#include "run_process.h"
#include "run.h"

extern char **environ;

void print_help()
{
  printf("Linux Black Box observer\n");
  printf("Usage: lbx [...args]\n\n");
  printf("%-50s %s\n", "--help", "Help");
  printf("%-50s %s\n", "--run-process <program_name>", "Run <program_name>. Runs a single process with no expected children.");
  printf("%-50s %s\n", "--run <program_name>", "Run <program_name>. Runs a process that is expected to fork and start children of its own.");
  printf("%-50s %s\n", "--watch <pid>", "Watch <pid>");
}

int main(int argc, char *argv[])
{
  bool run_process, watch, run = false;
  char *program_name = "";

  if (argc < 2)
  {
    print_help();
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < argc; i++)
  {
    if (strcmp(argv[i], "--run-process") == 0)
    {
      run_process = true;
      if (i + 1 < argc)
      {
        program_name = argv[i + 1];
      }
    }
    if (strcmp(argv[i], "--run") == 0)
    {
      run = true;
      if (i + 1 < argc)
      {
        program_name = argv[i + 1];
      }
    }
    if (strcmp(argv[i], "--watch") == 0)
    {
      watch = true;
      if (i + 1 < argc)
      {
        program_name = argv[i + 1];
      }
    }
  }

  if (strcmp(program_name, "") == 0)
  {
    printf("Please provide a program to be monitored!\n");
    print_help();
    exit(EXIT_FAILURE);
  }

  char **c_argv = &argv[2];

  if (watch)
  {
    // invoke watch
  }

  if (run_process)
  {
    start_run_process(program_name, c_argv);
  }

  if (run)
  {
    start_run(program_name, c_argv);
  }
  return 0;
}
