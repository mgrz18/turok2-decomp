# Not matching yet

C that does what the original does, written and checked against the
disassembly, but that does not yet compile to the same bytes: register
allocation and instruction order still differ. These files are not built;
the asm is. They are here so the reading of each function is not lost while
the permuter (`tools/permuter/`) or a person finishes it. When one matches,
it moves to `src/` through `tools/integrate.py` and leaves this folder.

No draft is left. The last one, `engine/func_00246BC0.c` (player input from
the controller, 6 of 330 words off: a dead reload of the control mode and a
pinned register), matched (a scripted search guided by the sched2 and dbr dumps; the
permuter stalled one load short) and is in
`src/us/code/func_00246BC0.c`, as a FAKEMATCH (its first comment says why).
The drafts before it matched in the F pass and the G pass and are in `src/`
(docs/MATCHING.md, "The F pass", "The G pass"); the VM modules' last one,
func_00424008, matched there too.
