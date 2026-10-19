# Workflow: getting functions into the build

How matched C reaches `main`, and the tools for each step. Everything heavy
runs on an x86_64 Linux box (cc1 is an i386 binary: native there, emulated on
a Mac); `make verify` passing the SHA1 is the only test that counts.

## Sources of matched C

| Source | Tool | Notes |
|---|---|---|
| m2c drafts | `tools/auto_match.py` | bulk; `--variants`, `--rdata --switch`; `--rw-protos` types the drafts with Rage Wars' prototypes (run both ways: each finds functions the other does not) |
| Other decomps of the same engine | `tools/xmatch_ref.py`, then `tools/import_ref.py` | compile their C with Turok 2's toolchain, find pending functions with the same words, cut each down to a file of its own with Turok 2's symbol names; `--near` writes drafts for the permuter |
| Near misses | `tools/permuter/` | `PERMUTER_JOBS`; check each zero-score output with `tools/match_func.py` (the permuter does not score frame size or immediates) |
| By hand | `tools/match_func.py` | with Ghidra (`tools/ghidra/`) and the structs in `include/structs/` |

Reference decomps, cloned into `references/` (not part of this repo):

```
references/rw_abu    https://github.com/AbuN64/ragewars-decomp
    --include include src --define VERSION_US_REV1
references/rw_junk   https://github.com/thejunkman9000/Turok-Rage-Wars-Decomp
    --include include include/their --define __UNBAKE_STDARG_GCC VERSION_US_REV1
    --exclude port/ wip/ test/
references/ultralib  https://github.com/decompals/ultralib   (libultra 2.0I)
    --src src --include . include include/compiler/gcc include/PR
    --define _MIPS_SZLONG=32 __USE_ISOC99 F3DEX_GBI BUILD_VERSION=VERSION_I NDEBUG _FINALROM
    --cc "tools/sn64/cc1 -quiet -G0 -mips3 -mgas -meb -mno-fix4300 -mhard-float
          -funsigned-char -O3 -mcpu=VR4300 -mgp32 -mfp32 -fforce-addr -fno-builtin"
    (and --cc-flags with the same flags to import_ref, which writes them into the file)
```

The two Rage Wars decomps move every day: pull them and run the cross-match
again now and then.

## Into the build

```
tools/integrate.py @list.txt          # C files that matched on their own
```

It wires them in (`tools/accept_manual.py`), rebuilds from a clean slate,
defines for `ld` the pool symbols and labels asm still names
(`versions/foreign_labels.us.txt`), and leaves out — with the reason, in
`build/integrate/excluded.txt` — any function that breaks the link or the
bytes, then tries again until `make verify` passes. After an import also run
`tools/centralize_types.py --write` (shared types per source decomp,
`include/rw/`) and `tools/rw_protos.py` (prototypes for `--rw-protos`).

## Traps

- `make setup` does not delete asm an earlier split left in `us/`; delete
  `us/` before rebuilding after a yaml change.
- `make verify` compares the ROM it finds: delete both `build/turok2.us.elf`
  and `build/turok2.us.z64` to force a relink.
- A paged VM module is in the ROM twice; the second copy's jumps land in the
  first, so its functions with `j` inside cannot be C. `tools/progress.py`
  leaves that copy out of the count.
- Commits carry no Co-Authored-By trailer; `gh` may switch back to another
  account, so switch to mgrz18 in the same command as the push.
