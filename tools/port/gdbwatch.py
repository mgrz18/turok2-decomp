"""gdb script (round 18 g2): watch a level actor's position between two det lines.

    T2_DET_LOG=1 T2_DET_WATCH=FRAME:ACTOR[:LEN] G2W=NAME [G2TRACE=REGEX] [G2RAND=1] [G2LD=EXE] \
      python3 tools/port/aimdrive.py -- gdb -q -batch -x tools/port/gdbwatch.py --args turok2 ...

The game calls T2Det_WatchBegin(&actor->m_vPos.x) at FRAME's det line and T2Det_WatchEnd LEN
frames later (port/game/t2det.c). In between: a hardware watchpoint logs every write with a
10-frame backtrace, and G2TRACE (a gdb basic regex: `^Collision_\\|^Sweep`) logs each call of the
matching functions. Output: $G2WDIR (else $TMPDIR, else /tmp)/watch_NAME.log. G2RAND=1 keeps ASLR
on (gdb turns it off); G2LD=EXE when the command is `/lib64/ld-linux-x86-64.so.2 EXE ...`
(STABILITY.md 12.1).
"""
import gdb

gdb.execute("set pagination off")
gdb.execute("set confirm off")
gdb.execute("set print thread-events off")
for sig in ("SIGPIPE", "SIGUSR1", "SIGUSR2", "SIGALRM", "SIGCHLD"):
    gdb.execute("handle %s nostop noprint pass" % sig)

import os
out = open(os.path.join(os.environ.get("G2WDIR", os.environ.get("TMPDIR", "/tmp")), "watch_%s.log" % os.environ.get("G2W", "x")), "w")
gdb.execute("set breakpoint pending on")
if os.environ.get("G2RAND"):
    gdb.execute("set disable-randomization off")


class Watch(gdb.Breakpoint):
    def stop(self):
        v = gdb.parse_and_eval("*(float *) %d" % self.addr)
        out.write("W %r\n" % float(v))
        f = gdb.newest_frame()
        k = 0
        while f is not None and k < 10:
            out.write("  %s\n" % (f.name() or hex(f.pc())))
            f = f.older()
            k += 1
        out.flush()
        return False


class Trace(gdb.Breakpoint):
    def stop(self):
        out.write("C %s rdi=%x\n" % (self.location, int(gdb.parse_and_eval("$rdi")) & 0xFFFF))
        return False


def trace_all():
    pat = os.environ.get("G2TRACE")
    if not pat:
        return
    txt = gdb.execute("info functions %s" % pat, to_string=True)
    names = set()
    for line in txt.splitlines():
        f = line.split()
        if len(f) == 2 and f[0].startswith("0x"):
            names.add(f[1])
    for n in sorted(names):
        Trace(n, internal=True)
    out.write("TRACE %d functions\n" % len(names))


class Begin(gdb.Breakpoint):
    def stop(self):
        a = int(gdb.parse_and_eval("$rdi"))
        out.write("BEGIN %x\n" % a)
        trace_all()
        w = Watch("*(float *) %d" % a, gdb.BP_WATCHPOINT, gdb.WP_WRITE, internal=True)
        w.addr = a
        return False


class End(gdb.Breakpoint):
    def stop(self):
        out.write("END\n")
        out.close()
        return True


if os.environ.get("G2LD"):
    # the exe run through ld.so: gdb does not see it as an objfile; load its symbols at its base
    gdb.execute("starti")
    gdb.execute("break _dl_debug_state")
    exe = os.environ["G2LD"]
    while True:
        gdb.execute("continue")
        pid = gdb.selected_inferior().pid
        base = None
        for line in open("/proc/%d/maps" % pid):
            f = line.split()
            if len(f) >= 6 and f[5] == exe and f[2] == "00000000":
                base = int(f[0].split("-")[0], 16)
                break
        if base is not None:
            break
    gdb.execute("delete")
    gdb.execute("add-symbol-file %s -o 0x%x" % (exe, base))
    Begin("T2Det_WatchBegin", internal=True)
    End("T2Det_WatchEnd", internal=True)
    gdb.execute("continue")
else:
    Begin("T2Det_WatchBegin", internal=True)
    End("T2Det_WatchEnd", internal=True)
    gdb.execute("run")
gdb.execute("kill")
gdb.execute("quit")
