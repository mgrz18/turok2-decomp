/* Boundary fixed in versions/cuts.us.txt: splat starts this function 2 words early (0x285CE8: never-executed
 * `swc1 $f21,0x10($sp); sw $s0,0x14($sp)`); the real entry is 0x285CF0, as the
 * pointer `.word 0x00285CF0` in us/asm/data/AB858.rodata.s shows. */
#include "common.h"

typedef struct LevelView {
    u8 pad0[0x23FD0];
    f32 spin;           /* 0x23FD0: degrees */
} LevelView;

extern s32 D_800AB8A0;
extern u8 D_800F7078[];
extern u8 D_801198F0[];
extern s32 D_800B6D1C;              /* frame buffer index */
extern f32 D_800F67C0[][16];        /* per-frame matrices */
extern u8 D_800AB0E8[];
extern u8 D_800B36F4[];
extern s32 D_800C2030;              /* screen width */

extern s32 func_00224180(void *, s32);
extern void func_0027E000(void *);
extern void func_0020FC24(f32 *matrix, f32 x, f32 y, f32 z);
extern void func_00210894(f32 *matrix, f32 angle);
extern void func_00210C3C(f32 *matrix);
extern void func_0020D274(f32 *src, f32 *dst);
extern s32 func_00224F84(void *, s32);
extern void func_0020B5D4(void);
extern void func_0020B7F8(s32 key, void *matrix, void *lights, s32 lod, s32 pass);
extern void func_0020B624(void);
extern s32 func_00286E14(LevelView *, s32, void *, s32, s32, s32, f32, f32);

void func_00285CF0(LevelView *view) {
    f32 mtx[16];
    s32 id;
    s32 key;

    D_800AB8A0 = 0;
    id = func_00224180(D_800F7078, 0xE10);
    if (id == -1) {
        return;
    }
    func_0027E000(D_801198F0);
    func_0020FC24(mtx, 0.0f, -100.0f, -500.0f);
    func_00210894(mtx, view->spin * 0.17453295f);
    func_00210C3C(mtx);
    func_0020D274(mtx, D_800F67C0[D_800B6D1C]);
    key = func_00224F84(D_800F7078, id);
    func_0020B5D4();
    func_0020B7F8(key, D_800F67C0[D_800B6D1C], D_800AB0E8, 0, -1);
    func_0020B624();
    func_00286E14(view, 0, D_800B36F4, D_800C2030 / 2, 0xBE, 0xA, 0.8f, 1.0f);
}
