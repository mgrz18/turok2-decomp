# turok2-decomp Makefile.
#
# Pipeline (adapted from references/turok3/Makefile):
#   .c  --cpp--> .i --cc1--> .s_c --asn64.exe(wine)--> .obj
#       --psyq-obj-parser--> .o
#   .s  --mips-linux-gnu-as--> .o
#   .bin --ld -r -b binary--> .o
#   all .o --mips-linux-gnu-ld--> turok2.us.elf --objcopy--> turok2.us.z64
#
# Designed to run inside the `turok2-build` docker image (linux/amd64).
# On the host Mac this Makefile generally won't work — wine/cc1/psyq are
# all Linux x86 binaries.

BASENAME      = turok2
GAME_VERSION ?= us
VERSION       = $(GAME_VERSION)

VERSIONS_DIR  = versions
VERSION_DIR   = $(VERSIONS_DIR)/$(VERSION)
TOOLS_DIR     = tools

# The splat yaml lives flat in versions/ (no per-version subdir for now)
SPLAT_YAML    = $(VERSIONS_DIR)/$(BASENAME).$(VERSION).yaml

# splat layout: us/asm, us/assets, us/include, us/turok2.ld
SPLAT_DIR     = $(VERSION)
ASM_DIR       = $(SPLAT_DIR)/asm
BIN_DIR       = $(SPLAT_DIR)/assets
INCLUDE_DIR   = $(SPLAT_DIR)/include
LD_SCRIPT     = $(SPLAT_DIR)/$(BASENAME).ld

# Hand-written sources
SRC_DIR       = src/$(VERSION)
SRC_ASM_DIR   = $(SRC_DIR)/asm

# Build output
BUILD_DIR     = build
TARGET        = $(BUILD_DIR)/$(BASENAME).$(VERSION)

# Symbol seeds (gitignored; produced by setup script / splat)
UNDEFINED_SYMS  = $(VERSIONS_DIR)/undefined_syms_auto.$(VERSION).txt
UNDEFINED_FUNCS = $(VERSIONS_DIR)/undefined_funcs_auto.$(VERSION).txt
# Addresses no segment places yet, harvested from the linker's own complaints
# by `tools/function_seed.py --from-link-log`. Defining them absolutely still
# reassembles each `jal` to the original word.
UNDEFINED_EXTRA = $(VERSIONS_DIR)/undefined_funcs_extra.$(VERSION).txt
# Hand-derived symbols with their derivation recorded. Tracked, unlike the two
# generated files above.
SYMS_MANUAL     = $(VERSIONS_DIR)/symbols_manual.$(VERSION).txt

# Toolchain (Linux container)
CROSS    = mips-linux-gnu-
AS       = $(CROSS)as
LD       = $(CROSS)ld
OBJCOPY  = $(CROSS)objcopy
CPP      = cpp
PYTHON  ?= python3

ASN64    = wine $(TOOLS_DIR)/sn64/asn64.exe
CC       = $(TOOLS_DIR)/sn64/cc1
LNKCONV  = $(TOOLS_DIR)/psyq-obj-parser
N64CRC   = $(TOOLS_DIR)/n64crc

# Flags (from turok3's Makefile)
OPT_FLAGS         = -O2
INCLUDE_CC_FLAGS  = -I. -Iinclude -I$(INCLUDE_DIR)
# Use -mips4 (drop -march/-mtune=vr4300 which imply -mips3) so the
# assembler accepts the conditional-move ops (movz/movn) the R4300
# implements but the MIPS III base ISA does not declare. The resulting
# object code still targets vr4300 and links against MIPS III objects
# from cc1.
AS_FLAGS          = -EB -mabi=32 -mips4 -O1 -I $(INCLUDE_DIR) --defsym ASSEMBLER=1
ASM_FLAGS         = -I $(INCLUDE_DIR) -mips3
D_FLAGS           = -D_LANGUAGE_C -DF3DEX_GBI_2 -D__GNUC__=2 -DGAME_VERSION=\"$(VERSION)\"

