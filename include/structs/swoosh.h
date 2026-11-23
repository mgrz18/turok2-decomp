#ifndef STRUCTS_SWOOSH_H
#define STRUCTS_SWOOSH_H

/* The swoosh/effect system, gEngineApp_m_SwooshSystem (CEngineApp + 0x19608): weapon and
 * particle trails (swooshes), the points they are made of, repeating timers, the ground
 * decals (drawn with gShadowTexture by SwooshSystem_DrawDecals) and the ribbon vertex pool the
 * textured trails are drawn from. The code is Rage Wars' (src/RENDER_3, src/SYSTEM_3) at
 * Turok 2's offsets. The offsets below are the N64's; every link is a pointer, so on the
 * host the members sit where the host types put them and the code reaches them by name.
 *
 * Lists: SwooshLinks is the list of DList_* and NodePool_* (head, tail, count; an element
 * links to its previous element in its first word and to its next in its second); a
 * SwooshPool is a NodePool (a free and a used SwooshLinks); the decal lists are CLists
 * (links at 0 and sizeof(void *)). */

#include "common.h"
#include "structs/list.h"
#include "structs/vector.h"

/* A libultra Vtx_t (structs/gbi.h; not included here: game.h includes this header). */
typedef struct SwooshVtx {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} SwooshVtx; /* size 0x10 */

/* A libultra Mtx kept as words (copied by value between the two frame buffers). */
typedef struct SwooshMtx {
    /* 0x00 */ s32 w[16];
} SwooshMtx; /* size 0x40 */

typedef struct SwooshLinks {
    /* 0x0 */ void *head;
    /* 0x4 */ void *tail;
    /* 0x8 */ s32 count;
} SwooshLinks; /* size 0xC */

/* The two links every element of a SwooshLinks list starts with (DList_*): its previous
 * element in its first word, its next in its second (pointer-wide on the host). */
typedef struct DListLink {
    /* 0x0 */ struct DListLink *prev;
    /* 0x4 */ struct DListLink *next;
} DListLink;

typedef struct SwooshPool {
    /* 0x00 */ SwooshLinks free;
    /* 0x0C */ SwooshLinks used;
} SwooshPool; /* size 0x18 (a NodePool) */

/* One point of a swoosh (Rage Wars' ObjectLinksB4 / EmitNode). */
typedef struct SwooshNode {
    /* 0x00 */ struct SwooshNode *prev;
    /* 0x04 */ struct SwooshNode *next;
    /* 0x08 */ f32 life;                /* counts down; <= 0: fading, removed below -3 */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ CVector3 pos;
    /* 0x1C */ CVector3 scale;          /* flags & 4: the axes' scale instead of matrices */
    /* 0x28 */ SwooshMtx mtx[2];        /* one per frame buffer (gEvenOdd) */
    /* 0xA8 */ f32 fraction;            /* where along the swoosh (0..1) */
    /* 0xAC */ f32 length;              /* to the next node */
    /* 0xB0 */ void *owner;             /* the particle or object it follows; -1: frozen */
    /* 0xB4 */ s32 unkB4;
} SwooshNode; /* size 0xB8 */

/* One step of a textured ribbon's cross-section (structs/morph.h's Morph.items): the colours
 * and the two points it blends. */
typedef struct MorphItem {          /* 0xC bytes */
    /* 0x00 */ u8 colorA[4];
    /* 0x04 */ u8 colorB[4];
    /* 0x08 */ u16 pointA;
    /* 0x0A */ u16 pointB;
} MorphItem;

/* A swoosh record (CScene__GetSwoosh, the level's "particle swooshes", ROM data: the same bytes
 * on the host): a point's life, how the texture runs, the colours and the texture blend steps. */
typedef struct SwooshInfo {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 fadeTime;            /* Swoosh_DetachOwner */
    /* 0x08 */ f32 nodeLife;            /* a new point's SwooshNode.life */
    /* 0x0C */ s32 bone;                 /* an object swoosh: the bone it follows (SwooshSystem_AddBonePoints) */
    /* 0x10 */ s16 sound;
    /* 0x12 */ u8 closed;
    /* 0x13 */ u8 unk13[0x16 - 0x13];
    /* 0x16 */ u8 prim[3];
    /* 0x19 */ u8 env[3];
    /* 0x1C */ u8 texMode;              /* 0: the texture runs along the length, else with time */
    /* 0x1D */ u8 textureScale;
    /* 0x1E */ u8 animMode;             /* Trail_GetAnimFrame */
    /* 0x1F */ u8 tex0;                 /* sTrailTextures index */
    /* 0x20 */ u8 tex1;
    /* 0x21 */ u8 scaleS;
    /* 0x22 */ u8 scaleT;
    /* 0x23 */ u8 count;                /* blends[count] */
    /* 0x24 */ MorphItem blends[1];     /* the ribbon's cross-section (Morph.items) */
} SwooshInfo;

