/* A looping sound's pitch: slide its offset (in cents) toward the target and set the pitch. */

#include "common.h"

typedef struct SoundBank {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ u8 player[0xDC - 0x84];        /* ALSndPlayer */
    /* 0x0DC */ s16 soundIds[(0x104 - 0xDC) / 2];
    /* 0x104 */ s32 flags;
} SoundBank;

typedef struct SoundParams {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 pitch;
} SoundParams;

typedef struct SoundTarget {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 cents;                      /* high half of an f32 */
    /* 0x06 */ u8 pad6[2];
} SoundTarget;

typedef struct Sound {
    /* 0x00 */ s32 index;
    /* 0x04 */ u8 pad4[0xC];
    /* 0x10 */ SoundParams params;
    /* 0x28 */ u8 pad28[0x34 - 0x28];
    /* 0x34 */ f32 volume;
    /* 0x38 */ u8 pad38[0x60 - 0x38];
    /* 0x60 */ SoundTarget target;
    /* 0x68 */ s16 delay;
    /* 0x6A */ u8 pad6A[2];
    /* 0x6C */ f32 cents;
    /* 0x70 */ f32 step;
    /* 0x74 */ u8 pad74[0xA4 - 0x74];
    /* 0xA4 */ s32 flags;
    /* 0xA8 */ u8 padA8[0xB0 - 0xA8];
    /* 0xB0 */ SoundBank *bank;
    /* 0xB4 */ u8 padB4[4];
    /* 0xB8 */ f32 pitchScale;
} Sound;

extern f32 D_800B2848[];                    /* pitch ratio every 60 cents from -6000 */
#define PITCH_TABLE (D_800B2848 + 1)

extern void func_00272D3C(Sound *s);
extern f32 func_0026D210(s32 halfFloat);
extern void func_00293E60(void *player, s32 id);    /* alSndpSetSound */
extern void func_00293DE0(void *player, f32 pitch); /* alSndpSetPitch */

static __inline__ f32 centsToPitch(f32 x) {
    f32 pos = (x + 6000.0f) * 0.016666668f;
    s16 i = pos;
    f32 fi = i;
    s16 j;
    f32 a;

    if (fi != pos) {
        j = i + 1;
        a = PITCH_TABLE[i];
        return a + (pos - fi) * (PITCH_TABLE[j] - a);
    }
    return PITCH_TABLE[i];
}

static __inline__ void setPitch(Sound *s, f32 pitch) {
    SoundBank *bank = s->bank;

    func_00293E60(bank->player, bank->soundIds[s->index]);
    func_00293DE0(bank->player, pitch * s->volume * s->pitchScale);
}

void func_00272EC4(Sound *s) {
    SoundParams *params;
    SoundTarget *target;
    SoundBank *bank;
    s32 update;
    s32 down;
    f32 pitch;

    update = 0;
    params = &s->params;
    target = &s->target;
    bank = s->bank;
    if ((s->flags & 0x400) && !(bank->flags & 3)) {
        func_00272D3C(s);
        update = 1;
    }
    if ((s->flags & 0xA) == 0xA) {
        if (s->delay > 0) {
            s->delay--;
        } else {
            down = 0;
            if (s->step < 0.0f) {
                down = 1;
            }
            if (down ? s->cents < func_0026D210(target->cents) : func_0026D210(target->cents) < s->cents) {
                s->cents -= s->step;
                if (down) {
                    if (func_0026D210(target->cents) < s->cents) {
                        s->cents = func_0026D210(target->cents);
                    }
                } else {
                    if (s->cents < func_0026D210(target->cents)) {
                        s->cents = func_0026D210(target->cents);
                    }
                }
                pitch = centsToPitch(s->cents);
                params->pitch = pitch;
                setPitch(s, pitch);
                return;
            }
        }
    }
    if (update) {
        setPitch(s, params->pitch);
    }
}
