/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00238EA8.c, func_00238EA8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct ObjInit {
    s32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C;
    union { s32 i; f32 f; } unk20;
    Vec3f v;
    f32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    s32 unk40;
} ObjInit;
extern f32 D_800A9BD4;
extern f32 D_800A9BD8;
extern s32 D_800B6D00[];
void func_00280C28(ObjInit *o) {
    Vec3f *p;
    f32 z;
    o->unk20.i = 0;
    z = o->unk20.f;
    o->unk0 = 1;
    o->unk4 = 0;
    o->unk8 = 0;
    o->unkC = 0;
    o->unk10 = 0;
    o->unk14 = 0;
    o->unk18 = 0;
    o->unk1C = 0;
    p = &o->v;
    p->x = p->y = p->z = z;
    o->unk30 = D_800A9BD4;
    o->unk34 = 0;
    o->unk38 = z;
    o->unk3C = D_800A9BD8;
    o->unk40 = D_800B6D00[0];
}
