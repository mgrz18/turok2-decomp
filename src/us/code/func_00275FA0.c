/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_6.c, func_80259260_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void SndVoice_SetSoundId(void *arg0, int arg1);
extern s16 SoundEffectInfo_GetGroup(void *);
extern s16 SoundEffectInfo_GetSample(void *);
extern s8 SoundEffectInfo_GetPitch(void *);
extern void SndVoice_Setup(void *, void *, s32);
extern s8 SoundEffectInfo_GetPriority(void *);
extern u8 SoundEffectInfo_GetPan(void *);
extern s16 SoundEffectInfo_GetFlags(void *);
s32 SndQueue_Add(ManagerRecAtD8V1 *manager, void *desc, Vec3Words *position, s32 volume, s32 extra) {
    extern void CAudioWorld__Unlock(s32 *);
    extern void CAudioWorld__Lock(s32 *);
    NodeRecAtC8 *node;
    NodeRecAtC8 *first;
    NodeRecAtC8 *at;
    s32 *scene;
    s32 priority;
    if (manager->free.next != (NodeRecAtC8 *)&manager->free) {
        CAudioWorld__Lock(manager->scene);
        first = manager->free.next;
        first->prev->next = first->next;
        first->next->prev = first->prev;
        scene = manager->scene;
        node = first;
        node->handle = scene[0x108 / 4];
        scene[0x108 / 4] = node->handle + 1;
        node->volume = volume;
        node->voice = -1;
        node->volume16 = volume;
        node->group = SoundEffectInfo_GetGroup(desc);
        node->priority = SoundEffectInfo_GetPriority(desc);
        node->sample = SoundEffectInfo_GetSample(desc);
        node->pan = SoundEffectInfo_GetPan(desc);
        node->flags = SoundEffectInfo_GetFlags(desc);
        node->pitch = SoundEffectInfo_GetPitch(desc);
        node->gainB = 0x7D00;
        node->gainA = 0x7D00;
        node->time = 0;
        node->frame = manager->scene[0x104 / 4];
        node->position = *position;
        node->pad58 = 0;
        node->padB4 = 0;
        SndVoice_SetSoundId(node->channel, ((struct ContextRecAt2BC0 *)manager->scene)->field2B8C);
        node->mix = manager->scene[0x2BAC / 4];
        SndVoice_Setup(node->channel, desc, extra);
        at = manager->active.next;
        priority = node->priority;
        while (at != (NodeRecAtC8 *)&manager->active) {
            if (at->priority < priority) {
                break;
            }
            at = at->next;
        }
        node->prev = at->prev;
        node->next = at;
        at->prev->next = node;
        at->prev = node;
        CAudioWorld__Unlock(manager->scene);
        return node->handle;
    }
    return -1;
}
