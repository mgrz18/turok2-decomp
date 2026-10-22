/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8043D54C_4.CPP, func_8043DE50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct func_80250BD4_S1;
struct func_80250BD4_S1 {
    char pad0[0x20];
    s32 * unk20;
};
struct func_80250BD4_S1;
typedef struct func_80250BD4_S1 func_80250BD4_S1;
struct SharedRecAt224;
struct SharedRecAt224 {
    char pad0[0x224];
    s32 unk224;
};
struct SharedRecAtA8;
typedef struct SharedRecAtA8 SharedRecAtA8;
struct Shared_func_8043DEDC_S3;
typedef struct Shared_func_8043DEDC_S3 Shared_func_8043DEDC_S3;
extern void func_00417ACC(func_80250BD4_S1 *arg0);
struct SharedRecAt224;
typedef struct SharedRecAt224 SharedRecAt224;
struct SharedRecAtA8;
struct SharedRecAtA8 {
    char pad0[0xA8];
    s8 unkA8;
};
struct Shared_func_8043DEDC_S3;
struct Shared_func_8043DEDC_S3 {
    char pad0[0x28];
    s8 unk28;
    char pad29[0x6];
    s8 unk2F;
};
void func_00275A74(void);
unsigned int func_002855E8(void *);
extern s32 D_800F5EC0;
extern s32 D_8011AB18;
extern struct SharedRecAtA8 D_8011AB98[];
void func_00417ACC(func_80250BD4_S1 *arg0) {
    Shared_func_8043DEDC_S3 *settings;
    s8 *status;
    s32 *profile;
    s32 on;
    s32 i;
    i = 3;
    status = (s8 *)&D_8011AB98[0];
    for (; i >= 0; i--) {
        ((SharedRecAtA8 *)status)->unkA8 = 0;
        status -= 0x40;
    }
    i = 0;
    settings = (Shared_func_8043DEDC_S3 *)&D_8011AB18;
    on = 1;
    for (profile = &D_800F5EC0; i < 4; i++, profile = &((SharedRecAt224 *)profile)->unk224) {
        if (func_002855E8(profile) != 0) {
            settings->unk28 = on;
            settings->unk2F = i;
            arg0->unk20 = profile;
            break;
        }
    }
    func_00275A74();
}
