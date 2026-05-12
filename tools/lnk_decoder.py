#!/usr/bin/env python3
"""
lnk_decoder.py - Decode SN64 LNK debug records embedded in N64 ROMs.

SN64 (SN Systems / Acclaim N64 toolchain) shipped object files in a format
descended from PSY-Q (PlayStation 1) LNK. Turok 2: Seeds of Evil (N64) keeps
three of those records around offset 0x107000 of the US ROM, each starting
with the 4-byte magic "LNK\\x02".

This decoder walks the bytecode stream record by record and reports the
sections, source filenames, exported/imported/local symbols, function
boundaries, line tables, etc. that it recognises. Unknown opcodes are
flagged and the parse for that record stops (but we attempt to find the
next record by re-scanning the file for the magic).

Stdlib only. Tested with Python 3.11+.
"""
from __future__ import annotations

import argparse
import io
import json
import struct
import sys
from dataclasses import dataclass, field
from typing import Any, BinaryIO

MAGIC = b"LNK\x02"

# Opcode table - inherited from PSY-Q (see pcsx-redux/tools/psyq-obj-parser).
# Numbers match decimal values; the wire format uses single bytes.
OP_END                        = 0
OP_BYTES                      = 2
OP_SWITCH                     = 6
OP_ZEROES                     = 8
OP_RELOCATION                 = 10
OP_EXPORTED_SYMBOL            = 12
OP_IMPORTED_SYMBOL            = 14
OP_SECTION                    = 16
OP_LOCAL_SYMBOL               = 18
OP_FILENAME                   = 28
OP_PROGRAMTYPE                = 46
OP_UNINITIALIZED              = 48
OP_INC_SLD_LINENUM            = 50
OP_INC_SLD_LINENUM_BY_BYTE    = 52
OP_INC_SLD_LINENUM_BY_WORD    = 54
OP_SET_SLD_LINENUM            = 56
OP_SET_SLD_LINENUM_FILE       = 58
OP_END_SLD                    = 60
OP_FUNCTION                   = 74
OP_FUNCTION_END               = 76
OP_BLOCK_START                = 78
OP_BLOCK_END                  = 80
OP_SECTION_DEF                = 82
OP_SECTION_DEF2               = 84
OP_FUNCTION_START2            = 86

OPCODE_NAMES = {
    OP_END: "END",
    OP_BYTES: "BYTES",
    OP_SWITCH: "SWITCH",
    OP_ZEROES: "ZEROES",
    OP_RELOCATION: "RELOCATION",
    OP_EXPORTED_SYMBOL: "EXPORTED_SYMBOL",
    OP_IMPORTED_SYMBOL: "IMPORTED_SYMBOL",
    OP_SECTION: "SECTION",
    OP_LOCAL_SYMBOL: "LOCAL_SYMBOL",
    OP_FILENAME: "FILENAME",
    OP_PROGRAMTYPE: "PROGRAMTYPE",
    OP_UNINITIALIZED: "UNINITIALIZED",
    OP_INC_SLD_LINENUM: "INC_SLD_LINENUM",
    OP_INC_SLD_LINENUM_BY_BYTE: "INC_SLD_LINENUM_BY_BYTE",
    OP_INC_SLD_LINENUM_BY_WORD: "INC_SLD_LINENUM_BY_WORD",
    OP_SET_SLD_LINENUM: "SET_SLD_LINENUM",
    OP_SET_SLD_LINENUM_FILE: "SET_SLD_LINENUM_FILE",
    OP_END_SLD: "END_SLD",
    OP_FUNCTION: "FUNCTION",
    OP_FUNCTION_END: "FUNCTION_END",
    OP_BLOCK_START: "BLOCK_START",
    OP_BLOCK_END: "BLOCK_END",
    OP_SECTION_DEF: "SECTION_DEF",
    OP_SECTION_DEF2: "SECTION_DEF2",
    OP_FUNCTION_START2: "FUNCTION_START2",
}

# Expression opcodes used inside RELOCATION entries.
EXPR_VALUE = 0
EXPR_SYMBOL = 2
EXPR_SECTION_BASE = 4
EXPR_SECTION_START = 12
EXPR_SECTION_END = 22
EXPR_ADD = 44
EXPR_SUB = 46
EXPR_DIV = 50


