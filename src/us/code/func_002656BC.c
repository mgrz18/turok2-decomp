/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204D2C.c, func_00204D2C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
/* Turok 2 LevelActor: only the fields used here. */
typedef struct B01Actor {
    u8 pad0[0x4];
    Vec3f pos;      /* 0x4 */
    u8 pad10[0xC4];
    u32 flags;      /* 0xD4 */
} B01Actor;
typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;
extern s32 D_800F7078[];
extern u8 D_801102B8[];
void func_00237EE4(B01Actor *a, B01Actor *b, s32 kind, Vec3f pos, B01Pair extra);
void func_00220260(void *mgr, B01Actor *actor, s32 flag);
void func_0026E110(B01Actor *actor, s32 flags, B01Actor *actor2);
void func_002365F8(void *p, B01Actor *actor);
void func_002656BC(B01Actor *actor) {
    B01Pair extra;
    extra.a = 0;
    func_00220260(D_800F7078, actor, 0);
    func_0026E110(actor, 0x8000, actor);
    func_002365F8(D_801102B8, actor);
    if (D_800F7078[0] != 4) {
        func_00237EE4(actor, actor, 7, actor->pos, extra);
        actor->flags |= 0x8000000;
    }
}
