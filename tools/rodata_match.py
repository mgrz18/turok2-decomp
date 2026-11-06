#!/usr/bin/env python3
"""Link a C file's object at its ROM place and compare .text and .rodata.

tools/match_func.py masks every relocated field and ignores the object's
data; tools/asm_diff.py compares two compiles, so it reports any edit that
moves a literal from an `extern D_X` into the C. This is the check for that
move: it compiles the file through the build's own pipeline (cpp, cc1,
tools/sn_as.sh), places the .text where build/ref.elf has the file's first
function and the .rodata at its slice of the engine's .rdata, resolves every
relocation (named symbols through ref.elf and versions/symbols_manual, the
object's own sections through those placements) and holds every word of both
sections against the ROM. The slice is found, not given: the object's
.rodata must sit at one of the .rdata addresses the base revision's code
reads (the lowest it can), byte for byte.

With --write, a file that matches gets its slice in versions/turok2.us.yaml:
its `[rom, .rodata, code/<name>]` subsegment in code_rodata is set to the
slice found (created, moved or grown over the `rodata` asm around it), and the
asm left on either side keeps a `[rom, rodata]` entry of its own. It refuses
when the slice reaches into another file's subsegment, or when a label inside
it (which the regenerated asm will no longer define) is still named by other
source: src/, include/, the hand-written asm or the other data asm.

Usage:
    python3 tools/rodata_match.py src/us/code/func_0027ECF0.c ...
    python3 tools/rodata_match.py --write src/us/code/func_0027ECF0.c ...
    python3 tools/rodata_match.py --slice 0x800A5A10 src/us/code/func_00218B10.c

Prints `OK <file> text <vram>+<size> rodata <vram>+<size> (ROM <off>)`
per file; exit status 0 when every file matches.
"""

import argparse
import os
import re
import struct
import subprocess
import sys
import tempfile
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from asm_diff import CPP_FLAGS, CC_FLAGS, FLAGS_LINE, symbols  # noqa: E402
from match_func import sym_rom  # noqa: E402

CC1 = ROOT / "tools" / "sn64" / "cc1"
ROM = (ROOT / "baserom.us.z64").read_bytes()
# The engine's .rdata (jump tables, float pools, strings), as (vram, ROM).
RDATA = (0x800A51F8, 0x800AB0A8, 0xA5DF8)


def compile_obj(src, out):
    inc = ["-I" + str(ROOT), "-I" + str(ROOT / "include"), "-I" + str(ROOT / "us" / "include")]
    with tempfile.TemporaryDirectory(dir=os.environ.get("TMPDIR")) as tmp:
        i, s = Path(tmp) / "f.i", Path(tmp) / "f.s"
        subprocess.run(["cpp", *CPP_FLAGS, *inc, "-o", str(i), str(src)], check=True)
        m = FLAGS_LINE.search(Path(src).read_text(errors="replace"))
        flags = (m.group(1) if m else CC_FLAGS).split()
        r = subprocess.run([str(CC1), *flags, "-o", str(s), str(i)], capture_output=True, text=True)
        if r.returncode:
            raise RuntimeError("cc1: " + " ".join(l for l in r.stderr.splitlines() if "warning" not in l)[:300])
        subprocess.run([str(ROOT / "tools" / "sn_as.sh"), str(s), str(out)], check=True,
                       stderr=subprocess.DEVNULL, env={**os.environ, "INC_DIR": str(ROOT / "us" / "include")})


def ref_info(elf_path):
    """name -> (vram, rom) for every function symbol in ref.elf."""
    out = {}
    with open(elf_path, "rb") as fh:
        elf = ELFFile(fh)
        for sym in elf.get_section_by_name(".symtab").iter_symbols():
            if sym["st_info"]["type"] == "STT_FUNC" and sym.name:
                rom = sym_rom(elf, sym)
                if rom is not None:
                    out.setdefault(sym.name, (sym["st_value"], rom))
    return out


def sections(elf):
    return {sec.name: (i, sec) for i, sec in enumerate(elf.iter_sections())}


