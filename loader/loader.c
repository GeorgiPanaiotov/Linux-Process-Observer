#include "loader.h"

int load_ebpf_program(const char *path)
{
  unsigned char instructions[4096];
  int fd = open(path, O_RDONLY);
  int instructions_count = read(fd, instructions, sizeof(instructions));
  close(fd);

  char log_buffer[65536] = {0};
  union bpf_attr prog_attr;
  memset(&prog_attr, 0, sizeof(prog_attr));

  prog_attr.prog_type = BPF_PROG_TYPE_TRACEPOINT;
  prog_attr.insns = (unsigned long)instructions;
  prog_attr.insn_cnt = instructions_count / 8;
  prog_attr.license = (unsigned long)"GPL";
  prog_attr.log_buf = (unsigned long)log_buffer;
  prog_attr.log_size = sizeof(log_buffer);
  prog_attr.log_level = 1;

  int program_fd = syscall(__NR_bpf, BPF_PROG_LOAD, &prog_attr, sizeof(prog_attr));
  if (program_fd == -1)
  {
    perror("load_ebpf_program: syscall");
    printf("Verifier Log: %s\n", log_buffer);
    return -1;
  }

  return program_fd;
}

// int load_program_event(int event_id)
// {
//   struct perf_event_attr pe_attr;
//   memset(&pe_attr, 0, sizeof(pe_attr));

//   pe_attr.type = PERF_TYPE_TRACEPOINT;
//   pe_attr.config = event_id;
//   pe_attr.size = sizeof(struct perf_event_attr);
//   pe_attr.sample_type = PERF_SAMPLE_RAW;
//   pe_attr.sample_period = 1;
//   pe_attr.wakeup_events = 1;

//   int event_fd = syscall(__NR_perf_event_open, &pe_attr, -1, 0, -1, 0);
//   if (event_fd == -1)
//   {
//     perror("load_program_event: syscall");
//     return -1;
//   }

//   return event_fd;
// }

int load_program_event(int *program_fd, int tracepoint_id)
{
  union bpf_attr attr;
  memset(&attr, 0, sizeof(attr));

  attr.link_create.prog_fd = *program_fd;
  attr.link_create.target_fd = tracepoint_id;
  attr.link_create.attach_type = 1;

  int link_fd = syscall(__NR_bpf, BPF_LINK_CREATE, &attr, sizeof(attr));
  if (link_fd < 0)
  {
    perror("load_program_event: syscall");
    return -1;
  }

  return link_fd;
}

void attach_event_program(int *program_fd, int *event_fd)
{
  if (ioctl(*event_fd, PERF_EVENT_IOC_SET_BPF, *program_fd) < 0)
  {
    perror("attach_event_program: ioctl");
    exit(EXIT_FAILURE);
  }

  if (ioctl(*event_fd, PERF_EVENT_IOC_ENABLE, 0) < 0)
  {
    perror("attach_event_program: ioctl");
    exit(EXIT_FAILURE);
  }
}
