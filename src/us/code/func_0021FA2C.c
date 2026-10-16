/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_12.c, func_8024C92C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct ObjRecAt2V1;
typedef struct ObjRecAt2V1 ObjRecAt2V1;
struct ObjectState14;
struct ObjectState14 {
    Vec3 position;
    s16 unk_C;
    char padC[0xE - 0xC - sizeof(s16)];
    s16 unk_E;
    char padE[0x10 - 0xE - sizeof(s16)];
    s16 unk_10;
    char pad10[0x12 - 0x10 - sizeof(s16)];
    s16 unk_12;
};
struct ObjectState14;
typedef struct ObjectState14 ObjectState14;
struct ObjRecAt2V1;
struct ObjRecAt2V1 {
    s16 translation;
    short unk2;
};
struct ObjectLinks24;
typedef struct ObjectLinks24 ObjectLinks24;
extern void func_0021FA2C(void *arg0, s32 arg1, void *arg2);
struct ObjectLinks24;
struct ObjectLinks24 {
    char * unk_0;
    char pad0[0x4 - 0x0 - sizeof(char*)];
    char * unk_4;
    void *translationFrames;
    void * unk_C;
    char padC[0x10 - 0xC - sizeof(void*)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    s32 translationFirst;
    s32 translationNext;
    f32 unk_20;
};
extern char *func_002017D4(s32 *, s32);
void func_0021FA2C(void *arg0, s32 arg1, void *arg2) {
    ObjectLinks24 *o = arg0;
    Vec3 *out = arg2;
    ObjRecAt2V1 *indices = (void *)o->unk_0;
    s16 idx;
    ObjectState14 *recs;
    f32 *base;
    f32 *a;
    f32 scale;
    idx = indices[arg1].translation;
    if (idx == -1) {
        recs = (void *)o->unk_4;
        *out = recs[arg1].position;
        return;
    }
    base = func_002017D4(o->translationFrames, (s32) idx);
    a = base + o->translationFirst;
    base = base + o->translationNext;
    scale = o->unk_20;
    out->x = a[0] + scale * (base[0] - a[0]);
    out->y = a[1] + scale * (base[1] - a[1]);
    out->z = a[2] + scale * (base[2] - a[2]);
}
