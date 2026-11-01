/* NON-MATCHING: 187 of 249 words differ (W batch). */
/* NON-MATCHING: 187 of 249 words differ: same size and control flow; register allocation differs (the ROM keeps the handles in s0/s1 and needs one s register less) and the display-list tail is scheduled differently. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/OVLGAME/80405454_3.CPP, func_804058B0_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    u32 w0;
    u32 w1;
} GfxWords;

extern void **func_00204EDC(s32 heap, s32 size, s32 flags, char *name); /* allocate a handle */
extern void **func_00204FD0(s32 heap, u32 addr, s32 size, char *name);  /* claim a fixed block */
extern void func_002052D8(s32 heap, void **handle);                     /* free a handle */
extern u32 func_00266B80(void);                                         /* osGetMemSize */
extern void func_00266C5C(void *dst, s32 value, s32 size);              /* memset */

extern char D_800C1810[];
extern char D_800C181C[];
extern char D_800C182C[];
extern char D_800C184C[];
extern char D_800C185C[];
extern char D_800C1864[];
extern u32 D_800C2210;        /* frame buffer count */
extern s32 D_800C2214;
extern s32 D_800C2218;
extern void *D_80130A88[];    /* frame buffers */
extern void *D_80130A78;      /* depth buffer */
extern void *D_80130A40;
extern s32 D_80130A44;
extern void *D_80130A48;
extern GfxWords *D_80130A4C;
extern GfxWords *D_80130A50;
extern s32 D_80130A7C;
extern GfxWords *D_80130A80;
extern GfxWords *D_80130A84;

/* Frees and reallocates the frame buffers, depth buffer and display list memory. */
s32 func_0045E140(void ***p) {
    s32 i;
    s32 size;
    void **fb;
    void **block;
    u32 mem;
    u32 mb = 0x100000;

    if (p[1] != NULL) {
        func_002052D8(0, p[1]);
    }
    if (p[2] != NULL) {
        func_002052D8(0, p[2]);
    }
    if (p[6] != NULL) {
        func_002052D8(0, p[6]);
    }
    if (p[7] != NULL) {
        func_002052D8(0, p[7]);
    }
    for (i = 0; i < 3; i++) {
        if (p[3 + i] != NULL) {
            func_002052D8(0, p[3 + i]);
        }
    }
    p[0] = NULL;
    p[1] = NULL;
    p[2] = NULL;
    p[6] = NULL;
    p[7] = NULL;
    for (i = 2; i >= 0; i--) {
        p[3 + i] = NULL;
    }
    if (func_00266B80() >= mb * 6) {
        mem = func_00266B80() | 0x80000000;
        size = 0x54600;
        block = func_00204FD0(0, mem - mb, size, D_800C1810);
        fb = func_00204FD0(0, mem - mb * 2, 0xFD200, D_800C181C);
        func_00266C5C(*fb, 0, 0xFD200);
        func_00266C5C(*block, 0, size);
        p[6] = block;
        p[3] = fb;
        D_80130A78 = *block;
        for (i = 0; i < D_800C2210; i++) {
            D_80130A88[i] = (u8 *) *fb + i * size;
        }
    } else {
        size = 0x1ECC0;
        mem = 0x80400000;
        block = func_00204FD0(0, mem - (D_800C2210 + 1) * size, (D_800C2210 + 1) * size, D_800C182C);
        p[3] = block;
        for (i = 0; i < D_800C2210; i++) {
            D_80130A88[i] = (u8 *) *block + i * size;
        }
        D_80130A78 = (u8 *) *block + size * i;
    }
    for (i = 0; i < D_800C2210; i++) {
        func_00266C5C(D_80130A88[i], 0, size);
    }
    func_00266C5C(D_80130A78, 0, size);
    p[1] = func_00204EDC(0, (D_800C2214 + D_800C2218) * 0x10, 0x23, D_800C184C);
    p[2] = func_00204EDC(0, 0x8000, 0x23, D_800C185C);
    p[7] = func_00204EDC(0, 0x22000, 0x23, D_800C1864);
    D_80130A44 = 0x8000;
    D_80130A7C = 0x22000;
    D_80130A80 = *(GfxWords **) p[1];
    D_80130A40 = *p[7];
    D_80130A4C = D_80130A80 + D_800C2214;
    D_80130A84 = D_80130A4C + D_800C2218;
    D_80130A48 = *p[2];
    D_80130A50 = D_80130A84 + D_800C2214;
    for (i = 0; i < D_800C2210; i++) {
    }
    p[0] = (void **) 1;
    return 1;
}
