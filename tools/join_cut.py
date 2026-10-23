#!/usr/bin/env python3
"""Find functions splat cut in two at an early return, and join them.

A function that returns early (`jr ra`) and goes on after it looks to
splat like two functions: it starts a new one past the `jr ra`. The first
part then branches into the second, to a label splat does not define in
that file; versions/undefined_funcs_extra.us.txt has hundreds of those
labels defined by hand so the link closes. Each such pair is one function,
and neither half can be matched alone: the first has no tail, the second no
head.

A pair is joined when
  - the first part branches to a label inside the second,
  - the second starts exactly where the first ends,
  - nothing calls the second (no `jal`/`j` to its name, no C that calls
    it), and it has no real name in versions/names.us.txt.
Chains (a part cut twice) join whole.

Joining writes the whole size into versions/function_sizes.us.txt, drops
the labels that become internal from undefined_funcs_extra, and takes the
second part's matched C (if any) out of the build: it goes back to asm
inside the whole function, to be matched as one.

Usage (on the build box, after a build):
    tools/join_cut.py            # report
    tools/join_cut.py --write
"""

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import auto_match as A  # noqa: E402
import match_func as M  # noqa: E402

EXTRA = ROOT / "versions" / "undefined_funcs_extra.us.txt"
SIZES = ROOT / "versions" / "function_sizes.us.txt"
YAML = ROOT / "versions" / "turok2.us.yaml"


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("--write", action="store_true")
    args = ap.parse_args()

    labels = {int(a, 16) for a in re.findall(r"^\.L([0-9A-F]{8}) =", EXTRA.read_text(), re.M)}
    syms = M.rom_symbols()
    funcs = {}                                   # vram -> (name, size)
    for name, (off, size) in syms.items():
        m = re.match(r"^func_([0-9A-F]{8})$", name)
        if m and size:
            funcs[int(m.group(1), 16)] = (name, size)
    starts = sorted(funcs)
    named = set(A.named_addresses().values())
    bodies = A.extract_asm()
    called = set()
    for b in bodies.values():
        called |= set(re.findall(r"\bjal\s+(func_[0-9A-F]{8})\b", b))
    # A jump into a function from anywhere but the part just before it is a
    # real entry (a tail call); from the part before, it is the same function.
    jumped = {}
    for n, b in bodies.items():
        for t in re.findall(r"\bj\s+(func_[0-9A-F]{8})\b", b):
            jumped.setdefault(t, set()).add(n)
    for p in (ROOT / "src").rglob("*.c"):
        called |= set(re.findall(r"\b(func_[0-9A-F]{8})\s*\(", re.sub(r"^\w[^\n]*\{", "", p.read_text(), flags=re.M)))

    # Every name a function's asm goes by: func_X and its real name.
    aliases = {a: {n} for a, (n, _) in funcs.items()}
    for real, fx in A.named_addresses().items():
        aliases.setdefault(int(fx[5:], 16), set()).add(real)

    def owner(addr):
        import bisect
        i = bisect.bisect_right(starts, addr) - 1
        if i >= 0:
            a = starts[i]
            if addr < a + funcs[a][1]:
                return a
        return None

    # A branch (not a call) to the very start of the next function names it
    # (func_X), not a label: those count as cut points too.
    for b in bodies.values():
        for t in re.findall(r"\b(?:b\w*|j)\s+(?:[^,\n]*,\s*)*func_([0-9A-F]{8})\b", b):
            labels.add(int(t, 16))
    # first part -> second part, for every label one branches to in the next
    join = {}
    for lab in labels:
        g = owner(lab)
        if g is None:
            continue
        gname, _ = funcs[g]
        prev = [a for a in starts if a < g]
        if not prev:
            continue
        f = prev[-1]
        fname, fsize = funcs[f]
        if f + fsize != g or gname in called or gname in named or jumped.get(gname, set()) - aliases.get(f, {fname}):
            continue
        if not re.search(rf"\b(?:b\w*|j)\s+(?:[^,\n]*,\s*)*(?:\.L|func_){lab:08X}\b", bodies.get(fname, "")):
            continue
        join[f] = g
    # chains: a -> b -> c joins into a
    heads = [f for f in join if f not in join.values()]
    groups = []
    for h in sorted(heads):
        chain, cur = [h], h
        while cur in join:
            cur = join[cur]
            chain.append(cur)
        groups.append(chain)
    have = {p.stem: p for p in (ROOT / "src").rglob("func_*.c")}
    lost = sum(funcs[a][1] for ch in groups for a in ch[1:] if funcs[a][0] in have)
    print(f"{len(labels)} hand-defined labels; {len(groups)} functions cut in parts "
          f"({sum(len(c) - 1 for c in groups)} parts to join); matched tails going back to asm: "
          f"{sum(1 for ch in groups for a in ch[1:] if funcs[a][0] in have)} ({lost:,d} bytes)")
    if not args.write:
        for ch in groups[:10]:
            print("  " + " + ".join(f"{funcs[a][0]}({funcs[a][1]:#x})" for a in ch))
        return 0

    inner = {lab for lab in labels for ch in groups if ch[0] <= lab < ch[-1] + funcs[ch[-1]][1]}
    # A label other code still reaches (the VM modules' second copy jumps
    # into the first) stays defined.
    for ch in groups:
        mine = {funcs[a][0] for a in ch}
        for n, b in bodies.items():
            if n in mine:
                continue
            for t in re.findall(r"\.L([0-9A-F]{8})\b", b):
                inner.discard(int(t, 16))
    EXTRA.write_text("".join(l for l in EXTRA.read_text().splitlines(keepends=True)
                             if not (m := re.match(r"^\.L([0-9A-F]{8}) =", l)) or int(m.group(1), 16) not in inner))
    sizes = SIZES.read_text()
    new = []
    for ch in groups:
        total = ch[-1] + funcs[ch[-1]][1] - ch[0]
        entry = f"{funcs[ch[0]][0]} = 0x{ch[0]:08X}; // type:func size:0x{total:X}"
        # A function joined before (cut twice) has its size here already.
        sizes, n = re.subn(rf"^{funcs[ch[0]][0]} = .*$", entry, sizes, flags=re.M)
        if not n:
            new.append(entry)
    # A tail's own size entry would keep splat cutting there: drop it.
    tails = {funcs[a][0] for ch in groups for a in ch[1:]}
    sizes = "".join(l for l in sizes.splitlines(keepends=True) if l.split(" =", 1)[0] not in tails)
    if new:
        sizes += "\n// Functions splat cut at an early jr ra, joined (tools/join_cut.py).\n" + "\n".join(new) + "\n"
    SIZES.write_text(sizes)
    yaml = YAML.read_text()
    for ch in groups:
        # A head matched as it was (shorter) no longer is: it goes back too.
        for a in ch:
            n = funcs[a][0]
            yaml = re.sub(rf"^\s+- \[0x[0-9A-F]+, (?:c|\.rodata|\.data|\.rdata|\.bss), code/{n}\]\n", "", yaml, flags=re.M)
            if n in have:
                have[n].unlink()
    YAML.write_text(yaml)
    # A tail with a name of its own in the scan's symbols stays a global
    # symbol (and a function to every tool) inside the joined asm: drop it,
    # so splat makes it a label.
    scan = ROOT / "versions" / "symbols_from_scan.us.txt"
    tails = {funcs[a][0] for ch in groups for a in ch[1:]}
    scan.write_text("".join(l for l in scan.read_text().splitlines(keepends=True)
                            if l.split(" =", 1)[0].strip() not in tails))
    return 0


if __name__ == "__main__":
    sys.exit(main())
