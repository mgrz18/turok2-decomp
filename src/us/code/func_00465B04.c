#include "structs/vector.h"
#include "common.h"
#include "structs/world.h"

/* The second VM module's copy of CScene__DecompressIntelligence, from the same source file as
 * func_002261A8.c: the engine's func_002261A8 and the VM's CScene__DecompressIntelligence.
 * Its code reads the first module's pool (versions/shared_rodata.us.txt):
 * func_002261A8's -1.0f at 0x800A6010, then this function's 1.0f and its jump
 * table at 0x800A6018, so the file's .rodata has to start with that -1.0f
 * for the table to land 8-aligned where the ROM has it. The engine half is
 * compiled into .text_engine, which no output section names: the linker
 * script's /DISCARD/ drops it. docs/VMASM.md. */

void CVector3__Subtract(CVector3 *out, CVector3 *a, u8 *b);
void *CISet__GetBlock();
static s32 func_002261A8(CScene *lv, s32 unused, CVector3 *pt) __attribute__((section(".text_engine")));

static s32 func_002261A8(CScene *lv, s32 unused, CVector3 *pt) {
    CVector3 d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    t = CISet__GetBlock(CISet__GetBlock(lv->unk_68), 1);
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            CVector3__Subtract(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C;
        } while (i < n);
    }
    return (s32)bestIdx;
}

s32 ProPackDecompress(void ***resource, s32 index);
void *CISet__GetBlock(s32 *table, s32 index);

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x24];
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z30;
    /* 0x34 */ f32 z34;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ f32 x2;
    /* 0x50 */ f32 y2;
    /* 0x54 */ f32 z2;
} Element;

#ifdef TARGET_PORT
int T2Port_LegacyIntel9(void); /* port/game/t2debug.c */
#endif
void CScene__DecompressIntelligence_2(s32 unused, void ***resource) {
    f32 scale = 1.0f;
    s32 *table;
    s32 count;
    s32 i;
    Element *e;

    if (ProPackDecompress(resource, 0)) {
        table = (s32 *) **resource;
        count = *table;
        for (i = 0; i < count; i++) {
            e = CISet__GetBlock(table, i);
            switch (e->type) {
                case 1:
                    e->x *= scale;
                    e->z30 *= scale;
                    e->z34 *= scale;
                    e->x2 *= scale;
                    e->y2 *= scale;
                    e->z2 *= scale;
                    break;
                case 11:
                    e->x *= scale;
                    e->y *= scale;
                    break;
                case 0:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    break;
            }
#ifdef TARGET_PORT
            /* The port (round 17 f3, tools/port/n64cmp.py): the archive's swap plan has no struct for
             * the type-9 (interactive anim) body (tools/assets/swap_spec.json, "no struct yet"), so its
             * three words (the type flags, the start animation id, 0) stayed big-endian: on a
             * little-endian host InteractiveAnim_Advance took 0xFD530000 for animation 0x53FD and the
             * flag tests (m_dwTypeFlags & 1, ...) read the wrong bits. Swapped here, once per load.
             * T2_LEGACY_INTEL9=1 (DEBUG-ONLY) keeps the old bytes. */
            if (e->type == 9 && !T2Port_LegacyIntel9()) {
                u32 *w = (u32 *) ((u8 *) e + 0x14);
                w[0] = __builtin_bswap32(w[0]);
                w[1] = __builtin_bswap32(w[1]);
                w[2] = __builtin_bswap32(w[2]);
            }
#endif
        }
    }
}
