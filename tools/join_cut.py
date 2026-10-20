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
        called |= set(re.findall(r"\b(?:jal|j)\s+(func_[0-9A-F]{8})\b", b))
    for p in (ROOT / "src").rglob("*.c"):
        called |= set(re.findall(r"\b(func_[0-9A-F]{8})\s*\(", re.sub(r"^\w[^\n]*\{", "", p.read_text(), flags=re.M)))

    def owner(addr):
        import bisect
        i = bisect.bisect_right(starts, addr) - 1
        if i >= 0:
            a = starts[i]
            if addr < a + funcs[a][1]:
                return a
        return None

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
        if f + fsize != g or gname in called or gname in named:
            continue
        if not re.search(rf"\b(?:b\w*|j)\s+(?:[^,\n]*,\s*)*\.L{lab:08X}\b", bodies.get(fname, "")):
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
    EXTRA.write_text("".join(l for l in EXTRA.read_text().splitlines(keepends=True)
                             if not (m := re.match(r"^\.L([0-9A-F]{8}) =", l)) or int(m.group(1), 16) not in inner))
    with SIZES.open("a") as fh:
        fh.write("\n// Functions splat cut at an early jr ra, joined (tools/join_cut.py).\n")
        for ch in groups:
            total = ch[-1] + funcs[ch[-1]][1] - ch[0]
            fh.write(f"{funcs[ch[0]][0]} = 0x{ch[0]:08X}; // type:func size:0x{total:X}\n")
    yaml = YAML.read_text()
    for ch in groups:
        for a in ch[1:]:
            n = funcs[a][0]
            yaml = re.sub(rf"^\s+- \[0x[0-9A-F]+, c, code/{n}\]\n", "", yaml, flags=re.M)
            if n in have:
                have[n].unlink()
    YAML.write_text(yaml)
    return 0


if __name__ == "__main__":
    sys.exit(main())
