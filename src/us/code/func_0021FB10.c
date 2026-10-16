/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_13.c, func_8024CA10_de.
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
struct Vector4f;
typedef struct Vector4f Vector4f;
struct Vector4f;
struct Vector4f {
    float x;
    float y;
    float z;
    float w;
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
extern void func_0021FB10(void *arg0, s32 arg1, Vector4f *arg2);
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
void func_0021FB10(void *arg0, s32 arg1, Vector4f *arg2) {
    char *o = (char *)arg0;
    s16 idx;
    char *rec;
    void *base;
    char *a;
    idx = ((struct ObjRecAt2V1 *) (((ObjectLinks24 *) o)->unk_0 + (arg1 * 4)))->unk2;
    if (idx == -1) {
        f32 scale;
        rec = ((ObjectLinks24 *)(o))->unk_4 + arg1 * 0x14;
        scale = (3.0518509447574615e-05f);
        arg2->x = (f32)(((ObjectState14 *)(rec))->unk_C) * scale;
        arg2->y = (f32)(((ObjectState14 *)(rec))->unk_E) * scale;
        arg2->z = (f32)(((ObjectState14 *)(rec))->unk_10) * scale;
        arg2->w = (f32)(((ObjectState14 *)(rec))->unk_12) * scale;
        return;
    }
    base = func_002017D4(((ObjectLinks24 *)(o))->unk_C, (s32)idx);
    a = (char *)base + (((ObjectLinks24 *)(o))->unk_10) * 4;
    base = (char *)base + (((ObjectLinks24 *)(o))->unk_14) * 4;
    {
        f32 scale;
        scale = ((ObjectLinks24 *)(o))->unk_20;
        arg2->x = ((Vector4f *)(a))->x + scale * (((Vector4f *)(base))->x - ((Vector4f *)(a))->x);
        arg2->y = ((Vector4f *)(a))->y + scale * (((Vector4f *)(base))->y - ((Vector4f *)(a))->y);
        arg2->z = ((Vector4f *)(a))->z + scale * (((Vector4f *)(base))->z - ((Vector4f *)(a))->z);
        arg2->w = ((Vector4f *)(a))->w + scale * (((Vector4f *)(base))->w - ((Vector4f *)(a))->w);
    }
}
