CC     = gcc
CFLAGS = --std=gnu89
EXEC   = replay_dump replay_inject replay_wipe

all: $(EXEC)

replay_dump: replay_dump.c util/ppt_addr.h
	$(CC) $(CFLAGS) -o $@ $<

replay_inject: replay_inject.c util/ppt_addr.h
	$(CC) $(CFLAGS) -o $@ $<

replay_wipe: replay_wipe.c util/ppt_addr.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	$(RM) $(EXEC)
