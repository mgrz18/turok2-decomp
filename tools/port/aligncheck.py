#!/usr/bin/env python3
"""aligncheck.py BIN (round 19 merge): the Release build faulted in GameNoteMenu_SelectYes (loading a save) on a
movaps into gLoadSavePersistentData, which gendata.py had placed at 8 mod 16 (T2_ALIGN(4)): clang gives a
typed extern array of 16 bytes or more the ABI's 16-byte alignment. This lists such accesses (heuristic) in a
built binary; fix a hit by adding the symbol to gendata.py ALIGN16.

Usage: aligncheck.py BIN [--latent]
  The sites whose global is misaligned in BIN are listed and counted (exit status 0 either way).
  --latent also lists the sites whose global gendata.py defines and aligns in BIN only by chance (not in
  its ALIGN16): a layout change can misalign them (round 29: sLitVertexOffsets); and those in ALIGN16.
"""
# Aligned SSE accesses (movaps/movdqa/movapd, and legacy-SSE ops with a memory operand, which require 16-byte
# alignment) whose base is a global data symbol of 16 bytes or more.
# Direct: RIP-relative operands. Via a register: the register holds the symbol's address from a
# `lea sym(%rip),%reg` (or a `mov $sym,%reg`) until something else is written to it, within the function:
# copies (`mov %r1,%r2`, `lea d(%r1),%r2`) carry it, `add/sub $imm` move it inside the symbol, a call drops
# the caller-saved registers. Round 29: there is no instruction window any more; the merged CMtxF.c inlines
# Model_ApplyPackedOffsets into Lighting_BuildLitVertices and the movaps into sLitVertexOffsets came long
# after its lea, so the old 24-instruction window missed it.
import bisect, re, subprocess, sys
from pathlib import Path

