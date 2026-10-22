/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0020612C.c, func_0020612C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef union B01IF { s32 i; f32 f; } B01IF;
typedef struct B01Info {
    u32 flags;
    s32 unk4;
    s16 unk8;
    s16 unkA;
    u8 padC[0x8];
    s16 unk14;
    s16 unk16;
} B01Info;
/* Turok 2 actor: def at 0x14, flags at 0xD4 (Rage Wars: 0x18 / 0x100). */
typedef struct B01Actor {
    u8 pad0[0x14];
    u8 *def;            /* 0x14 */
    u8 pad18[0xBC];
    u32 flags;          /* 0xD4 */
} B01Actor;
typedef struct B01State {
    u8 pad0[0x40];
    f32 unk40;          /* 0x40 */
    u8 pad44[0x20];
    f32 unk64;          /* 0x64 */
    u8 pad68[0xA4];
    B01IF unk10C;       /* 0x10C */
    B01IF unk110;       /* 0x110 */
} B01State;
void func_00243414(B01Actor *actor, B01State *state, s32 event);
void func_00263640(B01Actor *actor, B01State *state) {
    B01Info *info = ((B01Info *)((u8 *)(actor->def) + 0x14));
    if (info->unk4 == 0) {
        if (info->unk14 == -1) {
            state->unk40 = 0.0f;
            return;
        }
        if (info->unk16 == -1) {
            state->unk40 = 0.0f;
            return;
        }
    }
    if (state->unk10C.i == info->unkA) {
        state->unk40 = 0.0f;
        return;
    }
    if ((info->flags & 1) || state->unk110.i > 0) {
        if ((info->flags & 2) && (actor->flags & 0x200)) {
            state->unk40 = 0.0f;
            return;
        }
        if (state->unk40 >= state->unk64) {
            func_00243414(actor, state, 1);
        }
    }
}