class ParseError(Exception):
    pass


class Reader:
    def __init__(self, buf: bytes, base_offset: int = 0):
        self.buf = buf
        self.pos = 0
        self.base = base_offset

    def remaining(self) -> int:
        return len(self.buf) - self.pos

    def tell(self) -> int:
        return self.pos

    def abs(self) -> int:
        return self.base + self.pos

    def u8(self) -> int:
        if self.remaining() < 1:
            raise ParseError("EOF reading u8")
        v = self.buf[self.pos]
        self.pos += 1
        return v

    def u16(self) -> int:
        if self.remaining() < 2:
            raise ParseError("EOF reading u16")
        (v,) = struct.unpack_from("<H", self.buf, self.pos)
        self.pos += 2
        return v

    def u32(self) -> int:
        if self.remaining() < 4:
            raise ParseError("EOF reading u32")
        (v,) = struct.unpack_from("<I", self.buf, self.pos)
        self.pos += 4
        return v

    def bytes_n(self, n: int) -> bytes:
        if self.remaining() < n:
            raise ParseError(f"EOF reading {n} bytes")
        s = self.buf[self.pos : self.pos + n]
        self.pos += n
        return s

    def pstring(self) -> str:
        n = self.u8()
        raw = self.bytes_n(n)
        try:
            return raw.decode("ascii")
        except UnicodeDecodeError:
            return raw.decode("latin-1", errors="replace")


@dataclass
class Section:
    index: int
    group: int
    alignment: int
    name: str


@dataclass
class Symbol:
    kind: str  # "EXPORT" | "IMPORT" | "LOCAL" | "UNINIT"
    index: int | None
    section_index: int | None
    offset: int | None
    size: int | None
    name: str


@dataclass
class Function:
    section: int
    offset: int
    file_id: int
    start_line: int
    end_line: int | None
    frame_reg: int
    frame_size: int
    name: str


@dataclass
class LnkRecord:
    rom_offset: int
    version: int
    program_type: int | None = None
    sections: list[Section] = field(default_factory=list)
    files: dict[int, str] = field(default_factory=dict)
    symbols: list[Symbol] = field(default_factory=list)
    functions: list[Function] = field(default_factory=list)
    line_entries: list[dict] = field(default_factory=list)
    opcode_histogram: dict[str, int] = field(default_factory=dict)
    truncated_at: int | None = None
    truncated_reason: str | None = None
    end_offset: int | None = None

    def bump(self, op_name: str):
        self.opcode_histogram[op_name] = self.opcode_histogram.get(op_name, 0) + 1


def parse_expression(r: Reader) -> dict:
    op = r.u8()
    if op == EXPR_VALUE:
        return {"type": "value", "value": r.u32()}
    if op == EXPR_SYMBOL:
        return {"type": "symbol", "index": r.u16()}
    if op == EXPR_SECTION_BASE:
        return {"type": "section_base", "section": r.u16()}
    if op == EXPR_SECTION_START:
        return {"type": "section_start", "section": r.u16()}
    if op == EXPR_SECTION_END:
        return {"type": "section_end", "section": r.u16()}
    if op == EXPR_ADD:
        right = parse_expression(r)
        left = parse_expression(r)
        return {"type": "add", "right": right, "left": left}
    if op == EXPR_SUB:
        right = parse_expression(r)
        left = parse_expression(r)
        return {"type": "sub", "right": right, "left": left}
    if op == EXPR_DIV:
        right = parse_expression(r)
        left = parse_expression(r)
        return {"type": "div", "right": right, "left": left}
    raise ParseError(f"Unknown expression op {op}")


