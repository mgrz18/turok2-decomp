/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_4.c, func_80285180_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
struct Access_u8_3;
struct Access_u8_3 {
    char pad[0x3];
    u8 field;
};
struct Access_u8_3;
typedef struct Access_u8_3 Access_u8_3;
struct ObjRecAt12;
typedef struct ObjRecAt12 ObjRecAt12;
struct ObjRecAt4V2;
typedef struct ObjRecAt4V2 ObjRecAt4V2;
struct ObjRecAt4V2;
struct ObjRecAt4V2 {
    void * unk0;
    s32 unk4;
};
struct ObjRecAt12;
struct ObjRecAt12 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    char pad8[0x6];
    s8 unk12;
};
void func_002057B0(s32, void **, void *);
void * func_00205D7C(s32, void *, s32);
void * func_00205EFC(s32, s32, s32);
extern void *func_0029E460(void *destination, const void *source, int count);
s32 func_00200C40();
s32 func_002010C0();
s32 func_00200B00(void **arg0, s32 arg1) {
    s32 var_s0;
    u8 temp_v1;
    void **var_s1;
    ObjRecAt4V2 *temp_a2;
    ObjRecAt12 *temp_s0;
    ObjRecAt4V2 *temp_v0;
    temp_a2 = *arg0;
    temp_s0 = temp_a2->unk0;
    if ((temp_s0->unk0 & ~0xFF) == 0x524E4300) {
        if (arg1 != 0) {
            var_s1 = func_00205EFC(0, (s32) arg0, temp_s0->unk4);
        } else {
            var_s1 = func_00205D7C(0, arg0, temp_s0->unk4);
        }
        if (var_s1 != ((void *)0)) {
            temp_v1 = (((Access_u8_3 *)(temp_s0))->field);
            switch (temp_v1) {
            case 1:
                func_00200C40(&temp_s0->unk12, *var_s1, temp_s0->unk8, temp_s0->unk4);
                break;
            case 2:
                func_002010C0(temp_s0, *var_s1, temp_s0->unk8, temp_s0->unk4);
                break;
            default:
                break;
            }
            var_s0 = 1;
        } else {
            var_s0 = 0;
        }
    } else if (arg1 == 0) {
        var_s1 = func_00205D7C(0, arg0, temp_a2->unk4);
        if (var_s1 != ((void *)0)) {
            temp_v0 = *arg0;
            var_s0 = 1;
            func_0029E460(*var_s1, temp_v0->unk0, temp_v0->unk4);
        } else {
            var_s0 = 0;
        }
    } else {
        var_s0 = 1;
        goto done;
    }
    func_002057B0(0, arg0, var_s1);
done:
    return var_s0;
}
