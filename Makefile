CC = gcc
CFLAGS = -Wall -Wextra -Icli -Iobserve -Iprocess -Iparser -Iconfiguration -Itrace -Ilogger -Iloader -Ielf

MAIN = main.c
CLI = cli/run.c cli/watch.c
PROCESS = process/enumerate.c process/process.c
OBSERVE = observe/observe.c observe/sample.c
PARSER = parser/stats.c parser/io.c parser/status.c parser/net_dev.c parser/fd.c
CONFIG = configuration/config.c
TRACER = trace/tracer.c
LOGGER = logger/logger.c
LOADER = loader/loader.c
ELFPARSER = elf/elf_parser.c

SOURCES = $(MAIN) $(CLI) $(PROCESS) $(OBSERVE) $(PARSER) $(CONFIG) $(TRACER) $(LOGGER) $(LOADER) $(ELFPARSER)

compile_bpfs:
	clang -O2 -target bpf -c ebpf/bpf_sys_read_enter.c -o ebpf/bpf_sys_read_enter.o
	llvm-objcopy-21 -O binary --only-section=tracepoint/syscalls/sys_enter_read ebpf/bpf_sys_read_enter.o ebpf/bpf_sys_read_enter.bin

build: compile_bpfs
	$(CC) -o lbx $(CFLAGS) $(SOURCES)

dbg:
	$(CC) -o lbx $(CFLAGS) -g $(SOURCES)

clean:
	rm ./lbx