def relocate(elf, idx, data, place, syms):
    """Apply the REL relocations of section `idx` to `data` (a bytearray)."""
    symtab = elf.get_section_by_name(".symtab")
    rels = []
    for sec in elf.iter_sections():
        if isinstance(sec, RelocationSection) and sec["sh_info"] == idx:
            rels += list(sec.iter_relocations())
    pending = []          # HI16s waiting for their LO16
    for r in rels:
        sym = symtab.get_symbol(r["r_info_sym"])
        if sym["st_info"]["type"] == "STT_SECTION" or sym["st_shndx"] != "SHN_UNDEF" and sym.name in ("",):
            secname = elf.get_section(sym["st_shndx"]).name
            S = place[secname]
        elif sym["st_shndx"] != "SHN_UNDEF":
            S = place[elf.get_section(sym["st_shndx"]).name] + sym["st_value"]
        else:
            if sym.name not in syms:
                raise KeyError(sym.name)
            S = syms[sym.name]
        off, t = r["r_offset"], r["r_info_type"]
        w = struct.unpack(">I", data[off:off + 4])[0]
        if t == 2:                                    # R_MIPS_32
            w = (w + S) & 0xFFFFFFFF
        elif t == 4:                                  # R_MIPS_26
            w = (w & 0xFC000000) | (((((w & 0x03FFFFFF) << 2) + S) >> 2) & 0x03FFFFFF)
        elif t == 5:                                  # R_MIPS_HI16
            pending.append((off, S))
            continue
        elif t == 6:                                  # R_MIPS_LO16
            lo = w & 0xFFFF
            lo -= 0x10000 if lo & 0x8000 else 0
            for hoff, hS in pending:
                hw = struct.unpack(">I", data[hoff:hoff + 4])[0]
                val = ((hw & 0xFFFF) << 16) + lo + hS
                hw = (hw & 0xFFFF0000) | (((val + 0x8000) >> 16) & 0xFFFF)
                data[hoff:hoff + 4] = struct.pack(">I", hw)
            pending = []
            w = (w & 0xFFFF0000) | ((lo + S) & 0xFFFF)
        else:
            raise ValueError(f"relocation type {t}")
        data[off:off + 4] = struct.pack(">I", w)
    if pending:
        raise ValueError("HI16 without LO16")


def check(src, syms, funcs, slice_vram=None, verbose=False):
    with tempfile.TemporaryDirectory(dir=os.environ.get("TMPDIR")) as tmp:
        obj = Path(tmp) / "f.o"
        try:
            compile_obj(src, obj)
        except (RuntimeError, subprocess.CalledProcessError) as e:
            return str(e)
        with open(obj, "rb") as fh:
            elf = ELFFile(fh)
            secs = sections(elf)
            ti, text = secs[".text"]
            first = min((s for s in elf.get_section_by_name(".symtab").iter_symbols()
                         if s["st_shndx"] == ti and s["st_info"]["type"] == "STT_FUNC"),
                        key=lambda s: s["st_value"])
            if first.name not in funcs:
                return f"{first.name} not in ref.elf"
            tv, trom = funcs[first.name]
            tv -= first["st_value"]
            trom -= first["st_value"]
            ro = secs.get(".rodata")
            rodata = bytes(ro[1].data()) if ro and ro[1]["sh_size"] else b""
            for name in (".data", ".bss", ".sdata", ".sbss", ".rdata"):
                if name in secs and secs[name][1]["sh_size"]:
                    return f"object has {name}"
            cands = [slice_vram] if slice_vram else []
            if rodata and not cands:
                # the rdata addresses the ROM's code reads: every %hi/%lo pair
                cands = sorted(rdata_reads(trom, text["sh_size"], tv))
            rv = None
            if rodata:
                for c in cands:
                    data = bytearray(rodata)
                    relocate(elf, ro[0], data, {".text": tv, ".rodata": c}, syms)
                    r = RDATA[2] + c - RDATA[0]
                    if RDATA[0] <= c and c + len(data) <= RDATA[1] and ROM[r:r + len(data)] == bytes(data):
                        rv = c
                        break
                if rv is None:
                    return f"rodata ({len(rodata):#x} bytes) is nowhere the code reads " + \
                        " ".join(f"{c:08X}" for c in cands[:8])
            data = bytearray(text.data())
            try:
                relocate(elf, ti, data, {".text": tv, ".rodata": rv or 0}, syms)
            except KeyError as e:
                return f"symbol {e} not in ref.elf"
            want = ROM[trom:trom + len(data)]
            if bytes(data) != want:
                bad = [i for i in range(0, len(data), 4) if data[i:i + 4] != want[i:i + 4]]
                if verbose:
                    for i in bad[:20]:
                        print(f"    {tv + i:08X}: rom {want[i:i+4].hex()} obj {data[i:i+4].hex()}")
                return f"text: {len(bad)} words differ (first at {tv + bad[0]:08X})"
            msg = f"text {tv:08X}+{len(data):#x}"
            if rv:
                msg += f" rodata {rv:08X}+{len(rodata):#x} (ROM {RDATA[2] + rv - RDATA[0]:#X})"
            return "OK " + msg


def rdata_reads(rom, size, vram):
    """.rdata addresses the ROM's code at rom..rom+size builds with lui/addiu-or-load."""
    out = set()
    words = struct.unpack(f">{size // 4}I", ROM[rom:rom + size])
    his = {}
    for w in words:
        op, rt, rs = w >> 26, (w >> 16) & 31, (w >> 21) & 31
        if op == 0x0F:
            his[rt] = (w & 0xFFFF) << 16
        elif op in (0x09, 0x23, 0x31, 0x35, 0x21, 0x25, 0x20, 0x24, 0x37) and rs in his:
            lo = w & 0xFFFF
            lo -= 0x10000 if lo & 0x8000 else 0
            a = (his[rs] + lo) & 0xFFFFFFFF
            if RDATA[0] <= a < RDATA[1]:
                out.add(a)
    return out


