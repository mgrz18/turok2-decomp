/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205F18.c, func_00205F18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ActorLink {
               u8 pad0[8];
               void (*cb)();
} ActorLink;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef union B01IF { s32 i; f32 f; } B01IF;
typedef union B01FH { f32 f; s32 i; s16 h; } B01FH;
typedef struct B01Vec4 { f32 x, y, z, w; } B01Vec4;
typedef union B01W4 { s32 i; f32 f; s16 h[2]; u8 b[4]; } B01W4;
typedef struct B01Def {
                u8 pad0[0x14];
                u32 flags14;
                u8 pad18[0x4];
                s32 unk1C;
                u8 pad20[0x4];
                s32 unk24;
                s16 unk28;
                u8 pad2A[0x2];
                s32 unk2C;
                u8 pad30[0x8];
                u32 flags38;
                u8 pad3C[0x4];
                s32 unk40;
} B01Def;
typedef struct B01Info {
                u32 flags;
                s32 unk4;
                s16 unk8;
                s16 unkA;
                B01W4 unkC;
                B01FH unk10;
                s16 unk14;
                s16 unk16;
                s16 unk18;
                u8 unk1A;
                u8 unk1B;
                u8 pad1C[0x8];
                u32 flags24;
                u8 pad28[0x4];
                s32 unk2C;
                s32 unk30;
                B01W4 unk34;
                s32 unk38;
                s32 unk3C;
                B01FH unk40;
                u8 pad44[0x4];
                f32 unk48;
                f32 unk4C;
                u8 pad50[0x1C];
                f32 unk6C;
} B01Info;
typedef struct B01Seg {
                u8 b[0x18];
} B01Seg;
typedef struct B01Actor { /* Turok 2 layout */
    u8 pad0[0x14];
    B01Def *def;            /* 0x14 */
    u8 pad18[0xA0];
    u16 unkB8;              /* 0xB8: actor type id (Rage Wars: 0xE4) */
    u8 padBA[0x1A];
    u32 flags;              /* 0xD4 */
} B01Actor;
typedef struct B01State {
                u32 flags;
                s32 unk4;
                u8 pad8[0x24];
                void *table;
                ActorLink *link;
                s8 unk34;
                s8 unk35;
                u8 pad36[0x1];
                s8 unk37;
                u8 pad38[0x8];
                f32 unk40;
                u8 pad44[0x20];
                f32 unk64;
                u8 pad68[0x2C];
                s8 unk94;
                u8 pad95[0x1];
                u16 unk96;
                u16 unk98;
                u8 pad9A[0x30];
                u8 unkCA;
                s8 unkCB;
                u8 padCC[0x30];
                void *unkFC;
                u8 pad100[0x8];
                void (*fn108)();
                B01IF unk10C;       /* Turok 2: 0x10C (Rage Wars: 0x124) */
                B01IF unk110;
                Vec3f vec130;
                f32 unk13C;
                s32 unk140;
                s32 unk144;
                s32 unk148;
                s32 unk14C;
                u8 pad150[0x18];
                s32 unk168;
                s32 unk16C;
} B01State;
extern u8 D_8011AAF1[];
extern u8 D_8011AAF2;
f32 func_002119FC(f32 a, f32 b);
void func_00243414(B01Actor *actor, B01State *state, s32 event);
void func_0026342C(B01Actor *actor, B01State *state) {
    B01Actor *a2;
    B01Info *new_var;
    B01Info *info;
    a2 = actor;
    new_var = (B01Info *)(((u8 *)a2->def) + 0x14);
    if (!(a2->def->flags14 & 0x10) || (D_8011AAF1[0] != 0 && D_8011AAF1[1] == 1)) {
        if (state) {
            state->unk64 = func_002119FC(45.0f, 60.0f);
            info = new_var;
            state->unk10C.i = 0;
            state->unk110.i = info->unk8;
        } else {
            info = new_var;
            state->unk64 = func_002119FC(45.0f, 60.0f);
            state->unk10C.i = 0;
            state->unk110.i = info->unk8;
        }
        if (a2->unkB8 == 0x644 || (info->flags & 8)) {
            a2->flags &= ~0x2000;
        }
        func_00243414(a2, state, 0);
        if ((info->flags & 0x10) && D_8011AAF2 != 1) {
            goto clear;
        }
    } else {
clear:
        a2->flags &= ~0x100;
    }
}