args = [a for a in sys.argv[1:] if not a.startswith("--")]
latent = "--latent" in sys.argv
bin_ = args[0]
syms = {}
for line in subprocess.run(["nm", "-S", "--defined-only", bin_], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 4 and p[2] in "dDbBrR":
        syms[int(p[0], 16)] = (p[3], int(p[1], 16))
addrs = sorted(syms)
gd = Path(__file__).resolve().parent / "gendata.py"
m = re.search(r"^ALIGN16 = \{(.*?)\}", gd.read_text(), re.S | re.M) if gd.exists() else None
align16 = set(re.findall(r'"(\w+)"', m.group(1))) if m else set()
# the globals gendata.py defines (port/gen/data_*.c): their alignment is its T2_ALIGN, not the compiler's
gen = set()
for f in (Path(__file__).resolve().parent.parent.parent / "port" / "gen").glob("data_*.c"):
    gen.update(re.findall(r"^T2_ALIGN\(\d+\)[^;=\[]*?\b(\w+)\s*\[", f.read_text(errors="replace"), re.M))


def owner(a):
    """(name, size, start) of the data symbol that contains address a, or None."""
    i = bisect.bisect_right(addrs, a) - 1
    if i >= 0:
        s = addrs[i]
        name, size = syms[s]
        if s <= a < s + max(size, 1):
            return name, size, s
    return None


REG64 = {}
for r in "abcd":
    for n in ("r%sx" % r, "e%sx" % r, "%sx" % r, "%sl" % r, "%sh" % r):
        REG64[n] = "r%sx" % r
for r in ("si", "di", "bp", "sp"):
    for n in ("r" + r, "e" + r, r, r + "l"):
        REG64[n] = "r" + r
for i in range(8, 16):
    for n in ("r%d" % i, "r%dd" % i, "r%dw" % i, "r%db" % i):
        REG64[n] = "r%d" % i
CALLER_SAVED = {"rax", "rcx", "rdx", "rsi", "rdi", "r8", "r9", "r10", "r11"}
NO_WRITE = ("cmp", "test", "ucomis", "comis", "bt", "push", "jmp", "j", "nop", "ud2", "ret")


def reg(tok):
    t = tok.strip()
    return REG64.get(t[1:]) if t.startswith("%") else None


ALIGNED = re.compile(r"^(v?movaps|v?movapd|v?movdqa|andps|andnps|orps|xorps|addps|mulps|subps|divps|sqrtps|"
                     r"pand|pandn|por|pxor|paddd|paddq|psubd|pcmpeqb|pcmpeqd|cmpps|minps|maxps|unpcklps|"
                     r"unpckhps|shufps)$")
MEM = re.compile(r"(-?0x[0-9a-f]+|-?\d+)?\((%\w+)?(?:,(%\w+)(?:,\d)?)?\)")
dis = subprocess.Popen(["objdump", "-d", "--no-show-raw-insn", bin_], stdout=subprocess.PIPE, text=True)
func = "?"
held = {}     # 64-bit register -> (symbol name, size, start, address it holds)
found = {}    # (sym, func, how) -> (instruction, misaligned)
for line in dis.stdout:
    m = re.match(r"^[0-9a-f]+ <(.+)>:", line)
    if m:
        func, held = m.group(1), {}
        continue
    if "\t" not in line:
        continue
    ins = line.split("\t", 1)[1].strip()
    code, _, comment = ins.partition("#")
    parts = code.split(None, 1)
    if not parts:
        continue
    op = parts[0]
    ops = [o.strip() for o in re.split(r",(?![^(]*\))", parts[1])] if len(parts) > 1 else []
    # the global this instruction names (objdump's `# addr <sym+off>` for a RIP-relative operand)
    c = re.search(r"([0-9a-f]+) <([^>+]+)(\+0x[0-9a-f]+)?>", comment)
    named = None
    if c and "(%rip)" in code:
        a = int(c.group(1), 16)
        o = owner(a)
        if o:
            named = o + (a,)
    elif op.startswith("mov") and ops and ops[0].startswith("$0x"):
        o = owner(int(ops[0][1:], 16))
        if o:
            named = o + (int(ops[0][1:], 16),)
    # an aligned access
    if ALIGNED.match(op) and any("%xmm" in o for o in ops):
        for o in ops:
            mm = MEM.search(o)
            if not mm:
                continue
            if mm.group(2) == "%rip":
                if named and named[1] >= 16:
                    found.setdefault((named[0], func, "direct"), (ins, named[3] % 16 != 0))
                continue
            d = int(mm.group(1), 0) if mm.group(1) else 0
            for r, how in ((mm.group(2), "via"), (mm.group(3), "index")):
                h = held.get(reg(r) if r else None)
                if h and h[1] >= 16:
                    a = h[3] + (d if how == "via" else 0)
                    found.setdefault((h[0], func, "%s %s" % (how, r)), (ins, a % 16 != 0))
    # register bookkeeping
    if op.startswith("call"):
        for r in CALLER_SAVED:
            held.pop(r, None)
        continue
    if not ops or op.startswith(NO_WRITE):
        continue
    dst = reg(ops[-1])
    if not dst:
        continue
    src = ops[0] if len(ops) > 1 else None
    if op.startswith("lea") and named:
        held[dst] = named
    elif op.startswith("lea") and src:
        mm = MEM.match(src)
        h = held.get(reg(mm.group(2))) if mm and mm.group(2) and not mm.group(3) else None
        if h:
            held[dst] = h[:3] + (h[3] + (int(mm.group(1), 0) if mm.group(1) else 0),)
        else:
            held.pop(dst, None)
    elif op in ("mov", "movq") and src and reg(src) and reg(src) in held and src.startswith("%r"):
        held[dst] = held[reg(src)]
    elif op.startswith("mov") and named and src and src.startswith("$"):
        held[dst] = named
    elif op in ("add", "sub") and src and src.startswith("$") and dst in held and ops[-1].startswith("%r"):
        k = int(src[1:], 0)
        h = held[dst]
        held[dst] = h[:3] + (h[3] + (k if op == "add" else -k),)
    else:
        held.pop(dst, None)
bad = 0
lat = 0
for (s, f, how), (ins, mis) in sorted(found.items()):
    if mis:
        bad += 1
        print("%-36s %-40s %-14s %s" % (s, f, how, ins))
    elif s in align16:
        if latent:
            print("%-36s %-40s %-14s %s   (ALIGN16)" % (s, f, how, ins))
    elif s in gen:
        lat += 1
        if latent:
            print("%-36s %-40s %-14s %s   (aligned by chance)" % (s, f, how, ins))
print("aligncheck: %d sites" % bad
      + (" (%d more on globals aligned only by chance%s)" % (lat, "" if latent else ": --latent lists them")
         if lat else ""))
