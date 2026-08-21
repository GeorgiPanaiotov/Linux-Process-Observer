struct trace_event_sys_enter_read {
  unsigned short common_type;
  unsigned char common_flags;
  unsigned char common_preempt_count;
  int common_pid;

  int __syscall_nr;
  int __pad;

  unsigned long fd;
  char *buf;
  unsigned long count;
};

static long (*bpf_trace_printk)(const char *fmt, unsigned int fmt_size, ...) = (void *)14;

__attribute__((section("tracepoint/syscalls/sys_enter_read")))
int handle_enter_read(struct trace_event_sys_enter_read *ctx)
{
  char fmt[] = "sys_enter_read called - fd: 0x%08lx, buf: 0x%08lx, count: 0x%08lx\n";
  bpf_trace_printk(fmt, sizeof(fmt), ctx->fd, ctx->buf, ctx->count);

  return 0;
}