CC_FLAGS          = -quiet -G0 -mips3 $(OPT_FLAGS) -mgas -meb -mcpu=VR4300 -mhard-float -mfp64
CPP_FLAGS         = -P -undef -Wall -lang-c $(D_FLAGS) $(INCLUDE_CC_FLAGS) -nostdinc

# Symbol files are filtered first: anything the objects already define must not
# also be defined by the linker script, or it lands as ABS and N64Recomp stops
# treating it as a function. See tools/filter_defined_syms.py.
FILTERED_DIR      = $(BUILD_DIR)/syms
LD_FLAGS          = -T $(LD_SCRIPT) -Map $(TARGET).map --no-check-sections
# Add splat-generated symbol tables only if they exist
ifneq (,$(wildcard $(UNDEFINED_SYMS)))
LD_FLAGS         += -T $(FILTERED_DIR)/$(notdir $(UNDEFINED_SYMS))
endif
ifneq (,$(wildcard $(UNDEFINED_FUNCS)))
LD_FLAGS         += -T $(FILTERED_DIR)/$(notdir $(UNDEFINED_FUNCS))
endif
ifneq (,$(wildcard $(UNDEFINED_EXTRA)))
LD_FLAGS         += -T $(FILTERED_DIR)/$(notdir $(UNDEFINED_EXTRA))
endif
ifneq (,$(wildcard $(SYMS_MANUAL)))
LD_FLAGS         += -T $(SYMS_MANUAL)
endif

OBJCOPY_FLAGS     = -O binary

