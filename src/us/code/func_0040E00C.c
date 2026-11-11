#include "common.h"
#include "structs/game_object.h"
#include "structs/vector.h"
#include "structs/boss.h"
#include "globals/boss.h"

#ifdef TARGET_PORT
typedef struct Actor40E00C {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor40E00C(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Actor40E00C(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_Actor40E00C(header.m_pCurrentRegion)]; sptr region; }; /* host: a region pointer */ /* N64 0x10 */
        struct { u8 _hp_type[T2_HOFF_Actor40E00C(m_nTypeFlag)]; u16 type; }; /* N64 0xB8 */
    };
} Actor40E00C;
#else
typedef struct {
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ s32 region;
    /* 0x14 */ char pad14[0xA4];
    /* 0xB8 */ u16 type;
} Actor40E00C;
#endif

#ifdef TARGET_PORT
typedef struct Spawn40E00C {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Spawn40E00C(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_cur[T2_HOFF_Spawn40E00C(m_Health)]; s32 cur; }; /* N64 0x4 */
        struct { u8 _hp_start[T2_HOFF_Spawn40E00C(m_InitialHealth)]; s32 start; }; /* N64 0x8 */
        struct { u8 _hp_unk24[T2_HOFF_Spawn40E00C(m_HeadTurn)]; s32 unk24; }; /* N64 0x24 */
        struct { u8 _hp_unk28[T2_HOFF_Spawn40E00C(m_HeadTilt)]; s32 unk28; }; /* N64 0x28 */
        struct { u8 _hp_pos[T2_HOFF_Spawn40E00C(m_vInitialPos)]; CVector3 pos; }; /* N64 0x9C */
        struct { u8 _hp_region[T2_HOFF_Spawn40E00C(m_pInitialRegion)]; sptr region; }; /* host: a region pointer */ /* N64 0xA8 */
    };
} Spawn40E00C;
#else
typedef struct {
    /* 0x00 */ char pad0[0x4];
    /* 0x04 */ s32 cur;
    /* 0x08 */ s32 start;
    /* 0x0C */ char padC[0x18];
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ char pad2C[0x70];
    /* 0x9C */ CVector3 pos;
    /* 0xA8 */ s32 region;
} Spawn40E00C;
#endif


void BlindOne_ActorAppearEnter(Actor40E00C *actor, Spawn40E00C *spawn) {
    BlindOneState *stats;

    spawn->unk28 = 0;
    spawn->unk24 = 0;
    spawn->cur = spawn->start;
    actor->pos = spawn->pos;
    actor->region = spawn->region;
    stats = &gBlindOne;
    switch (actor->type) {
        case 0x454:
            stats->alive1++;
            break;
        case 0x455:
            stats->alive2++;
            break;
        case 0x456:
            stats->alive3++;
            break;
    }
}
