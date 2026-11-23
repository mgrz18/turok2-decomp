#!/usr/bin/env python3
"""Where the game's C files are: one directory per recovered module (docs/MODULES.md).

    src/us/code/<module>/   .code: the engine, libultra/libaudio/libc, libgcc
    src/us/vm1/<module>/    the paged VM module
    src/us/vm2/<module>/    its stale second copy, under the first copy's module names
    src/port_libultra/...   libultra C only the port compiles (the N64 build takes those
                            modules from lib/ultralib: docs/LIBULTRA.md)

versions/modules.us.txt gives each module's directory (`// dir`). A file is still one
function, named func_<vram>.c (or its ultralib name), and the names are unique across the
tree, so a file name finds its file:

    python3 tools/srcfiles.py func_0027ECF0      # print its path
    python3 tools/srcfiles.py --list             # every game C file, the port-only ones too
"""

import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
SRC_US = ROOT / "src" / "us"
GAME_DIRS = (SRC_US / "code", SRC_US / "vm1", SRC_US / "vm2")
PORT_LIBULTRA = ROOT / "src" / "port_libultra"

_index = None


def game_files(port=True):
    """Every game C file (Paths, sorted by path); port=False leaves out src/port_libultra."""
    dirs = GAME_DIRS + ((PORT_LIBULTRA,) if port else ())
    return sorted(p for d in dirs if d.exists() for p in d.rglob("*.c"))


def index():
    """file stem -> Path."""
    global _index
    if _index is None:
        _index = {}
        for p in game_files():
            if p.stem in _index:
                raise SystemExit(f"two files named {p.name}: {_index[p.stem]} and {p}")
            _index[p.stem] = p
    return _index


def find(stem):
    """The Path of the file named stem (func_0027ECF0, func_0027ECF0.c or a path), or None. A
    function of a merged module (one <Module>.c in its directory, tools/merge_module.py) is found
    in that file."""
    stem = Path(stem).name.removesuffix(".c")
    p = index().get(stem)
    if p is None:
        d = module_dirs().get(stem)
        if d is not None and (d / f"{d.name}.c").exists():
            p = d / f"{d.name}.c"
    return p


def rel(stem):
    """The file as a path from the repo root (src/us/code/CList/func_00200500.c), or None."""
    p = find(stem)
    return str(p.relative_to(ROOT)) if p else None


def obj(stem):
    """Its object in a `make` build (build/src/us/code/CList/func_00200500.c.o)."""
    r = rel(stem)
    return f"build/{r}.o" if r else None


def module_dirs():
    """file stem -> its module's directory (a Path), from versions/modules.us.txt: where a
    new file for that function goes. Module-2 files are named at the address splat gives them,
    0x39000 above their link address."""
    import re
    out, d = {}, None
    for line in (ROOT / "versions" / "modules.us.txt").read_text().splitlines():
        m = re.match(r"  // dir (\S+)", line)
        if m:
            d = m.group(1)
            continue
        m = re.match(r"  0x([0-9A-F]{8}) (\w+)", line)
        if m and d:
            v = int(m.group(1), 16) + (0x39000 if d.startswith("src/us/vm2/") else 0)
            out.setdefault(f"func_{v:08X}", ROOT / d)
            out.setdefault(m.group(2), ROOT / d)
    return out


def new_path(stem):
    """Where the file for stem is, or would go (its module's directory)."""
    return find(stem) or module_dirs().get(stem, SRC_US / "code") / f"{stem}.c"


if __name__ == "__main__":
    if sys.argv[1:] == ["--list"]:
        print("\n".join(str(p.relative_to(ROOT)) for p in game_files()))
    else:
        for a in sys.argv[1:]:
            print(rel(a) or f"{a}: no such file")
