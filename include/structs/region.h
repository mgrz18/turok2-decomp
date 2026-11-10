#ifndef STRUCTS_REGION_H
#define STRUCTS_REGION_H

/* The level floor: a mesh of triangular regions (CGameRegion, what an
 * object's m_pCurrentRegion points to), each linked to its three edge
 * neighbours (the region graph the path search and the floor walks follow),
 * and the region attributes (CRegionInfo) the regions share by index.
 *
 * Both live in the level's region data (CScene.m_pRegionData, see
 * structs/world.h): section 0 holds the CRegionInfo table (0x64 each,
 * CScene__GetRegionInfo indexes it with m_nInfo), section 2 the regions (0x20 each),
 * section 3 the floor grid. sCachedRawNormalRegion caches the region whose normal was
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

/* m_nInfo and m_wFlags read as one word, as some code tests the flags
 * ((REGION_WORD(r) & 6) == 2: a closed edge region). */
#define REGION_WORD(r) (*(u32 *)(r))

/* The attributes of a set of regions: water, damage, ceilings, map colour... */
typedef struct CRegionInfo {
    /* The camera attributes CCamera__AdvanceAttributes blends to (its CRegionSetAttributes view);
     * typed so that the archive's byte-swap plan (tools/assets/swap_spec.json reads this header)
     * swaps the fog start, far clip and field of view: as bytes they reached the host big-endian. */
    /* 0x00 */ u8 m_FogColor[4];
    /* 0x04 */ u8 m_HeldFogColor[4];                 /* under water / held by the player's flags */
    /* 0x08 */ u32 m_FogStart;
    /* 0x0C */ u32 m_HeldFogStart;
    /* 0x10 */ f32 m_FarClip;
    /* 0x14 */ f32 m_FieldOfView;
    /* 0x18 */ f32 m_HeldFarClip;
    /* 0x1C */ f32 m_HeldFieldOfView;
    /* 0x20 */ f32 m_WaterHeight;
    /* 0x24 */ f32 m_BlendTime;                      /* in 15ths of a second */
    /* 0x28 */ f32 m_DamageInterval;
    /* 0x2C */ f32 unk_2C;
    /* 0x30 */ f32 unk_30;                           /* 1.0 in the data */
    /* 0x34 */ CVector3 m_vPos;
    /* 0x40 */ f32 unk_40;                           /* 665.6 in the data */
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
