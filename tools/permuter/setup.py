#!/usr/bin/env python3
"""Set up a decomp-permuter run for one function.

The permuter (references/decomp-permuter, simonlindholm's) takes a C draft
that compiles close to the target and tries random rewrites of it (statement
order, temporaries, comparison direction, casts...) until the object matches.
Those are the rewrites hand matching here mostly comes down to.

This builds its input directory, build/permuter/<func>/:
  base.c        the draft, preprocessed the way tools/cc_func.sh does it
  target.s/.o   the function's original asm, assembled like the asm build
                (tools/fix_asm.py, then GNU as with the Makefile's flags)
  compile.sh    tools/cc_func.sh: SN64 cc1 -O2, then sn_as.sh
  settings.toml func_name, compiler_type = "gcc"

Everything runs in the turok2-permuter image (tools/permuter/Dockerfile: the
build image plus toml and Levenshtein). Then:

    tools/permuter/run.sh <func> [permuter flags]

Usage:
    tools/permuter/setup.py <draft.c> <func>
    tools/permuter/setup.py <draft.c> <func> <matched.c>
    tools/permuter/setup.py --dir build/near      # every <func>.c in it, one container

With a third argument the target is not the function's asm (there is none once
it is C) but the object of a C file that already matches it, a FAKEMATCH for
one: the permuter then looks for another source of the same code, here a
natural one (round 21 de-faked LightVolume_Construct and Cinema_RunEnd so).
Relocated fields compare as zero on both sides, so check each find with
tools/match_func.py and tools/asm_diff.py.
"""

import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
IMAGE = "turok2-permuter"
AS_FLAGS = "-EB -mabi=32 -mips4 -O1 -I us/include --defsym ASSEMBLER=1 --no-pad-sections"
CPP = ("cpp -P -undef -Wall -lang-c -D_LANGUAGE_C -DF3DEX_GBI_2 -D__GNUC__=2 "
       "-I/work/include -I/work/us/include -nostdinc")


COMPILE_SH = """#!/bin/bash
# invoked as: compile.sh input.c -o output.o
# A compile qemu kills by a signal (exit >= 128) is run again; a real compile
# error is returned at once, since most permutations are expected to fail.
for i in 1 2 3; do
    /work/tools/cc_func.sh "$1" "$3" cc1 -O2 2>/dev/null
    rc=$?
    [ $rc -lt 128 ] && exit $rc
done
exit $rc
"""


def target_asm(func):
    """The function's asm with its file's header, or None."""
    for path in sorted(ROOT.glob("us/asm/*.s")):
        text = path.read_text()
        m = re.search(rf"^\.globl {func}\n\.ent {func}\n.*?^\.end {func}\n", text, re.S | re.M)
        if m:
            header = text[:text.find(".globl ")]
            return header + m.group(0)
    return None


def prepare(draft, func, matched=None):
    """Write the directory's files; return the container commands, or None."""
    asm = target_asm(func) if matched is None else ""
    if asm is None:
        print(f"  {func}: not in us/asm (already C, or a fragment)")
        return None
    out = ROOT / "build" / "permuter" / func
    out.mkdir(parents=True, exist_ok=True)
    if asm:
        (out / "target.s").write_text(asm)
    (out / "settings.toml").write_text(f'func_name = "{func}"\ncompiler_type = "gcc"\n')
    compile_sh = out / "compile.sh"
    compile_sh.write_text(COMPILE_SH)
    compile_sh.chmod(0o755)
    (out / "draft.c").write_text(draft.read_text())
    rel = out.relative_to(ROOT)
    if matched is not None:
        (out / "matched.c").write_text(matched.read_text())
        return (f"{CPP} {rel}/draft.c > {rel}/base.c && "
                f"/work/tools/cc_func.sh {rel}/matched.c {rel}/target.o cc1 -O2 && "
                f"{rel}/compile.sh {rel}/base.c -o {rel}/base.o || echo FAILED {func}")
    return (f"{CPP} {rel}/draft.c > {rel}/base.c && "
            f"python3 tools/fix_asm.py < {rel}/target.s | mips-linux-gnu-as {AS_FLAGS} -o {rel}/target.o - && "
            f"{rel}/compile.sh {rel}/base.c -o {rel}/base.o || echo FAILED {func}")


def main():
    if len(sys.argv) == 3 and sys.argv[1] == "--dir":
        jobs = [(p, p.stem) for p in sorted(Path(sys.argv[2]).glob("func_*.c"))]
    elif len(sys.argv) == 3:
        jobs = [(Path(sys.argv[1]).resolve(), sys.argv[2])]
    elif len(sys.argv) == 4:
        jobs = [(Path(sys.argv[1]).resolve(), sys.argv[2], Path(sys.argv[3]).resolve())]
    else:
        print(__doc__)
        return 1
    cmds = [c for c in (prepare(*j) for j in jobs) if c]
    res = subprocess.run(["docker", "run", "--platform=linux/amd64", "--ulimit", "core=0", "--rm", "-v", f"{ROOT}:/work",
                          "-w", "/work", IMAGE, "bash", "-c", "; ".join(cmds)],
                         capture_output=True, text=True)
    failed = re.findall(r"FAILED (\w+)", res.stdout)
    for f in failed:
        print(f"  {f}: setup failed")
    print(f"ready: {len(cmds) - len(failed)} in build/permuter/")
    return 0


if __name__ == "__main__":
    sys.exit(main())