# Source enumeration
S_FILES   = $(wildcard $(ASM_DIR)/*.s) $(wildcard $(ASM_DIR)/data/*.s) $(wildcard $(SRC_ASM_DIR)/*.s)
C_FILES   = $(shell find $(SRC_DIR) -name '*.c' 2>/dev/null)
BIN_FILES = $(wildcard $(BIN_DIR)/*.bin)

# Object outputs (path mirrors splat ld script: build/asm/*.s.o, build/assets/*.bin.o)
S_OBJS   = $(patsubst $(SPLAT_DIR)/%.s,$(BUILD_DIR)/%.s.o,$(filter $(ASM_DIR)/%,$(S_FILES))) \
           $(patsubst %.s,$(BUILD_DIR)/%.s.o,$(filter $(SRC_ASM_DIR)/%,$(S_FILES)))
C_OBJS   = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/src/%.c.o,$(C_FILES))
BIN_OBJS = $(patsubst $(SPLAT_DIR)/%.bin,$(BUILD_DIR)/%.bin.o,$(BIN_FILES))

O_FILES  = $(S_OBJS) $(C_OBJS) $(BIN_OBJS)

EXPECTED_SHA1 = fb0400f21e3f043939ab56500c7b12a3231006f1

.PHONY: default all setup build link rom verify clean nuke help

default: all

help:
	@echo "turok2-decomp targets:"
	@echo "  make setup    - splat-disasm the ROM into us/"
	@echo "  make build    - compile all C and assemble all asm"
	@echo "  make          - full pipeline -> $(TARGET).z64"
	@echo "  make verify   - sha1sum produced rom vs baserom"
	@echo "  make clean    - remove $(BUILD_DIR)/"
	@echo "  make nuke     - also remove splat output ($(SPLAT_DIR)/asm,assets,bin)"

all: rom

setup:
	$(PYTHON) -m splat split $(SPLAT_YAML)

build: $(O_FILES)

link: $(TARGET).elf

rom: $(TARGET).z64

$(TARGET).z64: $(TARGET).elf | $(N64CRC)
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@
	$(N64CRC) $@

$(TARGET).elf: $(O_FILES) $(LD_SCRIPT)
	@mkdir -p $(dir $@)
	$(PYTHON) $(TOOLS_DIR)/filter_defined_syms.py $(FILTERED_DIR) \
		$(UNDEFINED_SYMS) $(UNDEFINED_FUNCS) $(UNDEFINED_EXTRA) $(SYMS_MANUAL)
	$(LD) $(LD_FLAGS) -o $@

verify: $(TARGET).z64
	@printf '%s  %s\n' '$(EXPECTED_SHA1)' '$(TARGET).z64' | sha1sum -c - \
		|| (echo "produced: $$(sha1sum $(TARGET).z64 | cut -d' ' -f1)"; \
		    echo "expected: $(EXPECTED_SHA1)"; \
		    exit 1)

# ---- Recipes -------------------------------------------------------------

# Splat-produced asm -> build/asm/*.s.o   (us/asm/foo.s -> build/asm/foo.s.o)
#
# splat emits 2-operand `div`/`divu`/`mult` pseudo-ops (633 sites). gas treats
# those as MACROS and expands each into the instruction plus divide-by-zero
# checks, which needs $at. Flipping `.set noat` to `.set at` let that through
# and silently inflated the build by 4,412 bytes — the first divergence from
# the baserom landed at ROM 0x160B, where a `beq +0x15` had become `beq +0x18`.
#
# Keep `.set noat` and add `.set nomacro` instead, so gas emits the bare
# instruction the original SN64 toolchain produced.
$(BUILD_DIR)/asm/%.s.o: $(ASM_DIR)/%.s
	@mkdir -p $(dir $@)
	$(PYTHON) $(TOOLS_DIR)/fix_asm.py < $< | $(AS) $(AS_FLAGS) -o $@ -

# Hand-written asm in src/us/asm/*.s
# Output mirrors source layout (build/src/us/asm/foo.s.o) so the
# splat-generated ld script's hasm references resolve.
$(BUILD_DIR)/$(SRC_ASM_DIR)/%.s.o: $(SRC_ASM_DIR)/%.s
	@mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -o $@ $<

# Splat-produced binary blobs -> build/assets/*.bin.o
$(BUILD_DIR)/assets/%.bin.o: $(BIN_DIR)/%.bin
	@mkdir -p $(dir $@)
	$(LD) -r -b binary -o $@ $<

# C compile pipeline: .c -> .i (cpp) -> .s_c (cc1) -> .obj (asn64) -> .o (psyq-obj-parser)
$(BUILD_DIR)/src/%.i: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CPP) -MMD -MP -MT $@ -MF $@.d $(CPP_FLAGS) -o $@ $<

$(BUILD_DIR)/src/%.s_c: $(BUILD_DIR)/src/%.i
	unix2dos $<
	$(CC) $(CC_FLAGS) -o $@ $<

$(BUILD_DIR)/src/%.c.obj: $(BUILD_DIR)/src/%.s_c
	# Prepend `.set noat` BEFORE unix2dos so the inserted line gets CRLF
	# along with the rest. asn64 returns nonzero on $at warnings under
	# wine, so we silence them at the source.
	sed -i -e '1i\.set noat' $<
	unix2dos $<
	sed -i -e 's/.version/#.version/g' $<
	sed -i -e 's/.size/#.size/g' $<
	sed -i -e 's/.type/#.type/g' $<
	$(ASN64) $(ASM_FLAGS) -o $@ $<

$(BUILD_DIR)/src/%.c.o: $(BUILD_DIR)/src/%.c.obj
	$(LNKCONV) $< -o $@ -b -n

# n64crc helper tool
$(N64CRC): $(TOOLS_DIR)/n64crc.c
	$(MAKE) -C $(TOOLS_DIR)

clean:
	rm -rf $(BUILD_DIR)

nuke: clean
	rm -rf $(SPLAT_DIR)/asm $(SPLAT_DIR)/assets $(SPLAT_DIR)/bin

# Auto-include dependency files generated by cpp -MMD
-include $(C_OBJS:.o=.d) $(C_OBJS:.o=.i.d)

.SECONDARY:
SHELL = /bin/bash -e -o pipefail

# Remove a target whose recipe failed. Without this make leaves the partial
# file behind, and because it is newer than its source the next run treats it
# as up to date. That is how an 812-byte virtual_rodata_0.data.s.o survived for
# a 47 KB region: fix_asm.py segfaulted mid-pipe, pipefail correctly failed the
# rule with Error 139, and the truncated object then linked cleanly into every
# build after it. The link failed 235 symbols away, on symbols that really were
# defined -- in the part of the file the crash had cut off.
.DELETE_ON_ERROR:
