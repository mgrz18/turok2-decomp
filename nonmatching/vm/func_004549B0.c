/* NON-MATCHING: 106 of 109 words differ (size 0x1B0 vs 0x1B4): same stores and calls; the ROM keeps the constant 1 in $a2/$v1 and walks the 0x30 loop with a pointer ($a1 = s + 9), shifting the whole schedule. */
#include "common.h"

/* A player's options block in the settings, 0x40 bytes (see func_00454B64). */
typedef struct Options {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 port;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 pad33[0x40 - 0x33];
} Options;

/* The multiplayer settings (D_8011AAD8, 0x184 bytes). */
typedef struct Settings {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 flags;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ u16 unk10;
    /* 0x012 */ u8 unk12;
    /* 0x013 */ u8 unk13;
    /* 0x014 */ u8 unk14;
    /* 0x015 */ u8 unk15;
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 unk18;
    /* 0x019 */ u8 multiplayer;
    /* 0x01A */ u8 unk1A;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ f32 speed;
    /* 0x020 */ u8 unk20[0x30 - 0x20];
    /* 0x030 */ u8 unk30[10];
    /* 0x03A */ u8 pad3A[2];
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 unk3D;
    /* 0x03E */ u8 unk3E;
    /* 0x03F */ u8 unk3F;
    /* 0x040 */ Options defaultOptions;
    /* 0x080 */ Options options[4];
    /* 0x180 */ u8 unk180;
} Settings;

void func_0041BBB0(Options *o);

/* func_00454B64 */
static __inline__ void initOptions(Options *o, u8 port) {
    o->unk29 = 0x80;
    o->unk2A = 0x80;
    o->unk28 = 0;
    o->unk2C = 0;
    o->unk2B = 0;
    o->unk2D = 1;
    o->unk2E = 0;
    o->port = port;
    o->unk30 = 0;
    o->unk31 = 1;
    o->unk32 = 0;
    func_0041BBB0(o);
}

/* Resets the multiplayer settings to their defaults. */
void func_004549B0(Settings *s) {
    s32 i;

    s->unk14 = 0x80;
    s->unk15 = 0xFA;
    s->unk16 = 0xFF;
    s->unk10 = 0x80;
    s->unk8 = 16;
    s->unk13 = 1;
    s->unkC = 0;
    s->unk17 = 1;
    s->unk18 = 0;
    s->unk0 = 0;
    s->flags = 0;
    s->unk1B = 0;
    s->unk180 = 0;
    s->unk20[0] = 10;
    s->unk20[1] = 0;
    s->unk20[2] = 0;
    s->unk20[3] = 0;
    s->unk20[4] = 1;
    s->unk20[5] = 0;
    s->unk20[6] = 0;
    s->unk20[7] = 0;
    s->unk20[8] = 0;
    s->unk20[9] = 1;
    s->unk20[10] = 10;
    s->unk20[11] = 0;
    s->unk20[12] = 0;
    s->unk20[13] = 0;
    s->unk20[14] = 1;
    s->unk12 = ((u8 *)&s->unk10)[1];
    s->speed = 1.0f;
    for (i = 9; i >= 0; i--) {
        s->unk30[i] = 1;
    }
    s->unk3C = 0;
    s->unk3D = 3;
    s->unk3E = 0;
    s->unk3F = 3;
    for (i = 0; i < 4; i++) {
        initOptions(&s->options[i], i);
    }
    initOptions(&s->defaultOptions, 0);
    s->defaultOptions.unk28 = 1;
    s->multiplayer = 0;
    s->unk1A = 1;
    s->unk0 = 0x4000000;
    s->flags = 0x4000000;
}
