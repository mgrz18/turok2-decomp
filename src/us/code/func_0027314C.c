/* Update one sound: refresh its volume, notice when it stopped or must stop, then slide its volume, pan and pitch. */

#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct SoundBank {
    /* 0x000 */ u8 pad0[0x84];
    /* 0x084 */ u8 player[0xDC - 0x84];        /* ALSndPlayer */
    /* 0x0DC */ s16 soundIds[(0x104 - 0xDC) / 2];
    /* 0x104 */ s32 current;
    /* 0x108 */ u8 pad108[0x134 - 0x108];
    /* 0x134 */ s32 effect;
    /* 0x138 */ u8 pad138[0x2B98 - 0x138];
    /* 0x2B98 */ s32 volume;
} SoundBank;

/* The bank seen from 0x7C: the ROM addresses its sound ids from there. */
typedef struct SoundVoice {
    /* 0x00 */ u8 pad0[0x60];
    /* 0x60 */ s16 ids[1];                     /* SoundBank.soundIds */
} SoundVoice;

typedef struct Sound {
    /* 0x00 */ s32 index;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 id;
    /* 0x14 */ u8 pad14[4];
    /* 0x18 */ s32 loops;
    /* 0x1C */ u8 pad1C[0x2C - 0x1C];
    /* 0x2C */ f32 volume;
    /* 0x30 */ u8 pad30[8];
    /* 0x38 */ s16 unk38;
    /* 0x3A */ s16 unk3A;
    /* 0x3C */ u8 pad3C[8];
    /* 0x44 */ Vec3 pos;
    /* 0x50 */ Vec3 *follow;
    /* 0x54 */ u8 pad54[0xA0 - 0x54];
    /* 0xA0 */ s32 delay;
    /* 0xA4 */ s32 flags;
    /* 0xA8 */ s32 effect;
    /* 0xAC */ s32 stop;
    /* 0xB0 */ SoundBank *bank;
    /* 0xB4 */ s32 unkB4;
    /* 0xB8 */ u8 padB8[4];
    /* 0xBC */ s32 hasEffect;
} Sound;

extern u8 D_801198B0[];

extern s32 func_00281444(void *game, Vec3 *pos);
extern void func_00293E60(void *player, s32 id);    /* alSndpSetSound */
extern s32 func_00293580(void *player);             /* alSndpGetState */
extern void func_00293EC0(void *player);            /* alSndpStop */
extern void func_00293CE0(void *player);
extern void func_00293530(void *player, s32 id);    /* alSndpDeallocate */
extern void func_002726DC(Sound *s);
extern void func_00272B54(Sound *s);
extern void func_00272EC4(Sound *s);

void func_0027314C(Sound *s) {
    SoundBank *bank;
    s32 playing;
    void *player;

    s->bank->volume = func_00281444(D_801198B0, &s->pos);
    bank = s->bank;
    func_00293E60(bank->player, bank->soundIds[s->index]);
    playing = func_00293580(bank->player);
    if ((s->volume <= 0.075f && s->effect != 0x228) || ((s->flags & 4) && s->loops == 0)) {
        s->stop = 1;
    }
    if (s->hasEffect && s->bank->effect != s->effect) {
        s->stop = 1;
    }
    if (playing) {
        if (s->stop) {
            SoundBank *b = s->bank;
            s->stop = 1;
            s->follow = NULL;
            player = b->player;
            if (s->id != b->current) {
                func_00293E60(player, b->soundIds[s->index]);
                if (func_00293580(player)) {
                    func_00293EC0(player);
                }
                s->unk4 = -1;
            }
            return;
        }
    } else if (s->delay > 0) {
        s->delay--;
        s->flags |= 0x10;
        return;
    }
    if ((s->flags & 0x18) == 0x10) {
        func_00293CE0(s->bank->player);
        return;
    }
    if (!playing) {
        SoundBank *b = s->bank;
        SoundVoice *v = (SoundVoice *)((u8 *)b + 0x7C);
        func_00293530(b->player, v->ids[s->index]);
        v->ids[s->index] = -1;
        s->unk38 = 0;
        s->unkC = -1;
        s->unk8 = -1;
        s->effect = -1;
        s->unk3A = -1;
        s->unkB4 = -1;
        return;
    }
    if (s->follow != NULL) {
        s->pos = *s->follow;
    }
    func_002726DC(s);
    func_00272B54(s);
    func_00272EC4(s);
}