/* A swoosh (a trail): its record (CScene__GetSwoosh), textures, owner and points. */
typedef struct Swoosh {
    /* 0x00 */ struct Swoosh *prev;
    /* 0x04 */ struct Swoosh *next;
    /* 0x08 */ SwooshInfo *info;        /* the swoosh record */
    /* 0x0C */ void *tex0;              /* sTrailTextures[info->tex0] */
    /* 0x10 */ void *tex1;
    /* 0x14 */ s32 texture[2];          /* Material_GetTextureSet's handle for the record's texture id */
    /* 0x1C */ void *owner;             /* flags & 1: a CGameObjectInstance, & 2: a CParticle, & 8: a key */
    /* 0x20 */ s32 id;
    /* 0x24 */ f32 time;
    /* 0x28 */ f32 unk28;
    /* 0x2C */ f32 endFrame;            /* the texture animation's (Trail_GetAnimFrame) */
    /* 0x30 */ f32 frame;
    /* 0x34 */ s32 layer;               /* the record's id; < 0 drawn first, untextured */
    /* 0x38 */ s32 randomFrame;         /* -1: not picked yet */
    /* 0x3C */ u32 flags;
    /* 0x40 */ SwooshLinks nodes;       /* SwooshNode */
    /* 0x4C */ f32 length;
    /* 0x50 */ s32 unk50;
} Swoosh; /* size 0x54 */

/* A repeating timer (SwooshSystem_AddTimer, SwooshSystem_UpdateTimers). */
typedef struct SwooshTimer {
    /* 0x00 */ struct SwooshTimer *prev;
    /* 0x04 */ struct SwooshTimer *next;
    /* 0x08 */ f32 time;
    /* 0x0C */ f32 period;
    /* 0x10 */ s32 repeats;
    /* 0x14 */ sptr arg;
    /* 0x18 */ s32 (*callback)(struct SwooshTimer *);
} SwooshTimer; /* size 0x1C */

/* A ground decal: a CList element and its model matrix (SwooshSystem_DrawDecals draws them). */
typedef struct SwooshDecal {
    /* 0x00 */ struct SwooshDecal *prev;
    /* 0x04 */ struct SwooshDecal *next;
    /* 0x08 */ SwooshMtx mtx;
    /* 0x48 */ sptr owner;
    /* 0x4C */ s32 unk4C;
} SwooshDecal; /* size 0x50 */

typedef struct CSwooshSystem {
    /* 0x0000 */ SwooshVtx m_RibbonVtx[2][300];     /* the ribbon vertex pool: one buffer per frame */
    /* 0x2580 */ s32 m_RibbonVtxLeft;
    /* 0x2584 */ SwooshVtx *m_pRibbonVtx;
    /* 0x2588 */ SwooshNode m_Nodes[96];
    /* 0x6A88 */ SwooshLinks m_FreeNodes;
    /* 0x6A94 */ Swoosh m_Swooshes[32];
    /* 0x7514 */ SwooshPool m_SwooshPool;       /* the live swooshes: m_SwooshPool.used */
    /* 0x752C */ SwooshTimer m_Timers[3];
    /* 0x7580 */ SwooshPool m_TimerPool;
    /* 0x7598 */ SwooshDecal m_Decals[100];
    /* 0x94D8 */ CList m_DecalLists[10];
    /* 0x95A0 */ CList m_FreeDecals;           /* m_nCount == 100: no decal to draw */
} CSwooshSystem; /* size 0x95B4: 0x1C bytes past m_SwooshSystem (0x9598), into unk22BA0 */

#if defined(TARGET_PORT) && defined(__mips__)
_Static_assert(sizeof(SwooshNode) == 0xB8, "SwooshNode");
_Static_assert(sizeof(Swoosh) == 0x54, "Swoosh");
_Static_assert(sizeof(SwooshTimer) == 0x1C, "SwooshTimer");
_Static_assert(sizeof(SwooshDecal) == 0x50, "SwooshDecal");
_Static_assert(__builtin_offsetof(CSwooshSystem, m_FreeNodes) == 0x6A88, "CSwooshSystem.m_FreeNodes");
_Static_assert(__builtin_offsetof(CSwooshSystem, m_SwooshPool) == 0x7514, "CSwooshSystem.m_SwooshPool");
_Static_assert(__builtin_offsetof(CSwooshSystem, m_TimerPool) == 0x7580, "CSwooshSystem.m_TimerPool");
_Static_assert(__builtin_offsetof(CSwooshSystem, m_DecalLists) == 0x94D8, "CSwooshSystem.m_DecalLists");
_Static_assert(sizeof(CSwooshSystem) == 0x95B4, "CSwooshSystem");
#endif

#endif
