/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_13.c, func_8023C85C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028E6DC(s32 arg0);
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern s32 D_801206C4;
extern void func_002052D8(void *arg0, void *arg1);
void func_0029B030(void *arg0, s32 arg1, s32 arg2);
void osSendMesg(void *arg0, void *arg1, s32 arg2);
extern void func_0029B030(void *arg0, s32 arg1, s32 arg2);
extern void osSendMesg(void *arg0, void *arg1, s32 arg2);
void func_0028E6DC(s32 arg0) {
    char sp10[0x18];
    Block_func_8023C85C_de block;
    s32 sp38;
    void *sp3C;
    func_0029B030(sp10, (s32)&sp38, 1);
    block.type = 2;
    block.value = arg0;
    block.data = sp10;
    osSendMesg(&D_801206C4, &block, 1);
    osRecvMesg(sp10, &sp3C, 1);
    func_002052D8(0, ((Draw *)(sp3C))->model);
}
