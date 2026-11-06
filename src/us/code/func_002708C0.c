/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_15.c, func_80257A14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern void SndQueue_CancelHandle(struct ChannelSet *arg0, s32 arg1);
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern void osSendMesg(void *, s32, s32);
extern void SndVoicePool_StopVoice(void *, s16);
s32 CAudioWorld__StopHandle(void *arg0, s32 id) {
    void *temp_s0;
    void *var_a0;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_v0;
    s32 found;
    s32 i;
    Voice *voice;
    found = 0;
    if (id == -1) {
        return found;
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_a0 = __osDisableInt();
    temp_v1 = ((MenuRules *)(temp_s0))->locked + 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 1) {
        __osRestoreInt(temp_a0);
        osRecvMesg(temp_s0, 0, 1);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    } else {
        __osRestoreInt(temp_a0);
        var_a0 = &((ContextRecAt2BC0 *)(arg0))->view[0];
    }
    SndQueue_CancelHandle(var_a0, id);
    for (i = 0; i < 17; i++) {
        if (((ContextRecAt2BC0 *)arg0)->voices[i].sound == id) {
            found++;
            voice = &((ContextRecAt2BC0 *)arg0)->voices[i];
            if (i == 16) {
                voice->sound = -1;
                voice->priority = 0;
                voice->channel = -1;
                voice->unkC = -1;
                voice->unk8 = -1;
            } else {
                if (voice->pending > 0) {
                    voice->pending = 0;
                    voice->flags &= ~0x10;
                }
                if ((&((ContextCode8025A3EC *)arg0)->slots)[0].ids[i] != -1) {
                    SndVoicePool_StopVoice(&((ContextRecAt2BC0 *)(arg0))->menu[0], i);
                }
            }
        }
    }
    temp_s0 = &((ContextRecAt2BC0 *)(arg0))->lock;
    temp_v0 = __osDisableInt();
    temp_v1 = ((MenuRules *)(temp_s0))->locked - 1;
    ((MenuRules *)(temp_s0))->locked = temp_v1;
    if (temp_v1 != 0) {
        __osRestoreInt(temp_v0);
        osSendMesg(temp_s0, 0, 1);
    } else {
        __osRestoreInt(temp_v0);
    }
    return found;
}
extern u32 __osDisableInt(void);
extern void __osRestoreInt(u32);
extern void osSendMesg(void *, s32, s32);
