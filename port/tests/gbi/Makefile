# gbi_test: the game's GBI (include/gbi_t2.h under TARGET_PORT) against libultraship's gbi.h.
#   make -C port/tests/gbi LUS=/path/to/libultraship   (the FetchContent checkout:
#        <build>/_deps/libultraship-src)
#   port/tests/gbi/build/gbi_test
ROOT  := ../../..
LUS   ?= $(error set LUS=<libultraship checkout>)
CC    := clang
BUILD := build
GAME_CFLAGS := -std=gnu89 -include $(ROOT)/port/include/port_shim.h -I$(ROOT)/port/include -I$(ROOT)/include \
               -DTARGET_PORT=1 -D_LANGUAGE_C -DF3DEX_GBI_2 -fsigned-char -fno-strict-aliasing -fwrapv -fcommon -w -g
GAME_SRC := game_side.c $(ROOT)/src/us/code/func_00277668.c $(ROOT)/src/us/code/func_00279AC4.c

$(BUILD)/gbi_test: gbi_test.c lus_side.c $(GAME_SRC) $(ROOT)/include/gbi_t2.h $(ROOT)/include/gfx.h
	mkdir -p $(BUILD)
	$(CC) $(GAME_CFLAGS) -c game_side.c -o $(BUILD)/game_side.o
	$(CC) $(GAME_CFLAGS) -c $(ROOT)/src/us/code/func_00277668.c -o $(BUILD)/func_00277668.o
	$(CC) $(GAME_CFLAGS) -c $(ROOT)/src/us/code/func_00279AC4.c -o $(BUILD)/func_00279AC4.o
	$(CC) -std=gnu11 -DF3DEX_GBI_2 -I$(LUS)/include -w -g -c lus_side.c -o $(BUILD)/lus_side.o
	$(CC) -g gbi_test.c $(BUILD)/game_side.o $(BUILD)/func_00277668.o $(BUILD)/func_00279AC4.o $(BUILD)/lus_side.o -o $@

clean:
	rm -rf $(BUILD)
.PHONY: clean
