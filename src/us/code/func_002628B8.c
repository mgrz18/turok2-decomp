/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00203B60.c, func_00203B60.
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
/* Turok 2 LevelActor / state: only the fields used here. */
typedef struct B01Actor {
    u8 pad0[0x14];
    void *def;      /* 0x14 */
    u8 pad18[0xBC];
    u32 flags;      /* 0xD4 */
} B01Actor;
typedef struct B01State {
    u8 pad0[0xF8];
    void (*fnF8)(); /* 0xF8 (0x110 in Rage Wars) */
} B01State;
extern s32 D_800F7078[];
void func_0024646C(B01Actor *actor, B01State *state, s32 kind, s32 a, s32 b);
void func_00220260(void *mgr, B01Actor *actor, s32 flag);
void func_0026E110(B01Actor *actor, s32 flags, B01Actor *actor2);
void func_002628B8(B01Actor *actor, B01State *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->def) + 0x14));
    func_0024646C(actor, state, 4, info->unkC.i, info->unk10.i);
    state->fnF8 = 0;
    func_00220260(D_800F7078, actor, 1);
    func_0026E110(actor, 1, actor);
    actor->flags &= ~0x10000;
    if (D_800F7078[0] != 4) {
        actor->flags |= 0x8000000;
    }
}
