# t2os (libultra threads/queues/events on host threads) and its test. Not the game build.
#   make -C port/runtime          build build/t2os_test
#   make -C port/runtime test     run it 3 times: checks + the same switch-order hash every run
CC      ?= cc
CFLAGS  ?= -O2 -g -Wall -Wextra -std=gnu11
BUILD   ?= build
TEST    := $(BUILD)/t2os_test

all: $(TEST)

$(TEST): t2os.c t2os.h ../tests/t2os_test.c
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) t2os.c ../tests/t2os_test.c -o $@ -lpthread

test: $(TEST)
	@for i in 1 2 3; do $(TEST) | tee $(BUILD)/run$$i.txt | tail -2; done
	@grep -h "trace hash" $(BUILD)/run*.txt | sort -u | wc -l | grep -qx 1 && echo "deterministic: same hash on 3 runs" || (echo "NOT deterministic"; exit 1)

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
