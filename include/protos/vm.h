/* What the VM pager module (src/us/code/VM) calls of its own and of the TLB helpers, declared
 * once (round 29, part r2). */

#ifndef PROTOS_VM_H
#define PROTOS_VM_H

#include "common.h"
#include "structs/pager.h"

/* stvm_tlb (asm, docs/ASM_ORIGIN.md): osMapTLB's arguments */
void func_0028EE40(s32 index, s32 pm, void *vaddr, u32 evenpaddr, u32 oddpaddr, s32 asid);
void func_0028EEC0(s32 index);                  /* stvm_tlb: osUnmapTLB */
s32 VM_AcquireNextTLB(void);
void VM_MapTLBEntry(s16 page, PageMapping *mapping, s32 unused, u8 *pair);
void VM_UnmapTLBEntry(PageMapping *mapping);   /* releases its TLB slot */

#endif