def parse_record(buf: bytes, start: int) -> LnkRecord:
    if buf[start : start + 4] != MAGIC:
        raise ParseError(f"No LNK magic at 0x{start:x}")
    # The 4-byte magic "LNK\x02" already includes the version byte (0x02).
    # Opcode stream begins immediately after.
    version = MAGIC[3]
    rec = LnkRecord(rom_offset=start, version=version)
    r = Reader(buf[start + 4 :], base_offset=start + 4)
    cur_function_name = ""
    cur_function_start = 0

    while r.remaining() > 0:
        op_pos = r.abs()
        op = r.u8()
        op_name = OPCODE_NAMES.get(op, f"UNKNOWN_{op}")
        try:
            if op == OP_END:
                rec.bump(op_name)
                rec.end_offset = r.abs()
                break
            elif op == OP_BYTES:
                size = r.u16()
                r.bytes_n(size)
                rec.bump(op_name)
            elif op == OP_SWITCH:
                r.u16()
                rec.bump(op_name)
            elif op == OP_ZEROES:
                r.u32()
                rec.bump(op_name)
            elif op == OP_RELOCATION:
                r.u8()   # reloc type
                r.u16()  # offset
                parse_expression(r)
                rec.bump(op_name)
            elif op == OP_EXPORTED_SYMBOL:
                sym_idx = r.u16()
                sec_idx = r.u16()
                off = r.u32()
                name = r.pstring()
                rec.symbols.append(Symbol("EXPORT", sym_idx, sec_idx, off, None, name))
                rec.bump(op_name)
            elif op == OP_IMPORTED_SYMBOL:
                sym_idx = r.u16()
                name = r.pstring()
                rec.symbols.append(Symbol("IMPORT", sym_idx, None, None, None, name))
                rec.bump(op_name)
            elif op == OP_SECTION:
                sec_idx = r.u16()
                group = r.u16()
                alignment = r.u8()
                name = r.pstring()
                rec.sections.append(Section(sec_idx, group, alignment, name))
                rec.bump(op_name)
            elif op == OP_LOCAL_SYMBOL:
                sec_idx = r.u16()
                off = r.u32()
                name = r.pstring()
                rec.symbols.append(Symbol("LOCAL", None, sec_idx, off, None, name))
                rec.bump(op_name)
            elif op == OP_FILENAME:
                idx = r.u16()
                name = r.pstring()
                rec.files[idx] = name
                rec.bump(op_name)
            elif op == OP_PROGRAMTYPE:
                rec.program_type = r.u8()
                rec.bump(op_name)
            elif op == OP_UNINITIALIZED:
                sym_idx = r.u16()
                sec_idx = r.u16()
                size = r.u32()
                name = r.pstring()
                rec.symbols.append(Symbol("UNINIT", sym_idx, sec_idx, None, size, name))
                rec.bump(op_name)
            elif op == OP_INC_SLD_LINENUM:
                r.u16()
                rec.bump(op_name)
            elif op == OP_INC_SLD_LINENUM_BY_BYTE:
                r.u16(); r.u8()
                rec.bump(op_name)
            elif op == OP_INC_SLD_LINENUM_BY_WORD:
                r.u16(); r.u16()
                rec.bump(op_name)
            elif op == OP_SET_SLD_LINENUM:
                offset = r.u16()
                line = r.u32()
                rec.line_entries.append({"offset": offset, "line": line})
                rec.bump(op_name)
            elif op == OP_SET_SLD_LINENUM_FILE:
                offset = r.u16()
                line = r.u32()
                file_id = r.u16()
                rec.line_entries.append({"offset": offset, "line": line, "file": file_id})
                rec.bump(op_name)
            elif op == OP_END_SLD:
                r.u16()
                rec.bump(op_name)
            elif op == OP_FUNCTION:
                section = r.u16()
                offset = r.u32()
                file_id = r.u16()
                start_line = r.u32()
                frame_reg = r.u16()
                frame_size = r.u32()
                retn_pc_reg = r.u16()
                mask = r.u32()
                mask_offset = r.u32()
                name = r.pstring()
                cur_function_name = name
                cur_function_start = offset
                rec.functions.append(Function(section, offset, file_id, start_line,
                                              None, frame_reg, frame_size, name))
                rec.bump(op_name)
            elif op == OP_FUNCTION_END:
                section = r.u16()
                offset = r.u32()
                end_line = r.u32()
                if rec.functions and rec.functions[-1].name == cur_function_name:
                    rec.functions[-1].end_line = end_line
                rec.bump(op_name)
            elif op == OP_BLOCK_START:
                r.u16(); r.u32(); r.u32()
                rec.bump(op_name)
            elif op == OP_BLOCK_END:
                r.u16(); r.u32(); r.u32()
                rec.bump(op_name)
            elif op == OP_SECTION_DEF:
                r.u16(); r.u32(); r.u16(); r.u16(); r.u32(); r.pstring()
                rec.bump(op_name)
            elif op == OP_SECTION_DEF2:
                r.u16(); r.u32(); r.u16(); r.u16(); r.u32()
                dims = r.u16()
                for _ in range(dims):
                    r.u16()
                r.pstring()  # tag
                r.pstring()  # name
                rec.bump(op_name)
            elif op == OP_FUNCTION_START2:
                section = r.u16()
                offset = r.u32()
                file_id = r.u16()
                start_line = r.u32()
                frame_reg = r.u16()
                frame_size = r.u32()
                retn_pc_reg = r.u16()
                mask = r.u32()
                mask_offset = r.u32()
                r.u32(); r.u32()  # unk1, unk2
                name = r.pstring()
                cur_function_name = name
                cur_function_start = offset
                rec.functions.append(Function(section, offset, file_id, start_line,
                                              None, frame_reg, frame_size, name))
                rec.bump(op_name)
            else:
                rec.truncated_at = op_pos
                rec.truncated_reason = f"Unknown opcode 0x{op:02x} ({op})"
                break
        except ParseError as e:
            rec.truncated_at = op_pos
            rec.truncated_reason = f"Parse error at op {op_name}: {e}"
            break

    return rec


