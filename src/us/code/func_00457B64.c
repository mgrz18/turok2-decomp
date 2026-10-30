/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_004070F4.c, func_004070F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;
typedef struct O01Plr2 {
                u8 pad0[0x5DC];
                u8 *unk5DC;
} O01Plr2;
typedef struct O01Sub6 {
               u8 pad0[4];
               s8 unk4;
} O01Sub6;
typedef struct O01Fl6 {
                u8 pad0[0x328];
                u32 unk328;
} O01Fl6;
typedef struct O01Own6 {
               u8 pad0[0xC];
               O01Fl6 *unkC;
               u8 pad10[0xC];
               O01Plr2 *unk1C;
               O01Sub6 *unk20;
} O01Own6;
extern s32 D_80130984;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_800C2024;
extern s32 D_801309BC;
extern s32 D_80130990;
extern s32 D_800C2020;
extern u8 D_80119E2C[];
extern u8 D_004390B0[];
extern u8 D_0043BB8C[];
extern u8 D_0043BF7C[];
extern u8 D_0043B610[];
extern u8 D_0043BB20[];
void func_004263A0(s32 ch);
s32 func_0042647C(s32 ch);
s32 func_004264B4(s32 ch);
s32 func_00426AF8(s32 ch);
void func_0041D948(O01Own6 *o);
void func_00416644(u8 *a, void *b, void *c, void *d, void *e);
void func_00457B64(O01Own6 *own) {
    s32 pad;
    register s32 o1 asm("$8");
    register s32 o2 asm("$18");
    D_800C2024 = 2;
    D_80130984 = 0;
    D_801309A0 = 0;
    o1 = 1;
    D_80130994 = o1;
    D_801309B4 = 0;
    D_801309BC = 0;
    D_80130990 = 0;
    pad = own->unk20->unk4;
    if (D_800C2020 != 0) {
        D_801309BC = o1;
        func_00416644(D_80119E2C, D_004390B0, own->unk1C, own->unk20, 0);
    } else {
        own->unkC->unk328 &= 0xFEFFFFFF;
        func_004263A0(own->unk20->unk4);
        if (func_0042647C(pad) == 0) {
            func_0041D948(own);
        } else {
            o2 = 1;
            D_801309BC = o2;
            if (func_004264B4(pad) != 0) {
                func_00416644(D_80119E2C, D_0043BB8C, own->unk1C, own->unk20, (void *)o2);
            } else if (func_00426AF8(pad) != 0) {
                func_00416644(D_80119E2C, D_0043BF7C, own->unk1C, own->unk20, D_0043B610);
            } else {
                func_00416644(D_80119E2C, D_0043BB20, own->unk1C, own->unk20, D_004390B0);
            }
        }
    }
}
