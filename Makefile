CC = gcc
CFLAGS = -Wall -Wextra -Icli -Iobserve -Iprocess -Iparser -Iconfiguration -Itrace -Ilogger

MAIN = main.c
CLI = cli/run.c cli/watch.c
PROCESS = process/enumerate.c process/process.c
OBSERVE = observe/observe.c observe/sample.c
PARSER = parser/stats.c parser/io.c parser/status.c parser/net_dev.c parser/fd.c
CONFIG = configuration/config.c
TRACER = trace/tracer.c
LOGGER = logger/logger.c

SOURCES = $(MAIN) $(CLI) $(PROCESS) $(OBSERVE) $(PARSER) $(CONFIG) $(TRACER) $(LOGGER)

build:
	$(CC) -o lbx $(CFLAGS) $(SOURCES)

dbg:
	$(CC) -o lbx $(CFLAGS) -g $(SOURCES)

clean:
	rm ./lbx
