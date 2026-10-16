/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_3.c, func_802A25D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
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
union func_8020E674_S1_U8;
typedef union func_8020E674_S1_U8 func_8020E674_S1_U8;
union func_8020E674_S1_U8;
union func_8020E674_S1_U8 {
    f32 v0;
    Vec3 v1;
};
union func_8022E280_S1_U744;
typedef union func_8022E280_S1_U744 func_8022E280_S1_U744;
union func_8022E280_S1_U744;
union func_8022E280_S1_U744 {
    s32 v0;
    f32 v1;
};
struct ObjectLinks54;
struct ObjectLinks54 {
    char pad0[0x8];
    char * unk_8;
    char pad8[0x28 - 0x8 - sizeof(char*)];
    f32 unk_28;
    char pad28[0x3C - 0x28 - sizeof(f32)];
    s32 unk_3C;
    char pad3C[0x40 - 0x3C - sizeof(s32)];
    char * unk_40;
    char pad40[0x4C - 0x40 - sizeof(char*)];
    f32 unk_4C;
    char pad4C[0x50 - 0x4C - sizeof(f32)];
    s32 unk_50;
};
extern float D_800A6440;
struct ObjectLinks54;
typedef struct ObjectLinks54 ObjectLinks54;
struct ObjectStateB0;
struct ObjectStateB0 {
    char pad0[0x10];
    Vec3 unk_10;
    char pad10[0xAC - 0x10 - sizeof(Vec3)];
    f32 unk_AC;
};
struct ObjectLinksB4;
struct ObjectLinksB4 {
    char pad0[0x4];
    char * unk_4;
    char pad4[0x8 - 0x4 - sizeof(char*)];
    f32 unk_8;
    char pad8[0x10 - 0x8 - sizeof(f32)];
    func_8020E674_S1_U8 unk_10;
    char pad10[0x1C - 0x10 - sizeof(func_8020E674_S1_U8)];
    f32 unk_1C;
    char pad1C[0x28 - 0x1C - sizeof(f32)];
    f32 unk_28;
    char pad28[0x68 - 0x28 - sizeof(f32)];
    f32 unk_68;
    char pad68[0xA8 - 0x68 - sizeof(f32)];
    f32 unk_A8;
    char padA8[0xAC - 0xA8 - sizeof(f32)];
    func_8022E280_S1_U744 unk_AC;
    char padAC[0xB0 - 0xAC - sizeof(func_8022E280_S1_U744)];
    s32 unk_B0;
};
struct ObjectStateB0;
typedef struct ObjectStateB0 ObjectStateB0;
struct ObjectState20;
struct ObjectState20 {
    unsigned char padding_0[8];
    f32 unk_8;
    unsigned char padding_C[16];
    u8 unk_1C;
};
struct ObjectLinksB4;
typedef struct ObjectLinksB4 ObjectLinksB4;
struct ObjectState20;
typedef struct ObjectState20 ObjectState20;
extern void *func_00235FF4(void *arg0, s32 arg1);
extern void func_002101A0(char *object, f32 *output);
extern void func_002101BC(void *arg0, f32 *arg1);
extern void func_0020D274(void *arg0, f32 *arg1);
extern void func_0020EF2C(Vec3 *arg0, Vec3 *arg1, Vec3 *arg2);
extern f32 func_00298470(f32);
void *func_002333C0(void *arg0, void *arg1, void *arg2) {
    Vec3 sp10;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 var_f1;
    void *temp_s1;
    char *var_s0;
    char *temp_s2;
    temp_s2 = func_00235FF4(arg0, (s32)arg1);
    var_s0 = temp_s2;
    if (temp_s2 != 0) {
        ((ObjectLinks54 *)(arg1))->unk_50 = 0;
        ((ObjectLinksB4 *)(var_s0))->unk_8 = ((struct ObjectState20 *) ((ObjectLinks54 *) arg1)->unk_8)->unk_8;
        ((ObjectLinksB4 *)(var_s0))->unk_B0 = 0;
        func_002101A0(arg2, &((ObjectLinksB4 *)(var_s0))->unk_10.v0);
        if (((ObjectLinks54 *)(arg1))->unk_3C & 4) {
            func_002101BC(arg2, &((ObjectLinksB4 *)(var_s0))->unk_1C);
        } else {
            func_0020D274(arg2, &((ObjectLinksB4 *)(var_s0))->unk_28);
            func_0020D274(arg2, &((ObjectLinksB4 *)(var_s0))->unk_68);
        }
        temp_s1 = *(void **)var_s0;
        ((ObjectLinksB4 *)(var_s0))->unk_AC.v0 = 0;
        ((ObjectLinksB4 *)(var_s0))->unk_A8 = 0.0f;
        if (temp_s1 != 0) {
            func_0020EF2C(&sp10, &((ObjectLinksB4 *)(var_s0))->unk_10.v1, &((ObjectStateB0 *)(temp_s1))->unk_10);
            temp_f0 = func_00298470((sp10.x * sp10.x) + (sp10.y * sp10.y) + (sp10.z * sp10.z));
            ((ObjectStateB0 *)(temp_s1))->unk_AC = temp_f0;
            ((ObjectLinks54 *)(arg1))->unk_4C += temp_f0;
        }
        if (((struct ObjectState20 *) ((ObjectLinks54 *) arg1)->unk_8)->unk_1C == 0) {
            if (((ObjectLinks54 *)(arg1))->unk_4C > 0.0f) {
                var_f1 = 0.0f;
                var_s0 = ((ObjectLinks54 *)(arg1))->unk_40;
                if (var_s0 != 0) {
                    do {
                        ((ObjectLinksB4 *)(var_s0))->unk_A8 = var_f1 / ((ObjectLinks54 *)(arg1))->unk_4C;
                        temp_f0_2 = ((ObjectLinksB4 *)(var_s0))->unk_AC.v1;
                        var_s0 = ((ObjectLinksB4 *)(var_s0))->unk_4;
                        var_f1 += temp_f0_2;
                    } while (var_s0 != 0);
                }
            }
        } else {
            if (temp_s1 != 0) {
                ((ObjectLinks54 *)(arg1))->unk_28 += ((ObjectStateB0 *)(temp_s1))->unk_AC * D_800A6440;
            }
            ((ObjectLinksB4 *)(var_s0))->unk_A8 = ((ObjectLinks54 *)(arg1))->unk_28;
        }
    }
    return temp_s2;
}
