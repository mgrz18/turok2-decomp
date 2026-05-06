# turok2-decomp Makefile — bootstrap stage.
#
# Status: this Makefile only supports `make setup` (splat disasm) so far.
# The compile/link/checksum pipeline will be added once the splat config
# is producing usable .s output. See references/turok3/Makefile for the
# fuller version we'll converge on.

GAME_VERSION ?= us
BASENAME      = turok2
VERSION_DIR   = versions
TOOLS_DIR     = tools
SPLAT_YAML    = $(VERSION_DIR)/$(BASENAME).$(GAME_VERSION).yaml

PYTHON ?= python3

.PHONY: all setup clean nuke help

help:
	@echo "turok2-decomp targets:"
	@echo "  make setup [GAME_VERSION=us]  - splat-disasm the ROM"
	@echo "  make clean                    - remove build/ outputs"
	@echo "  make nuke                     - remove generated splat output too"

setup:
	$(PYTHON) -m splat split $(SPLAT_YAML)

clean:
	rm -rf build/

nuke: clean
	rm -rf us/asm us/bin us/assets

all: setup
