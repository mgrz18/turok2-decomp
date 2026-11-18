#!/usr/bin/env python3
"""aligncheck.py BIN (round 19 merge): the Release build faulted in GameNoteMenu_SelectYes (loading a save) on a
movaps into gLoadSavePersistentData, which gendata.py had placed at 8 mod 16 (T2_ALIGN(4)): clang gives a
typed extern array of 16 bytes or more the ABI's 16-byte alignment. This lists such accesses (heuristic) in a
built binary; fix a hit by adding the symbol to gendata.py ALIGN16."""
# aligncheck.py BIN: aligned SSE accesses (movaps/movdqa/movapd, and SSE ops with a memory operand that
# require 16-byte alignment) whose base is a global data symbol that is not 16-byte aligned in BIN.
# Direct: RIP-relative operands. Via a register: `lea sym(%rip),%reg` then an aligned access through %reg
# within the next 24 instructions of the same function (heuristic).
import re, subprocess, sys
bin_ = sys.argv[1]
syms = {}
for line in subprocess.run(["nm", "-S", "--defined-only", bin_], capture_output=True, text=True).stdout.splitlines():
    p = line.split()
    if len(p) == 4 and p[2] in "dDbBrR":
        syms[int(p[0], 16)] = (p[3], int(p[1], 16))
ALIGNED = re.compile(r"\b(movaps|movapd|movdqa|andps|andnps|orps|xorps|addps|mulps|subps|pand|por|pxor|paddd|paddq|pcmpeqb|pcmpeqd|cmpps|minps|maxps)\b")
dis = subprocess.Popen(["objdump", "-d", "--no-show-raw-insn", bin_], stdout=subprocess.PIPE, text=True)
func = "?"
recent = []   # (reg, sym, addr, countdown)
found = {}
for line in dis.stdout:
    m = re.match(r"^[0-9a-f]+ <(.+)>:", line)
    if m:
        func, recent = m.group(1), []
        continue
    if "\t" not in line:
        continue
    ins = line.split("\t", 1)[1]
    c = re.search(r"#\s*([0-9a-f]+) <([^>+]+)(\+0x[0-9a-f]+)?>", ins)
    if c:
        a = int(c.group(1), 16)
        name = c.group(2)
        ent = syms.get(a - int(c.group(3)[1:], 16) if c.group(3) else a)
        if ent and ent[1] >= 16 and (a % 16) != 0:
            if ALIGNED.search(ins) and "(%rip)" in ins:
                found.setdefault((ent[0], func, "direct"), ins.strip())
            l = re.match(r"\s*lea\s+-?(?:0x[0-9a-f]+)?\(%rip\),(%\w+)", ins)
            if l:
                recent.append([l.group(1), ent[0], a, 24])
    for r in recent:
        if ALIGNED.search(ins) and ("(" + r[0]) in ins and "%xmm" in ins:
            found.setdefault((r[1], func, "via " + r[0]), ins.strip())
        r[3] -= 1
    recent = [r for r in recent if r[3] > 0]
for (s, f, how), ins in sorted(found.items()):
    print("%-36s %-40s %-10s %s" % (s, f, how, ins))
print("aligncheck: %d sites" % len(found))
