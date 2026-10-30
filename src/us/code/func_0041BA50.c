/* WRONG CUT: real function 0x0041BA50, size 0x1B4 (through 0x0041BC04): splat cut its tail
 * off as func_0041BBB0. */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ u8 active;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ s8 index;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
    /* 0x33 */ u8 colors[13];
} SetupSlot; /* 0x40 bytes */

typedef struct {
    /* 0x000 */ u32 options;
    /* 0x004 */ u32 unlocked;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s16 unk10;
    /* 0x012 */ u8 unk12;
    /* 0x013 */ u8 unk13;
    /* 0x014 */ u8 unk14;
    /* 0x015 */ u8 unk15;
    /* 0x016 */ u8 unk16;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 unk18;
    /* 0x019 */ u8 unk19;
    /* 0x01A */ u8 unk1A;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ f32 unk1C;
    /* 0x020 */ u8 unk20;
    /* 0x021 */ u8 unk21;
    /* 0x022 */ u8 unk22;
    /* 0x023 */ u8 unk23;
    /* 0x024 */ u8 unk24;
    /* 0x025 */ s8 minutes;
    /* 0x026 */ s8 unk26;
    /* 0x027 */ s8 level;
    /* 0x028 */ u8 unk28;
    /* 0x029 */ u8 unk29;
    /* 0x02A */ u8 unk2A;
    /* 0x02B */ u8 unk2B;
    /* 0x02C */ u8 unk2C;
    /* 0x02D */ u8 unk2D;
    /* 0x02E */ u8 unk2E;
    /* 0x02F */ u8 pad2F;
    /* 0x030 */ u8 unk30[10];
    /* 0x03A */ u8 pad3A[2];
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 unk3D;
    /* 0x03E */ u8 unk3E;
    /* 0x03F */ u8 unk3F;
    /* 0x040 */ SetupSlot extra;
    /* 0x080 */ SetupSlot slots[4];
    /* 0x180 */ u8 unk180;
} GameSetup;

extern void func_0041BC50(SetupSlot *slot);

static __inline__ void InitSlot(SetupSlot *slot, u8 index) {
    slot->active = 0;
    slot->unk29 = 0x80;
    slot->unk2A = 0x80;
    slot->unk2C = 0;
    slot->unk2B = 0;
    slot->unk2D = 1;
    slot->unk2E = 0;
    slot->index = index;
    slot->unk30 = 0;
    slot->unk31 = 1;
    slot->unk32 = 0;
    func_0041BC50(slot);
}

/* Default multiplayer setup. */
void func_0041BA50(GameSetup *s) {
    s32 i;

    s->unk13 = 1;
    s->unk14 = 0x80;
    s->unk15 = 0xFA;
    s->unk16 = 0xFF;
    s->unk10 = 0x80;
    s->unk8 = 0x10;
    s->unkC = 0;
    s->unk17 = 1;
    s->unk18 = 0;
    s->options = 0;
    s->unlocked = 0;
    s->unk1B = 0;
    s->unk180 = 0;
    s->unk20 = 10;
    s->unk21 = 0;
    s->unk22 = 0;
    s->unk23 = 0;
    s->unk24 = 1;
    s->minutes = 0;
    s->unk26 = 0;
    s->level = 0;
    s->unk28 = 0;
    s->unk29 = 1;
    s->unk2A = 10;
    s->unk2B = 0;
    s->unk2C = 0;
    s->unk2D = 0;
    s->unk2E = 1;
    s->unk12 = s->unk10;
    s->unk1C = 1.0f;
    for (i = 0; i < 10; i++) {
        s->unk30[i] = 1;
    }
    s->unk3C = 0;
    s->unk3D = 3;
    s->unk3E = 0;
    s->unk3F = 3;
    for (i = 0; i < 4; i++) {
        InitSlot((SetupSlot *) ((u8 *) s + 0x80 + i * 0x40), i);
    }
    InitSlot((SetupSlot *) ((u8 *) s + 0x40), 0);
    s->extra.active = 1;
    s->unk19 = 0;
    s->unk1A = 1;
    s->options = 0x4000000;
    s->unlocked = 0x4000000;
}
