/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E77C.c, func_0022E77C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct ActorInfo {
               u32 flags;
               s32 unk4;
               u8 pad8[4];
               s16 unkC;
               u8 padE[6];
               f32 unk14;
               u8 pad18[0xC];
               u32 flags24;
               u8 pad28[0x28];
               f32 unk50;
} ActorInfo;
typedef struct ActorDef {
               s32 type;
               u8 pad4[0x10];
               ActorInfo info;
} ActorDef;
typedef struct ActorStats {
               u8 pad0[0x80];
               s8 unk80;
               u8 pad81[0xD];
               u8 unk8E;
               u8 unk8F;
               u8 pad90[2];
               u8 unk92;
               u8 unk93;
               u8 unk94;
} ActorStats;
typedef struct Brain {
                u8 pad0[0xC];
                s32 unkC;
                u8 pad10[0x210];
                s32 unk220;
                u8 pad224[0x18];
                s32 unk23C;
                s32 unk240;
                u8 pad244[0x94];
                s32 unk2D8;
                s32 unk2DC;
                u8 pad2E0[0xC];
                s32 unk2EC;
                u8 pad2F0[0xC];
                s32 unk2FC;
} Brain;
typedef struct Actor Actor;
struct Actor {
                 u8 pad0[0x18];
                 ActorDef *def;
                 u8 pad1C[0x1C];
                 u32 flags38;
                 u8 pad3C[0xA8];
                 u16 unkE4;
                 u8 padE6[0x1A];
                 u32 flags;
                 u8 pad104[0xD4];
                 Actor *owner;
                 u8 pad1DC[0x110];
                 Actor *next2EC;
                 u8 pad2F0[0x2E4];
                 s32 unk5D4;
                 ActorStats *stats;
                 u8 pad5DC[8];
                 s32 unk5E4;
                 u8 pad5E8[0x88];
                 f32 unk670;
                 u8 pad674[0x1CC];
                 f32 unk840;
                 u8 pad844[4];
                 s32 unk848;
                 u8 pad84C[0x974];
                 s32 unk11C0;
                 u8 pad11C4[0x28C];
                 s32 unk1450;
                 Brain *brain;
                 u8 pad1458[0x280];
                 u16 unk16D8;
                 u8 pad16DA[6];
                 Actor *next;
};
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
extern u8 D_8010FF34[];
void *func_00228AEC(void *tbl, s32 id);
void func_0021F00C(Vec3f *out, Actor *actor, Vec3f dir);
void func_0020EEF8(Vec3f *out, Vec3f *a, void *b);
f32 func_002671B4(Actor *actor);
void func_00256ED0(Actor *actor, void **slot, s32 id, f32 z)
{
    void *p;
    Vec3f dir;
    Vec3f out;
    p = *slot;
    if (p == ((void *)0)) {
        p = func_00228AEC(D_8010FF34, id);
        if (p == ((void *)0)) {
            return;
        }
    }
    *slot = p;
    dir.x = 0;
    dir.y = 0;
    dir.z = z;
    func_0021F00C(&out, actor, dir);
    func_0020EEF8(&out, &out, (u8 *)actor + 4);
    (*(s16 *)((u8 *)(p) + (0x16))) = 1;
    (*(s16 *)((u8 *)(p) + (0x10))) = out.x;
    (*(s16 *)((u8 *)(p) + (0x12))) = out.y + func_002671B4(actor) * 0.5f;
    (*(s16 *)((u8 *)(p) + (0x14))) = out.z;
    (*(f32 *)((u8 *)(p) + (0xC))) = 1.0f;
}