def find_records(buf: bytes) -> list[int]:
    offs = []
    pos = 0
    while True:
        i = buf.find(MAGIC, pos)
        if i < 0:
            break
        offs.append(i)
        pos = i + 4
    return offs


def collect_all_files(records: list[LnkRecord]) -> list[str]:
    seen = {}
    for rec in records:
        for idx, name in rec.files.items():
            seen.setdefault(name, None)
    return sorted(seen.keys())


def emit_human(records: list[LnkRecord], out: io.TextIOBase):
    for rec in records:
        out.write(f"\n=== LNK record @ 0x{rec.rom_offset:08x} (version {rec.version}) ===\n")
        if rec.program_type is not None:
            out.write(f"  program type: {rec.program_type}\n")
        out.write(f"  sections ({len(rec.sections)}):\n")
        for s in rec.sections:
            out.write(f"    [{s.index:3d}] align={s.alignment:2d} group={s.group:#x} {s.name}\n")
        out.write(f"  source files ({len(rec.files)}):\n")
        for idx in sorted(rec.files):
            out.write(f"    [{idx:3d}] {rec.files[idx]}\n")
        if rec.symbols:
            out.write(f"  symbols ({len(rec.symbols)}):\n")
            for sym in rec.symbols:
                if sym.kind == "EXPORT":
                    out.write(f"    EXPORT  sec={sym.section_index:>3} off={sym.offset:#010x}  {sym.name}\n")
                elif sym.kind == "IMPORT":
                    out.write(f"    IMPORT                              {sym.name}\n")
                elif sym.kind == "LOCAL":
                    out.write(f"    LOCAL   sec={sym.section_index:>3} off={sym.offset:#010x}  {sym.name}\n")
                elif sym.kind == "UNINIT":
                    out.write(f"    UNINIT  sec={sym.section_index:>3} size={sym.size:#010x}  {sym.name}\n")
        if rec.functions:
            out.write(f"  functions ({len(rec.functions)}):\n")
            for fn in rec.functions:
                end = f"end_line={fn.end_line}" if fn.end_line is not None else ""
                out.write(f"    sec={fn.section:>3} off={fn.offset:#010x} file={fn.file_id} "
                          f"line={fn.start_line} {end} {fn.name}\n")
        if rec.line_entries:
            out.write(f"  line entries: {len(rec.line_entries)}\n")
        out.write("  opcode histogram:\n")
        for name, cnt in sorted(rec.opcode_histogram.items(), key=lambda kv: -kv[1]):
            out.write(f"    {name:28s} {cnt}\n")
        if rec.truncated_at is not None:
            out.write(f"  TRUNCATED @ 0x{rec.truncated_at:08x}: {rec.truncated_reason}\n")
        elif rec.end_offset is not None:
            out.write(f"  clean END @ 0x{rec.end_offset:08x}\n")


