#!/usr/bin/env python3
"""m2c for code built with -mfp64: every float register holds a whole value.

The engine is compiled `-mhard-float -mfp64`, which runs the FPU with FR=1:
thirty-two 64-bit registers, where a float can live in $f1 or $f3 and a
double does not take two registers. m2c only knows the FR=0 layout, where
$f1 is the upper half of the double in $f0. So it reads `add.s $f1, ...` as
touching half of a double, prints `(second half of f64)`, and fuses two
`lwc1` into consecutive registers into one double load. 611 of the 1,418
engine functions m2c could not draft usefully use odd float registers.

This runs m2c unchanged except for three points, patched at import:

- `other_f64_reg` no longer pairs $fN with $fN^1. It names a register no
  instruction uses, so a double's "other half" never aliases a real one.
- `Mips1DoubleLoadStorePattern` is dropped: `lwc1 $f0` + `lwc1 $f1` are two
  floats here, not one double.
- `InstrArgs.dreg` reads a double from its one register, the way m2c
  already does for PPC.

Usage is m2c's own:
    python3 tools/m2c_fp64.py -t mips-gcc-c --valid-syntax func.s
"""

import sys

import m2c.arch_mips as arch_mips
import m2c.translate as translate
from m2c.asm_instruction import Register
from m2c.main import main


def other_f64_reg(reg):
    assert reg.is_float(), "tried to get complement reg of non-floating point register"
    # f64..f95: valid float register names that no instruction ever mentions.
    return Register(f"f{64 + int(reg.register_name[1:])}")


def dreg(self, index):
    reg = self.reg_ref(index)
    if not reg.is_float():
        raise translate.DecompFailure(f"Expected instruction argument {reg} to be a float register")
    return self.regs[reg]


arch_mips.other_f64_reg = other_f64_reg
arch_mips.MipsArch.asm_patterns = [
    p for p in arch_mips.MipsArch.asm_patterns
    if not isinstance(p, arch_mips.Mips1DoubleLoadStorePattern)
]
translate.InstrArgs.dreg = dreg

if __name__ == "__main__":
    main()
