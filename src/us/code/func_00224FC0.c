/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C1B0.c, func_0028C1B0.
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
typedef struct Ent12 { s32 **tblp; s32 b; s32 c; } Ent12;
typedef struct Obj2 { u8 pad0[8]; void *unk8; f32 unkC; } Obj2;
typedef struct Rec14 {
                 u8 pad0[0xE];
                 u8 flags;
                 u8 key;
                 u8 pad10[0x4];
} Rec14;
typedef struct Entity {
                 u8 pad0[0x4];
                 s16 unk4s;
                 u8 pad6[0x2];
                 Vec3f pos;
                 s32 unk14;
                 ActorDef *def;
                 Vec3f vel;
                 u8 pad28[0xBC];
                 u16 unkE4;
                 u8 padE6[0x1A];
                 u32 flags;
                 u8 pad104[0x70];
                 s32 unk174;
                 u8 pad178[0x4];
                 f32 box[6];
                 u8 pad194[0x8];
                 u32 flags19C;
                 u8 pad1A0[0x4];
                 s8 unk1A4;
                 u8 pad1A5[0x23];
                 f32 unk1C8;
                 u8 pad1CC[0x72];
                 s8 unk23E;
                 u8 pad23F[0xA9];
} Entity;
typedef struct Level { /* Turok 2 layout: record tables sit 0x58 earlier than Rage Wars */
    u8 pad0[0x116C];
    s32 cnt116C;        /* Rage Wars: 0x11C4 */
    u8 pad1170[4];
    s32 unk1174;        /* Rage Wars: 0x11CC */
    Rec14 *recA;        /* 0x1178 */
    Rec14 *recB;        /* 0x117C */
} Level;
s32 func_00266D8C(s32 a, s32 b, s32 c, s32 *lo, s32 *hi);
void func_00224FC0(Level *lv, u16 *idp, Rec14 **outRecs, s32 *outCount) {
    s32 hi;
    s32 lo;
    s32 r; s32 a; s32 b; s32 id;
    id = *idp; a = lv->unk1174; b = lv->cnt116C;
    r = func_00266D8C(a, b, id, &lo, &hi);
    if (r != 0) {
        *outRecs = lv->recB + lo;
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}
