/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0021E5D4.c, func_0021E5D4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
/* Turok 2 layout (Rage Wars: pos 0x8, timer 0x678, fx 0x67C..0x684, flags 0x6B0). */
typedef struct B05P14 {
    u8 pad0[0x4];
    Vec3f pos;              /* 0x004 */
    u8 pad10[0xA30 - 0x10];
    f32 timer;              /* 0xA30 */
    void *fx1;              /* 0xA34 */
    void *fx2;              /* 0xA38 */
    void *fx3;              /* 0xA3C */
    u8 padA40[0xA68 - 0xA40];
    u32 flagsA68;           /* 0xA68 */
} B05P14;
typedef struct B05Fx { u8 pad0[8]; void *unk8; f32 unkC; s16 unk10; s16 unk12; s16 unk14; s16 unk16; } B05Fx;
extern s32 D_8011AAD8;
extern u8 D_8010FF34[];
extern u8 D_800ADB10[];
extern u8 D_800ADB18[];
extern u8 D_800ADB20[];
extern u8 D_800ADB28[];
void func_00228B4C(u8 *, void *);
s32 func_002113A4();
B05Fx *func_00228AEC(u8 *, u8 *);
void func_0021F00C(Vec3f *out, B05P14 *p, Vec3f off);
void func_0020EEF8(Vec3f *out, Vec3f *a, Vec3f *b);
f32 func_002671B4(B05P14 *p);

/* A separate function in Rage Wars; inlined here in Turok 2. */
static inline void fxupd(B05P14 *p, void **slot, u8 *tab, f32 offz, f32 k1, f32 k2) {
    B05Fx *fx = *slot;
    Vec3f off;
    Vec3f v;
    if (fx == ((void *)0)) {
        fx = func_00228AEC(D_8010FF34, tab);
        if (fx == ((void *)0)) {
            return;
        }
    }
    *slot = fx;
    off.x = 0.0f;
    off.y = 0.0f;
    off.z = offz;
    func_0021F00C(&v, p, off);
    func_0020EEF8(&v, &v, &p->pos);
    fx->unk16 = 1;
    fx->unk10 = v.x;
    fx->unk12 = v.y + func_002671B4(p) * k1;
    fx->unk14 = v.z;
    fx->unkC = k2;
}
void func_0024AB0C(B05P14 *p) {
    s32 vis;
    if (D_8011AAD8 & 0x4000) {
        p->timer = 0.0f;
    } else {
        if (p->fx3 != ((void *)0)) {
            func_00228B4C(D_8010FF34, p->fx3);
            p->fx3 = ((void *)0);
        }
    }
    if (p->timer != 0.0f) {
        fxupd(p, &p->fx1, D_800ADB10, 204.79999f, 0.5f, 1.0f);
        fxupd(p, &p->fx2, D_800ADB18, 614.39996f, 0.5f, 1.0f);
        if (p->timer < 70.0f) {
            vis = 1;
            if (p->timer < 20.0f) {
                vis = ((func_002113A4() % 100) < 0x32) ^ vis;
            } else if ((func_002113A4() % 100) < 0xF) {
                vis = 0;
            }
            if (p->fx1 != ((void *)0)) {
                ((B05Fx *)p->fx1)->unk16 = vis;
            }
            if (p->fx2 != ((void *)0)) {
                ((B05Fx *)p->fx2)->unk16 = vis;
            }
        }
    } else {
        if (p->fx1 != ((void *)0)) {
            func_00228B4C(D_8010FF34, p->fx1);
            p->fx1 = ((void *)0);
        }
        if (p->fx2 != ((void *)0)) {
            func_00228B4C(D_8010FF34, p->fx2);
            p->fx2 = ((void *)0);
        }
    }
    if (D_8011AAD8 & 0x4000) {
        fxupd(p, &p->fx3, D_800ADB20, 0.0f, 0.5f, 1.0f);
        if ((p->flagsA68 & 0x20) && p->fx3 != ((void *)0)) {
            if (((B05Fx *)p->fx3)->unk8 == D_800ADB20) {
                ((B05Fx *)p->fx3)->unk8 = D_800ADB28;
            } else {
                ((B05Fx *)p->fx3)->unk8 = D_800ADB20;
            }
        }
    }
}
