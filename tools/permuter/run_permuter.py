#!/usr/bin/env python3
"""Run decomp-permuter with its helper processes retried when qemu crashes.

The permuter preprocesses (cpp) and disassembles (objdump) through
subprocess.check_output. Under the container's qemu those die by a signal
now and then, and an exception there ends the whole run, losing the load of
every function. A process killed by a signal is a crash, not an answer, so
it is run again here; a real error (a positive exit status) still raises.

Same arguments as references/decomp-permuter/permuter.py.
"""

import subprocess
import sys
from pathlib import Path

PERMUTER = Path(__file__).resolve().parent.parent.parent / "references" / "decomp-permuter"
_check_output = subprocess.check_output


def check_output(*args, **kwargs):
    for attempt in range(5):
        try:
            return _check_output(*args, **kwargs)
        except subprocess.CalledProcessError as e:
            if e.returncode >= 0 or attempt == 4:
                raise


subprocess.check_output = check_output
sys.path.insert(0, str(PERMUTER))
sys.argv[0] = str(PERMUTER / "permuter.py")

from src.main import main  # noqa: E402

main()