def emit_json(records: list[LnkRecord], out: io.TextIOBase):
    payload = []
    for rec in records:
        payload.append({
            "rom_offset": rec.rom_offset,
            "version": rec.version,
            "program_type": rec.program_type,
            "sections": [s.__dict__ for s in rec.sections],
            "files": rec.files,
            "symbols": [s.__dict__ for s in rec.symbols],
            "functions": [f.__dict__ for f in rec.functions],
            "line_entries": rec.line_entries,
            "opcode_histogram": rec.opcode_histogram,
            "truncated_at": rec.truncated_at,
            "truncated_reason": rec.truncated_reason,
            "end_offset": rec.end_offset,
        })
    json.dump(payload, out, indent=2)
    out.write("\n")


def emit_symbols(records: list[LnkRecord], out: io.TextIOBase):
    """Emit splat-compatible symbol assignments.

    LNK symbols carry section-relative offsets. Without the relocation pass we
    cannot turn those into final VRAM addresses, so we emit them as comments
    that document name + (section_index, offset) for downstream tools.
    """
    out.write("// Generated by tools/lnk_decoder.py from SN64 LNK records in baserom.us.z64.\n")
    out.write("// Symbols are listed with their LNK section index + section-relative offset,\n")
    out.write("// pending a real linker pass to assign final VRAM addresses.\n\n")

    seen = set()
    for rec in records:
        # Build a per-record section name map.
        sec_name = {s.index: s.name for s in rec.sections}
        files = rec.files
        out.write(f"// ---- record @ 0x{rec.rom_offset:08x} ----\n")
        for sym in rec.symbols:
            key = (sym.name, sym.kind)
            if key in seen:
                continue
            seen.add(key)
            sname = sec_name.get(sym.section_index, "?") if sym.section_index is not None else "-"
            if sym.kind == "EXPORT":
                out.write(f"{sym.name} = 0x{sym.offset:08x}; // EXPORT section={sname}\n")
            elif sym.kind == "LOCAL":
                out.write(f"// LOCAL  {sname:>10}+{sym.offset:#010x} {sym.name}\n")
            elif sym.kind == "IMPORT":
                out.write(f"// IMPORT {sym.name}\n")
            elif sym.kind == "UNINIT":
                out.write(f"// BSS    {sname:>10} size={sym.size:#x} {sym.name}\n")
        for fn in rec.functions:
            src = files.get(fn.file_id, "?")
            out.write(f"// FUNC   sec={fn.section} off={fn.offset:#010x} {fn.name}  ({src}:{fn.start_line})\n")
    out.write("\n// Source files referenced:\n")
    for path in collect_all_files(records):
        out.write(f"//   {path}\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--rom", required=True, help="Path to baserom (raw bytes)")
    ap.add_argument("--start", default="0x107000",
                    help="Optional starting offset hint (decimal or 0x...). The tool always scans the whole ROM for additional records.")
    ap.add_argument("--json", action="store_true", help="Emit JSON")
    ap.add_argument("--symbols", action="store_true",
                    help="Emit splat-style symbol stubs (mutually exclusive with --json)")
    ap.add_argument("--limit", type=int, default=None,
                    help="Stop after N records (debug)")
    args = ap.parse_args()

    if args.json and args.symbols:
        print("error: --json and --symbols are mutually exclusive", file=sys.stderr)
        return 2

    with open(args.rom, "rb") as f:
        buf = f.read()

    offsets = find_records(buf)
    start_hint = int(args.start, 0)
    # Reorder so the hint comes first if present.
    if start_hint in offsets:
        offsets = [start_hint] + [o for o in offsets if o != start_hint]
    if args.limit is not None:
        offsets = offsets[: args.limit]

    print(f"; found {len(offsets)} LNK record(s) in {args.rom}", file=sys.stderr)
    for o in offsets:
        print(f";   0x{o:08x}", file=sys.stderr)

    records = [parse_record(buf, o) for o in offsets]

    if args.json:
        emit_json(records, sys.stdout)
    elif args.symbols:
        emit_symbols(records, sys.stdout)
    else:
        emit_human(records, sys.stdout)

    return 0


if __name__ == "__main__":
    sys.exit(main())