YAML = ROOT / "versions" / "turok2.us.yaml"
ENTRY = re.compile(r"^(\s+)- \[(0x[0-9A-Fa-f]+),\s*([.\w]+)(?:,\s*([\w/]+))?\s*\]")


def own_slice(name, start, end):
    """Give code/<name> the ROM range [start, end) of code_rodata in the yaml.
    Returns None, or why not."""
    lines = YAML.read_text().splitlines()
    seg = next(i for i, l in enumerate(lines) if l.strip() == "- name: code_rodata")
    stop = next(i for i in range(seg + 1, len(lines)) if lines[i].startswith("  - name:"))
    ents = [(i, ENTRY.match(lines[i])) for i in range(seg, stop)]
    ents = [(i, int(m.group(2), 16), m.group(3), m.group(4), m.group(1)) for i, m in ents if m]
    indent = ents[0][4]
    me = f"code/{name}"
    seg_end = RDATA[2] + RDATA[1] - RDATA[0]
    spans = [(a, ents[k + 1][1] if k + 1 < len(ents) else seg_end, typ, sub, i)
             for k, (i, a, typ, sub, _) in enumerate(ents)]
    hit = [s for s in spans if s[0] < end and start < s[1]]
    for a, b, typ, sub, _ in hit:
        if typ != "rodata" and sub != me:
            return f"reaches into {sub} at {a:#X}"
    old = [s for s in spans if s[3] == me]
    for a, b, *_ in old:
        if a < start or b > end:
            return f"its old slice {a:#X}-{b:#X} is not inside {start:#X}-{end:#X}"
    first, last = hit[0], hit[-1]
    new = []
    if first[0] < start:
        new.append(f"{indent}- [0x{first[0]:X}, rodata]")
    new.append(f"{indent}- [0x{start:X}, .rodata, {me}]")
    if end < last[1]:
        if last[2] != "rodata":
            return "ends inside its own old slice"
        new.append(f"{indent}- [0x{end:X}, rodata]")
    lines[first[4]:last[4] + 1] = new
    YAML.write_text("\n".join(lines) + "\n")
    return None


def labels_named_elsewhere(src, lo, hi, elf_path):
    """Labels ref.elf has in [lo, hi) that source other than `src` still names."""
    names = []
    with open(elf_path, "rb") as fh:
        for sym in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if sym.name and lo <= sym["st_value"] < hi and sym["st_info"]["type"] != "STT_SECTION" \
                    and not sym.name.startswith((".", "$")):
                names.append(sym.name)
    if not names:
        return []
    pat = re.compile(r"\b(" + "|".join(map(re.escape, names)) + r")\b")
    out = set()
    roots = [ROOT / "src", ROOT / "include", ROOT / "us" / "asm"]
    for root in roots:
        for p in root.rglob("*"):
            if p.suffix not in (".c", ".h", ".s") or p.resolve() == Path(src).resolve():
                continue
            for line in p.read_text(errors="replace").splitlines():
                s = line.strip()
                if s.startswith((".globl", "glabel", "dlabel", ".size", ".type")) or s.endswith(":"):
                    continue
                for m in pat.finditer(line):
                    out.add(f"{m.group(1)} ({p.relative_to(ROOT)})")
    return sorted(out)


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("files", nargs="+")
    ap.add_argument("--slice", type=lambda s: int(s, 16), help="vram of the .rodata slice")
    ap.add_argument("--elf", default=str(ROOT / "build" / "ref.elf"))
    ap.add_argument("-v", "--verbose", action="store_true")
    ap.add_argument("--write", action="store_true", help="give each matching file its slice in the yaml")
    args = ap.parse_args()
    syms = symbols(args.elf)
    funcs = ref_info(args.elf)
    bad = 0
    for f in args.files:
        res = check(f, syms, funcs, args.slice, args.verbose)
        ok = res.startswith("OK")
        m = re.search(r"rodata ([0-9A-F]{8})\+(0x[0-9a-f]+)", res)
        if ok and args.write and m:
            lo, size = int(m.group(1), 16), int(m.group(2), 16)
            named = labels_named_elsewhere(f, lo, lo + size, args.elf)
            why = f"labels still named elsewhere: {', '.join(named)}" if named else \
                own_slice(Path(f).stem, RDATA[2] + lo - RDATA[0], RDATA[2] + lo + size - RDATA[0])
            if why:
                ok, res = False, res + "; not written: " + why
            else:
                res += "; yaml written"
        bad += not ok
        print(("" if ok else "BAD ") + f"{f}: {res}")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
