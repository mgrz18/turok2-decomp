/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025E590_de_9.c, func_802604AC_de.
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
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
struct Triple;
struct ObjRecAt2V1;
typedef struct ObjRecAt2V1 ObjRecAt2V1;
struct Rec_func_8024C92C_de;
typedef struct Rec_func_8024C92C_de Rec_func_8024C92C_de;
struct Rec_func_8024C92C_de;
struct Rec_func_8024C92C_de {
    s32 x;
    s32 y;
    s32 z;
    s32 pad0;
    s32 pad1;
};
struct ObjectLinks24_2;
typedef struct ObjectLinks24_2 ObjectLinks24_2;
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
struct Triple;
struct Triple;
struct Triple;
struct Triple;
struct Rec_func_8024C92C_de;
struct Triple;
struct Rec_func_8024C92C_de;
struct Triple;
struct Triple;
struct Triple;
extern void func_0020EF8C(Vec3 *arg0, s32 arg1, void *arg2, void *arg3);
void func_0021794C(void *arg0, s32 arg1, void *arg2) {
    extern void *func_002017D4(void *arg0, s32 arg1);
    ObjectLinks24_2 *o = arg0;
    ObjRecAt2V1 *indices = (void *)o->unk_0;
    s16 idx;
    Rec_func_8024C92C_de *recs;
    void *base;
    Triple *out = arg2;
    idx = indices[arg1].translation;
    if (idx == -1) {
        recs = (void *)o->unk_4;
        __builtin_memcpy(out, &recs[arg1], sizeof(*out));
        return;
    }
    base = func_002017D4(o->unk_8, (s32) idx);
    func_0020EF8C(arg2, o->unk_20,
                  (char *)base + (o->unk_18) * 4,
                  (char *)base + (o->unk_1C) * 4);
}
