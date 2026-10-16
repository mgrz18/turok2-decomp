/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_14.c, func_8024CB4C_de.
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
struct ObjectLinks24_2;
typedef struct ObjectLinks24_2 ObjectLinks24_2;
struct ObjectState14;
typedef struct ObjectState14 ObjectState14;
struct ObjRecAt2V1;
struct ObjRecAt2V1 {
    s16 translation;
    short unk2;
};
struct ObjectLinks24_2;
struct ObjectLinks24_2 {
    char * unk_0;
    char pad0[0x4 - 0x0 - sizeof(char*)];
    char * unk_4;
    void * unk_8;
    void * unk_C;
    char padC[0x10 - 0xC - sizeof(void*)];
    s32 unk_10;
    char pad10[0x14 - 0x10 - sizeof(s32)];
    s32 unk_14;
    s32 unk_18;
    s32 unk_1C;
    s32 unk_20;
};
extern float D_800A5D2C;
extern char *func_002017D4(s32 *, s32);
extern void func_0020DAA4(void *arg0, s32 arg1, void *arg2, void *arg3);
void func_0021FC4C(ObjectLinks24_2 *arg0, s32 arg1, Vector4f *arg2) {
    ObjectLinks24_2 *o = arg0;
    ObjRecAt2V1 *indices;
    s16 idx;
    ObjectState14 *rec;
    void *base;
    f32 scale;
    indices = (void *)o->unk_0;
    idx = indices[arg1].unk2;
    if (idx == -1) {
        rec = (void *)(o->unk_4 + arg1 * 0x14);
        scale = D_800A5D2C;
        arg2->x = (f32)rec->unk_C * scale;
        arg2->y = (f32)rec->unk_E * scale;
        arg2->z = (f32)rec->unk_10 * scale;
        arg2->w = (f32)rec->unk_12 * scale;
        return;
    }
    base = func_002017D4(o->unk_C, (s32)idx);
    func_0020DAA4(arg2, o->unk_20,
                    (char *)base + o->unk_10 * 4,
                    (char *)base + o->unk_14 * 4);
}
