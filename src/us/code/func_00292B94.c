/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/seqplayer_802B6BE4.c, func_802B1C34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_00292B94(void *arg0, void *arg1);
extern void func_002933B0(void *arg0);
extern void func_002933E0(void *arg0, void **arg1);
void func_00292B94(void *arg0, void *arg1) {
    ALEventListItem *cur;
    ALEventListItem *next;
    u16 type;
    s32 type16;
    cur = (void *)((ALSeqPlayer *)arg0)->evtq.allocList.next;
    if (cur != 0) {
        type16 = 0x16;
        do {
            type = cur->evt.type;
            next = (void *)cur->node.next;
            if ((type == 0x16 || type == 0x17) &&
                ((ALEvent *)&cur->evt)->msg.osc.vs == arg1) {
                ((ALSeqPlayer *)arg0)->stopOsc(
                    ((ALEvent *)&cur->evt)->msg.osc.oscState);
                func_002933B0(cur);
                if (next != 0) {
                    next->delta = next->delta +
                                        cur->delta;
                }
                func_002933E0(cur, &((ALSeqPlayer *)arg0)->evtq.freeList.next);
                if ((short)type == type16) {
                    ((ALVoiceState *)arg1)->flags &= 0xFE;
                } else {
                    ((ALVoiceState *)arg1)->flags &= 0xFD;
                }
                if (((ALVoiceState *)arg1)->flags == 0) {
                    break;
                }
            }
            cur = next;
        } while (cur != 0);
    }
}
