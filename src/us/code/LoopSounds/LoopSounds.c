#include "common.h"
#include "m2c_macros.h"

void func_00275BE0(void) {

}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025C8D8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/list.h"
#include "globals/audio.h"
#include "structs/game_object.h"
#include "structs/sound.h"
#include "structs/camera.h"

#ifndef TARGET_PORT /* the port defines LoopSounds_Construct .. LoopSounds_StopAll in port/audio/t2audio_world.c */
void LoopSounds_Construct(void *arg0, SoundSourceNode *arg1, s32 arg2) {
    s32 i;
    CList__Construct(arg0, 0, sizeof(void *));
    CList__Construct(&((SoundVoicePool *)arg0)->playing, 0, sizeof(void *));
    i = 0;
    if (arg2 > 0) {
        do {
            CList__AddTail(arg0, arg1);
            i += 1;
            arg1 = arg1 + 1;
        } while (i < arg2);
    }
    ((SoundVoicePool *)arg0)->state = 0;
}


extern s32 Sound__Play(s16 arg0, CVector3 position, sptr arg4, sptr arg5);

void *LoopSounds_Start(void *arg0, s32 arg1, CVector3 *arg2, void *arg3, sptr arg4) {
    SoundSourceNode *node;
    s32 result;
    node = ((SoundVoicePool *)arg0)->available.m_pHead;
    if (node != 0) {
        CList__Remove(arg0, node);
        CList__AddTail(&((SoundVoicePool *)arg0)->playing, node);
        node->state = -1;
        node->value = -1;
        result = Sound__Play((s16)arg1, *arg2,
                                (sptr)arg3, arg4);
        node->value = result;
        sLoopSoundsLastStarted = result;
        node->state = arg1;
        node->position = *arg2;
        node->owner = arg3;
    }
    return node;
}

s32 Sound__Stop(s32);

s32 LoopSounds_Stop(void *arg0, void *arg1) {
    void *var_s0;
    if (arg1 == 0) {
        return 0;
    }
    var_s0 = ((SoundVoicePool *)arg0)->playing.m_pHead;
    if (var_s0 != 0) {
        do {
            if (var_s0 == arg1) {
                Sound__Stop(((SoundSourceNode *)var_s0)->value);
                sLoopSoundsLastStopped = ((SoundSourceNode *)var_s0)->value;
                CList__Remove(&((SoundVoicePool *)arg0)->playing, var_s0);
                CList__AddTail(arg0, var_s0);
                return 1;
            }
            var_s0 = ((SoundSourceNode *)var_s0)->next;
        } while (var_s0 != 0);
    }
    return 0;
}

typedef struct Node00275DC0 {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ struct Node00275DC0 *next;
    /* 0x8 */ s32 id;
} Node00275DC0;
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Node00275DC0 *head;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 done;
} List00275DC0;

void LoopSounds_Pause(List00275DC0 *arg0) {
    s32 temp_v0;
    Node00275DC0 *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            Sound__Stop(var_s0->id);
            temp_v0 = var_s0->id;
            var_s0 = var_s0->next;
            sLoopSoundsLastPaused = temp_v0;
        } while (var_s0 != NULL);
    }
    arg0->done = 1;
}


typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SndSource *sounds;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 playing;
} SoundOwner;
s32 Sound__Play(s16, CVector3, sptr, sptr);    /* extern */

/* Restarts every looping sound of the owner. */
void LoopSounds_Resume(SoundOwner *arg0) {
    s32 temp_v0;
    SndSource *var_s0;

    var_s0 = arg0->sounds;
    if (var_s0 != NULL) {
        do {
            temp_v0 = Sound__Play(var_s0->sound.h.lo, var_s0->pos, var_s0->flags, -1);
            var_s0->handle = temp_v0;
            var_s0 = var_s0->next;
            sLoopSoundsLastResumed = temp_v0;
        } while (var_s0 != NULL);
    }
    arg0->playing = 0;
}

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ SndSource *head;
    /* 0x18 */ u8 pad18[0x28 - 0x18];
    /* 0x28 */ s32 paused;
} SoundList;
s32 Sound__IsPlaying(s32);                             /* extern */

void LoopSounds_Update(SoundList *arg0) {
    SndSource *var_s0;

    if (arg0->paused == 0) {
        var_s0 = arg0->head;
        if (var_s0 != NULL) {
            do {
                if (Sound__IsPlaying(var_s0->handle) == 0) {
                    var_s0->sound.w = -1;
                    var_s0->handle = -1;
                }
                var_s0 = var_s0->next;
            } while (var_s0 != NULL);
        }
    }
}


/* The free list at 0, the used list at 0x14. */
typedef struct {
    /* 0x00 */ CList m_Free;
    /* 0x14 */ CList m_Used;
} SoundSlotPool;

void LoopSounds_StopAll(SoundSlotPool *arg0) {
    SndSource *temp_s0;
    SndSource *var_s1;

    var_s1 = (SndSource *) arg0->m_Used.m_pHead;
    if (var_s1 != NULL) {
        do {
            temp_s0 = var_s1->next;
            Sound__Stop(var_s1->handle);
            var_s1->sound.w = -1;
            var_s1->handle = -1;
            CList__Remove(&arg0->m_Used, var_s1);
            CList__AddTail(arg0, var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
#endif /* !TARGET_PORT */

#include "protos/game.h"

LoopSounds *LoopSounds_Get(void) {
    return &gLoopSounds;
}

void func_00275F8C(void) {

}
