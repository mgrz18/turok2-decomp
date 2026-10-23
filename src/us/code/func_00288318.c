typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    /* 0x000 */ char pad0[0x114];
    /* 0x114 */ s32 colorImage;
    /* 0x118 */ s32 depthImage;
    /* 0x11C */ char pad11C[0x24];
} GfxTask288318; /* size 0x140 */

typedef struct {
    /* 0x000 */ GfxTask288318 tasks[3];
    /* 0x3C0 */ GfxTask288318 *cur;
} GfxState288318;

extern u32 D_800B6D18; /* frame counter */
extern s32 D_800B6D1C; /* double-buffer index */
extern s32 D_800B6D20;
extern u32 D_800C2210; /* task count */
extern s32 D_80130A4C;
extern s32 D_80130A50;
extern s32 D_80130A80;
extern s32 D_80130A84;

void func_00288318(GfxState288318 *gfx) {
    D_800B6D18++;
    D_800B6D20++;
    D_800B6D1C ^= 1;
    gfx->cur = &gfx->tasks[D_800B6D18 % D_800C2210];
    gfx->cur->colorImage = D_800B6D1C ? D_80130A84 : D_80130A80;
    gfx->cur->depthImage = D_800B6D1C ? D_80130A4C : D_80130A50;
}
