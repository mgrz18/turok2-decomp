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

LD_FLAGS          = -T $(LD_SCRIPT) -Map $(TARGET).map --no-check-sections
# Add splat-generated symbol tables only if they exist
ifneq (,$(wildcard $(UNDEFINED_SYMS)))
LD_FLAGS         += -T $(UNDEFINED_SYMS)
endif
ifneq (,$(wildcard $(UNDEFINED_FUNCS)))
LD_FLAGS         += -T $(UNDEFINED_FUNCS)
endif

OBJCOPY_FLAGS     = -O binary

# Source enumeration
S_FILES   = $(wildcard $(ASM_DIR)/*.s) $(wildcard $(ASM_DIR)/data/*.s) $(wildcard $(SRC_ASM_DIR)/*.s)
C_FILES   = $(wildcard $(SRC_DIR)/*.c)
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
	$(LD) $(LD_FLAGS) -o $@

verify: $(TARGET).z64
	@printf '%s  %s\n' '$(EXPECTED_SHA1)' '$(TARGET).z64' | sha1sum -c - \
		|| (echo "produced: $$(sha1sum $(TARGET).z64 | cut -d' ' -f1)"; \
		    echo "expected: $(EXPECTED_SHA1)"; \
		    exit 1)

# ---- Recipes -------------------------------------------------------------

# Splat-produced asm -> build/asm/*.s.o   (us/asm/foo.s -> build/asm/foo.s.o)
#
# Note: splat emits `.set noat` at the top, but the dumped code contains
# 2-operand `div`/`divu`/`mul` etc. pseudo-ops whose gas expansion uses
# $at. We flip to `.set at` on the fly; literal $at references downstream
# still resolve fine — `.set at` only suppresses gas's warning, the
# register name binding is unchanged.
$(BUILD_DIR)/asm/%.s.o: $(ASM_DIR)/%.s
	@mkdir -p $(dir $@)
	sed 's/^\.set noat$$/.set at/' $< | $(AS) $(AS_FLAGS) -o $@ -

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
