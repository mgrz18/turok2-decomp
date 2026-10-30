/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00407748.c, func_00407748.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
void *func_00266C5C(void *s, int c, u32 n);
typedef struct K7Sub { u8 pad0[4]; s8 b4; } K7Sub;
typedef struct K7Obj {
    s16 state;
    u8 pad2[0x12];
    s32 f14;
    u8 pad18[4];
    s32 f1C;
    K7Sub *f20;
} K7Obj;
typedef struct K7Blk { s32 w[6]; } K7Blk;
typedef struct K7Mem { s32 addr; } K7Mem;
extern s32 D_800F70A8;
extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_801309A0;
extern s32 D_80130994;
extern s32 D_801309B4;
extern s32 D_801309C4;
extern s32 D_80130988;
extern K7Blk D_80130968;
extern K7Blk D_80130948;
extern u8 D_800B45C0[];
extern u8 D_800B45CC[];
extern u8 D_800B48D4[];
extern u8 D_800B4B1C[];
extern s32 D_800C2010;
extern s32 D_800C2014;
extern s32 D_800C2018;
extern s32 D_800C201C;
extern u8 D_800C13F0[];
extern u8 D_80119E2C[];
extern u8 D_0043B994[];
extern u8 D_0043B808[];
extern u8 D_0043B9B8[];
extern u8 D_0043B970[];
extern u8 D_0043B7E4[];
extern u8 D_0043C150[];
extern u8 D_0043B610[];
extern u8 D_0043B5C8[];
s32 func_0041DCB0();
s32 func_00425D94();
s32 func_004264D0();
s32 func_00426D98();
s32 func_00426D5C();
void func_0041D948();
s32 func_00426C74();
void func_00426D24();
void func_002053A8();
void func_002052D8();
K7Mem *func_00204EDC();
void *func_00266C5C();
void func_00416644();
s32 func_00426CAC();
extern u8 D_800B45D8[];
extern u8 D_800B45D4[];
/* Turok 2 inlines this game/company-code check (Rage Wars calls it) */
static inline s32 k7_chk(u8 *gc, u8 *cc) {
    if (func_00426CAC(gc) == 4 && func_00426CAC(cc) == 2) {
        if (func_00426C74(gc, D_800B45D8) == 0 && func_00426C74(cc, D_800B45D4) == 0) return 1;
    }
    return 0;
}
s32 func_0045824C(s32 *arg0, K7Obj *arg1) {
    u8 sp20[8];
    u8 sp28[8];
    u8 sp30[8];
    u8 sp38[16];
    s32 sp48;
    s32 sp4C;
    s32 s0, s1, s2, s3, s4, s5, hs, s7, v1, v0;
    K7Mem *m;
    u8 *d;
    s32 i, ch;
    u8 *a1p;
    s2 = 0;
    s5 = *arg0 - 3;
    s7 = D_800F70A8 + 0x688;
    if (D_80130990 != 0) {
        s4 = D_800C2028;
    } else {
        s4 = arg1->f20->b4;
    }
    if (func_0041DCB0(arg1, s4, 0) != 0) {
        D_801309BC = 1;
        return 1;
    }
    if (D_801309A0 == 0) goto L00407BBC;
    s1 = func_00425D94(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30);
    __asm__("" : : "r"(s1));
    if (s1 == 0 && sp48 == 0) {
        arg1->state = 0x14;
        func_0041D948(arg1);
        return 0;
    }
    v1 = k7_chk(sp30, sp28);
    if (v1 != 0) {
        s3 = func_00426C74(sp38, D_800B45C0) == 0;
        s0 = func_00426C74(sp38, D_800B45CC) == 0;
    } else {
        s3 = 0;
        s0 = 0;
    }
    if (s1 != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    __asm__("" : : "r"(s3));
    if (s3 == 0) goto L004079A0;
    if (func_004264D0(s4, s5, &D_80130968, 0x18) != 0) goto L0040799B;
    if (func_00426D98(&D_80130968, 0x14, 4) != D_80130968.w[5]) goto L0040799B;
    s3 = 0;
    D_80130948 = D_80130968;
    D_801309C4 = s5;
    func_00416644(D_80119E2C, D_0043B994, arg1->f1C, arg1->f20, arg1->f14);
    return 1;
L0040799B:
    s3 = 0;
L0040799C:
    s2 = 1;
L004079A0:
    if (s0 == 0) goto L00407AD0;
    if (s2 != 0) goto L00407AD0;
    s0 = sp4C;
    if (D_800C2014 != 0) {
        func_002053A8(0);
        func_002052D8(0, D_800C2014);
    }
    if (s0 == 0) {
        s0 = D_800F70A8 + 0x688;
    } else {
        s0 = s0 << 8;
    }
    m = func_00204EDC(0, s0, 0x33, D_800C13F0);
    D_800C2014 = (s32)m;
    D_800C201C = m->addr;
    func_00266C5C((void *)D_800C201C, 0, s0);
    if (func_004264D0(s4, s5, D_800C201C, sp4C << 8) != 0) goto L00407ACC;
    hs = *(s32 *)D_800C201C;
    if (hs != s7) goto L00407ACC;
    if (func_00426D98(D_800C201C, hs - 4, 0xB) != *(s32 *)(*(s32 *)D_800C201C + D_800C201C - 4)) goto L00407ACC;
    s0 = 0;
    if (D_800F70A8 != *(s32 *)(D_800C201C + 4)) { s2 = 1; goto L00407AD0; }
    D_801309C4 = s5;
    func_00416644(D_80119E2C, D_0043B808, arg1->f1C, arg1->f20, arg1->f14);
    return 1;
L00407ACC:
    s0 = 0;
    s2 = 1;
L00407AD0:
    if (s3 != 0) goto L00407EE8;
    if (s0 != 0) goto L00407EE8;
    ch = 0;
    D_801309C4 = s5;
    d = D_800B4B1C;
    if (func_00426D5C(sp38, 0x10) != 0) {
        func_00426D24(sp38, D_800B48D4);
    }
    i = 1;
    v1 = 0;
    do {
        if (i != 0) ch = sp38[v1];
        if (ch == 0) { i = 0; ch = 0x20; }
        *d = ch;
        v1 += 1;
        d += 1;
    } while (v1 < 0x10);
    i = 1;
    d += i;
    v1 = 0;
    do {
        if (i != 0) ch = sp20[v1];
        if (ch == 0) { i = 0; ch = 0x20; }
        *d = ch;
        v1 += 1;
        d += 1;
    } while (v1 < 4);
    func_00416644(D_80119E2C, D_0043B9B8, arg1->f1C, arg1->f20, arg1->f14);
    return 1;
L00407BBC:
    if (D_80130994 == 0) goto L00407EE8;
    if (D_801309B4 == 0) goto L00407CDC;
    if (s2 != 0) goto L00407CDC;
    if (func_00425D94(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30) != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    if (func_00426C74(sp38, D_800B45C0) != 0) goto L00407EE8;
    if (func_004264D0(s4, s5, &D_80130968, 0x18) != 0) goto L00407CD0;
    if (func_00426D98(&D_80130968, 0x14, 4) != D_80130968.w[5]) goto L00407CD0;
    D_80130948 = D_80130968;
    func_00416644(D_80119E2C, D_0043B970, arg1->f1C, arg1->f20, 0);
    return 1;
L00407CD0:
    s2 = 1;
    goto L00407EE8;
L00407CDC:
    if (D_80130994 == 0) goto L00407EE8;
    v0 = D_801309B4;
    if (v0 != 0) goto L00407EE8;
    if (s2 != 0) goto L00407EF8;
    if (func_00425D94(s4, s5, &sp48, sp38, sp20, &sp4C, sp28, sp30) != 0) goto L00407EE8;
    if (sp48 == 0) goto L00407EE8;
    if (func_00426C74(sp38, D_800B45CC) != 0) goto L00407EE8;
    s0 = sp4C;
    if (D_800C2014 != 0) {
        func_002053A8(0);
        func_002052D8(0, D_800C2014);
    }
    if (s0 == 0) {
        s0 = D_800F70A8 + 0x688;
    } else {
        s0 = s0 << 8;
    }
    m = func_00204EDC(0, s0, 0x33, D_800C13F0);
    D_800C2014 = (s32)m;
    D_800C201C = m->addr;
    func_00266C5C((void *)D_800C201C, 0, s0);
    if (func_004264D0(s4, s5, D_800C201C, sp4C << 8) != 0) goto L00407E70;
    hs = *(s32 *)D_800C201C;
    if (hs != s7) goto L00407E70;
    if (func_00426D98(D_800C201C, hs - 4, 0xB) != *(s32 *)(*(s32 *)D_800C201C + D_800C201C - 4)) goto L00407E70;
    if (D_800F70A8 != *(s32 *)(D_800C201C + 4)) goto L00407E70;
    func_00416644(D_80119E2C, D_0043B7E4, arg1->f1C, arg1->f20, 0);
    return 1;
L00407E70:
    s2 = 1;
    if (D_800C2010 != 0 || D_800C2014 != 0) {
        func_002053A8(0);
    }
    if (D_800C2010 != 0) {
        func_002052D8(0, D_800C2010);
    }
    if (D_800C2014 != 0) {
        func_002052D8(0, D_800C2014);
    }
    D_800C2010 = 0;
    D_800C2014 = 0;
    D_800C2018 = 0;
    D_800C201C = 0;
L00407EE8:
    if (s2 == 0) return 0;
    v0 = D_801309B4;
L00407EF8:
    a1p = D_0043B610;
    if (v0 != 0) a1p = D_0043B5C8;
    D_80130988 = s5;
    func_00416644(D_80119E2C, D_0043C150, arg1->f1C, arg1->f20, a1p);
    return 1;
}
