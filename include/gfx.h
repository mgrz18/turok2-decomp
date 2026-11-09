#ifndef GFX_H
#define GFX_H

/* One display list command: two words, as libultra's Gfx is to the RSP. The
 * engine writes commands one at a time through gDList, the current
 * position in the frame's display list (turok3 calls its gDisplayList):
 *
 *     g = gDList++;
 *     g->w0 = 0xE7000000;      (gDPPipeSync)
 *     g->w1 = 0;
 *
 * The port build (TARGET_PORT) gives the words the host pointer size, the
 * layout of libultraship's Gfx (Gwords { uintptr_t w0, w1; }), so w1 can hold
 * a real address and a display list built here runs through Fast3D as is.
 * Code that sizes display lists in bytes uses GFX_SIZE, never 8. See
 * include/gbi_t2.h and docs/port/GBI.md. */
#ifdef TARGET_PORT
typedef __UINTPTR_TYPE__ GfxWord;
#define GFX_SIZE (2 * (int) sizeof(GfxWord))
#else
typedef u32 GfxWord;
#define GFX_SIZE 8
#endif

typedef struct {
    /* 0x0 */ GfxWord w0;
    /* 0x4 */ GfxWord w1;
} Gfx;

extern Gfx *gDList;

#endif
