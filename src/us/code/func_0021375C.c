/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002782E4.c, func_002782E4, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"

/* A vertex (0x10 bytes). */
typedef struct {
    /* 0x0 */ s16 pos[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 col[4];
} LitVtx;

/* One chunk of the vertex file. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ LitVtx v[1];
} VtxBlk;

extern u32 D_8011AAD8;          /* game flags */
extern u32 D_8011AAE4;          /* brightness, 0-255 */

VtxBlk *CISet__GetBlock(s32 *file, s32 index); /* returns chunk `index` of a chunked file */

#define MN(a, b) ((a) < (b) ? (a) : (b))

/* Writes the vertex colours of every chunk of file from the light levels in
 * src, brightened toward white by the brightness setting; black with game
 * flag 0x4000. */
void Lighting_WriteVertexColors(s32 *file, CVector3 *src) {
    s32 i;
    VtxBlk *b;
    CVector3 t;
    f32 f, x, a;
    s32 j, v, r;
    s32 k = 0;

    f = MN(1.0f, (f32) D_8011AAE4 * 0.003921568859f);
    if (D_8011AAD8 & 0x4000) {
        for (j = 0; j < *file; j++) {
            s32 n;
            LitVtx *vp;

            b = CISet__GetBlock(file, j);
            i = 0;
            n = b->count;
            vp = b->v;
            for (; i < n; i++) {
                vp[i].col[0] = 0;
                vp[i].col[1] = 0;
                vp[i].col[2] = 0;
            }
        }
    } else {
        for (j = 0; j < *file; j++) {
            s32 n;
            LitVtx *vp;

            b = CISet__GetBlock(file, j);
            i = 0;
            n = b->count;
            vp = b->v;
            for (; i < n; i++) {
                t = src[k];
                k++;
                x = t.x;
                if (!(x <= 255.0f)) x = 255.0f;
                if (!(5.0f <= x)) x = 5.0f;
                t.x = x * (1.0f - f) + f * 255.0f;
                x = t.y;
                if (!(x <= 255.0f)) x = 255.0f;
                if (!(5.0f <= x)) x = 5.0f;
                t.y = x * (1.0f - f) + f * 255.0f;
                a = t.z;
                if (!(a <= 255.0f)) a = 255.0f;
                x = a;
                if (!(5.0f <= x)) x = 5.0f;
                t.z = x * (1.0f - f) + f * 255.0f;
                if (!(vp[i].flag & 0x80)) {
                    v = (s32) t.x;
                    r = v >= 256 ? 255 : v < 0 ? 0 : v < 256 ? v : 255;
                    vp[i].col[0] = r;
                    v = (s32) t.y;
                    r = v >= 256 ? 255 : v < 0 ? 0 : v < 256 ? v : 255;
                    vp[i].col[1] = r;
                    v = (s32) t.z;
                    r = v >= 256 ? 255 : v < 0 ? 0 : v < 256 ? v : 255;
                    vp[i].col[2] = r;
                }
            }
        }
    }
}
