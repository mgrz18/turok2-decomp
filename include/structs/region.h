#ifndef STRUCTS_REGION_H
#define STRUCTS_REGION_H

/* The level floor: a mesh of triangular regions (CGameRegion, what an
 * object's m_pCurrentRegion points to), each linked to its three edge
 * neighbours (the region graph the path search and the floor walks follow),
 * and the region attributes (CRegionInfo) the regions share by index.
 *
 * Both live in the level's region data (CWorld.m_pRegionData, see
 * structs/world.h): section 0 holds the CRegionInfo table (0x64 each,
 * func_0022425C indexes it with m_nInfo), section 2 the regions (0x20 each),
 * section 3 the floor grid. D_800B2608 caches the region whose normal was
 * computed last. Rage Wars has the same region (CGameRegion_t in its
 * COMMON/PACKED.H) with unnamed fields.
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "structs/game_object.h"

/* m_wFlags */
#define REGION_FLAG_WATER       0x0001 /* has a water surface (CRegionInfo.m_WaterHeight) */
#define REGION_FLAG_CEILING     0x0040
#define REGION_FLAG_REGISTERED  0x0400 /* seen on the map */
#define REGION_FLAG_VISITED     0x8000 /* marked by a graph walk */

typedef struct CGameRegion {
    /* 0x00 */ u16 m_nInfo;                          /* index into the CRegionInfo table */
    /* 0x02 */ u16 m_wFlags;                         /* REGION_FLAG_* */
    /* 0x04 */ CVector3 *m_pVertices[3];
    /* 0x10 */ struct CGameRegion *m_pNeighbours[3]; /* across each edge, NULL on the border */
    /* 0x1C */ s32 m_VisBits;                        /* tested against a view's visibility mask */
} CGameRegion; /* size 0x20 */

/* The attributes of a set of regions: water, damage, ceilings, map colour... */
typedef struct CRegionInfo {
    /* 0x00 */ u8 unk_00[0x20];
    /* 0x20 */ f32 m_WaterHeight;
    /* 0x24 */ u8 unk_24[0x4];
    /* 0x28 */ f32 m_DamageInterval;
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ u8 unk_30[0x4];
    /* 0x34 */ CVector3 m_vPos;
    /* 0x40 */ u8 unk_40[0x4];
    /* 0x44 */ u32 m_dwFlags;
    /* 0x48 */ u8 unk_48[0x8];
    /* 0x50 */ u16 m_DamageAmount;
    /* 0x52 */ u16 m_wFlags52;
    /* 0x54 */ u16 m_wFlags54;
    /* 0x56 */ u16 m_EmitterMask;
    /* 0x58 */ s8 unk_58;
    /* 0x59 */ u8 unk_59[0x2];
    /* 0x5B */ u8 unk_5B;
    /* 0x5C */ u8 m_MapColour;
    /* 0x5D */ u8 unk_5D[0x7];
} CRegionInfo; /* size 0x64 */

#endif
