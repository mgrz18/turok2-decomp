/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D0E4.c, func_0028D0E4.
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
typedef struct Level { /* Turok 2 layout */
    u8 pad0[0x18FAC];
    s32 unk18FAC;       /* palette index (Rage Wars: 0x1B410) */
    s32 unk18FB0;       /* fade state (Rage Wars: 0x1B414) */
    f32 unk18FB4;       /* fade timer (Rage Wars: 0x1B418) */
} Level;
typedef struct { f32 div; u8 r; u8 g; u8 b; u8 pad7; } Pal;
void func_002883E0(u8 *a, u8 *col, s32 c, s32 d, s32 e, s32 f);
extern Pal D_800ACDE8[];
extern s32 D_800C2030;
extern s32 D_800C2034;
extern u8 D_800F6CB0[];
void func_00226024(Level *lv) {
    u8 col[4];
    f32 t;
    Pal *p;
    s32 a = lv->unk18FB0;
    if (a != 0) {
      if (a >= 0) {
       if (a < 3) {
        p = &D_800ACDE8[lv->unk18FAC];
        if (p->div != 0.0f) {
            col[0] = p->r;
            col[1] = p->g;
            col[2] = p->b;
            t = lv->unk18FB4 * 255.0f / p->div;
            col[3] = (u32)t;
            func_002883E0(D_800F6CB0, col, 0, 0, D_800C2030, D_800C2034);
        }
       }
      }
    }
}
