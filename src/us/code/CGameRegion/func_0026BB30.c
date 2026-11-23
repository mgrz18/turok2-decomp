/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802744AC_de_4.c, func_80275AA4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
/* The port: the same through CGameRegion (structs/region.h), whose host size is not the 32 bytes
 * the neighbour index is scaled by; vertices stay 16 bytes (CollisionBuildVertex). */
#include "common.h"
#include "structs/region.h"

typedef struct {
    /* 0x00 */ u16 info;
    /* 0x02 */ u16 flags;
    /* 0x04 */ u16 vertex[3];
    /* 0x0A */ u16 neighbour[3];
    /* 0x10 */ s32 visBits;
} PackedRegion; /* size 0x14 */

void CGameRegion__Unpack(CGameRegion *dst, PackedRegion *src, char *base, int unused, CGameRegion *extra) {
    int i;
    dst->m_nInfo = src->info;
    dst->m_wFlags = src->flags;
    dst->m_VisBits = src->visBits;
    for (i = 0; i < 3; i++) {
        dst->m_pVertices[i] = (CVector3 *)(base + src->vertex[i] * 16);
        dst->m_pNeighbours[i] = src->neighbour[i] == 0xFFFF ? NULL : &extra[src->neighbour[i]];
    }
}
#else
#include "rw/abu_types.h"

extern void CGameRegion__Unpack(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra);
void CGameRegion__Unpack(HeaderRecAt1C *dst, Packed *src, char *base, int unused, char *extra) {
    int i;
    dst->unk0 = src->unk0;
    dst->unk2 = src->unk2;
    dst->unk1C = src->unk10;
    for (i = 0; i < 3; i++) {
        dst->first[i] = base + src->first[i] * 16;
        if (src->second[i] == 0xFFFF) {
            dst->second[i] = 0;
        } else {
            dst->second[i] = extra + src->second[i] * 32;
        }
    }
}
#endif
