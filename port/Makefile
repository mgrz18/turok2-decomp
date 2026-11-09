# Turok 2 port: host-side asset reader prototype (not the game build; the matching
# build in the top-level Makefile does not use this directory).
#   make -C port            build build/t2asset_test, build/rnc_test
#   make -C port test ROM=../baserom.us.z64 UNITS=units.tsv ARCHIVES="turok2.o2r"
CC      ?= cc
CFLAGS  ?= -O2 -g -Wall -Wextra -Wno-unused-parameter
BUILD   ?= build

LIB_SRC := assets/t2asset.c assets/rnc.c third_party/miniz/miniz.c
TEST    := $(BUILD)/t2asset_test
RNCTEST := $(BUILD)/rnc_test

all: $(TEST) $(RNCTEST)

$(BUILD)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/third_party/%.o: CFLAGS += -w -D_LARGEFILE64_SOURCE

$(TEST): $(patsubst %.c,$(BUILD)/%.o,$(LIB_SRC) tests/t2asset_test.c)
	$(CC) $(CFLAGS) $^ -o $@

$(RNCTEST): $(BUILD)/assets/rnc.o $(BUILD)/tests/rnc_test.o
	$(CC) $(CFLAGS) $^ -o $@

test: $(TEST) $(RNCTEST)
	$(RNCTEST) $(ROM)
	$(TEST) $(ROM) $(UNITS) $(ARCHIVES)

clean:
	rm -rf $(BUILD)

.PHONY: all test clean
