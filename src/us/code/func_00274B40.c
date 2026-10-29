/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/AUDIO_2/8025D470.CPP, func_8025D450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
extern void func_00290E10(s32 arg0, s32 arg1);
extern void func_00290E90(s32 arg0, s32 arg1);
extern void func_00290EC0(s32 arg0, s16 arg1);
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct Track;
typedef struct Track Track;
struct Resource;
typedef struct Resource Resource;
struct ContextRecAt2BB8;
typedef struct ContextRecAt2BB8 ContextRecAt2BB8;
struct Resource;
struct Resource {
    s32 unk0;
    u16 unk4;
};
struct ContextRecAt2BB8;
struct ContextRecAt2BB8 {
    char pad[0x2B50];
    s32 unk2B50;
    char p54[12];
    s32 unk2B60;
    s32 unk2B64;
    char p68[0x3C];
    f32 unk2BA4;
    char pa8[16];
    s32 unk2BB8;
};
struct ContextRecAt2BB8;
struct IntPair;
struct Track;
struct Track {
    struct ContextRecAt2BB8 *unk0;
    s32 unk4;
    struct IntPair *unk8;
    void *unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    s32 unk38;
    f32 unk3C;
    f32 unk40;
};
extern s32 D_800B2A70;
extern s32 D_800B2A74;
extern f32 D_800A81A0;
extern f32 D_800A81A8;
extern f32 D_800A81B0;
extern f32 D_800A81B4;
extern f32 D_800A81B8;
extern f32 D_800A81BC;
extern f32 D_800A81C0;
extern void func_00290DE0(s32);
extern int func_0028FA00(void *object);
struct Track;
extern void func_00290E40(s32 arg0, s32 arg1, s8 arg2);
void *func_002017D4(void *table, s32 index);
extern s32 func_00266D24(s32,s32,s32);
extern f32 func_0026D210(u16);
extern void func_0028F518(s32, void *);
static __inline__ f32 fade_rate(Track *track,f32 time,f32 rate) {track->unk30=time;return (f32)track->unk20/(time*rate);}
void func_00274B40(Track *arg0) {
    f32 temp_f0;
    f32 temp_f20;
    f32 var_f20;
    f32 var_f2;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_s0;
    s32 temp_v0;
    s32 temp_v0_3;
    s32 temp_v1;
    s32 var_s0;
    s32 var_v0;
    void *temp_s0_2;
    ContextRecAt2BB8 *temp_v0_2;
    struct Resource *temp_v0_4;
    ContextRecAt2BB8 *temp_v0_5;
    void *var_v0_2;
    temp_v1 = arg0->unk18;
    switch (temp_v1) {
    case 0x20:
        if (arg0->unk1C & 4) {
            arg0->unk1C = 0;
            D_800B2A70 = -1;
            if (arg0->unk28 >= 0) {
                arg0->unk18 = 0x40;
                return;
            }
            arg0->unk18 = 0x10;
            return;
        }
        return;
    case 0x100:
        if (arg0->unk1C & 8) {
            arg0->unk18 = 0x80;
            arg0->unk1C = 0;
            return;
        }
        temp_v0 = D_800B2A74 + 1;
        D_800B2A74 = temp_v0;
        if (temp_v0 >= 0xA) {
            D_800B2A70 = -1;
            arg0->unk18 = 0x40;
            return;
        }
        break;
    case 0x40:
        temp_a2 = arg0->unk28;
        var_v0 = 0x10;
        if (temp_a2 != -1) {
            temp_v0_2 = arg0->unk0;
            temp_v0_3 = func_00266D24(temp_v0_2->unk2B60, temp_v0_2->unk2B64, temp_a2);
            temp_s0 = temp_v0_3;
            if (temp_s0 != -1) {
                temp_s0 *= 2;
                temp_s0_2 = func_002017D4(arg0->unk0->unk2B50, temp_s0 | 1);
                temp_v0_4 = func_002017D4(arg0->unk0->unk2B50, temp_s0);
                arg0->unk24 = (s32) temp_v0_4->unk0;
                temp_f0 = func_0026D210(temp_v0_4->unk4);
                arg0->unk30 = temp_f0;
                if (temp_f0 <= 0.0f) {
                    arg0->unk30 = D_800A81A0;
                }
                var_v0_2 = temp_s0_2;
            } else {
                var_v0_2 = ((void *)0);
            }
            arg0->unkC = var_v0_2;
            if (var_v0_2 == ((void *)0)) {
                arg0->unk18 = 0x100;
                D_800B2A74 = 0;
                arg0->unk1C = 0;
                return;
            }
            arg0->unk18 = 0x80;
            arg0->unk4 = 1;
        case 0x80:
            var_s0 = 0;
            func_0028F518(arg0->unk10, arg0->unkC);
            func_00290E90(arg0->unk14, arg0->unk10);
            func_00290E10(arg0->unk14, arg0->unk8->field_4);
            do {
                func_00290E40(arg0->unk14, var_s0 & 0xFF, 0);
                var_s0 += 1;
            } while (var_s0 < 0x14);
            temp_f20 = (f32) arg0->unk24 * (arg0->unk0->unk2BA4 * *(&D_800A81A0+1));
            func_00290EC0(arg0->unk14, (s16) (s32) (temp_f20 * D_800A81A8));
            arg0->unk2C = temp_f20;
            func_00290DE0(arg0->unk14);
            arg0->unk1C = 0;
            arg0->unk18 = 0x10;
            D_800B2A70 = arg0->unk28;
            return;
        }
        goto block_44;
    case 0x10:
    default:
        if ((D_800B2A70 > 0) && (func_0028FA00((void *)arg0->unk14) == 0)) {
            D_800B2A70 = 0;
            arg0->unk28 = 0;
        } else {
            if (arg0->unk38 != 0) {
                var_f20 = (f32) arg0->unk24;
                var_f20 *= arg0->unk0->unk2BA4 * *(&D_800A81A8+1);
                var_f20 *= arg0->unk3C;
                goto block_37;
            }
            temp_v0_5 = arg0->unk0;
            var_f20 = (f32) arg0->unk24 * (temp_v0_5->unk2BA4 * D_800A81B0);
            if (temp_v0_5->unk2BB8 != 0) {
                var_f20 *= D_800A81B4;
            }
            if (var_f20 != arg0->unk2C) {
block_37:
                func_00290EC0(arg0->unk14, (s16) (s32) (var_f20 * D_800A81B8));
                arg0->unk2C = var_f20;
            }
        }
        if (arg0->unk28 != D_800B2A70) {
            if (D_800B2A70 > 0) {
                var_f2 = arg0->unk30;
                arg0->unk1C = 2;
                arg0->unk18 = 0x20;
                arg0->unk20 = (s32) (arg0->unk2C * D_800A81BC);
                if (!(var_f2 >= D_800A81C0)) {
                    var_f2 = D_800A81C0;
                }
                arg0->unk34 = fade_rate(arg0,var_f2,*(&D_800A81C0+1));
                return;
            }
            var_v0 = 0x40;
            arg0->unk1C = 0;
block_44:
            arg0->unk18 = var_v0;
        }
        break;
    }
}
