/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_8026826C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Kind;
typedef struct Kind Kind;
extern float D_800A65E0;
struct ParamsRecAt4;
typedef struct ParamsRecAt4 ParamsRecAt4;
struct ObjectRecAt17C;
typedef struct ObjectRecAt17C ObjectRecAt17C;
extern float D_800A65E4;
struct ParamsRecAt4;
struct ParamsRecAt4 {
    s16 unk0;
    s16 value;
    s32 team;
};
struct Kind;
struct Kind {
    s32 value;
    u8 pad4[0xA];
    s8 team;
};
struct Kind;
struct ObjectRecAt17C;
struct ObjectRecAt17C {
    u8 type;
    u8 pad1[0x13];
    struct Kind *kind; /* Turok 2 offsets (0x18/0xD0/0x17C in Rage Wars) */
    u8 pad18[0x80 - 0x18];
    s32 value;
    u8 pad84[0x14C - 0x84];
    u32 mask;
};
extern s32 func_002755B0(s32 arg0, CVector3 arg1, s32 arg4, s32 arg5,
                         f32 arg6);
extern f32 D_800A65D8;
extern s32 D_8011ACA4;
void func_00238EE4(ObjectRecAt17C *arg0, ObjectRecAt17C *arg1, s32 arg2, CVector3 arg3,
                   ParamsRecAt4 arg6) {
    s32 *global = &D_8011ACA4;
    s32 selected;
    f32 scale;
    scale = (&D_800A65D8)[1];
    selected = -1;
    if (global[0] != 0) {
        return;
    }
    switch (arg1->type) {
    case 0:
        if (arg0->kind->team != arg6.team) {
            return;
        }
        selected = arg1->value;
        break;
    case 1:
        if (arg1->kind->value == arg1->type) {
            if (global[-115] & 0x40) {
                scale = D_800A65E0;
            } else if (global[-115] & 0x20) {
                scale = D_800A65E4;
            }
        }
        if ((arg1->mask & (1 << arg6.team)) == 0) {
            return;
        }
        break;
    }
    func_002755B0(arg6.value, arg3, 0, selected, scale);
}
