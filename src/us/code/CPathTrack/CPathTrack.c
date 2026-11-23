#include "common.h"
#include "globals/scene.h"

/* A CPathTrack (callers pass &ai->m_PathTrack): steps the current point along
 * its path, ping-pong or looping as the path says. Not a sprite animation
 * player, as this comment once said: animId is the path index
 * (m_nCurrentPathIndex), frame the point index (m_nCurrentPointIndex). */
typedef struct AnimPlayer {
    s8 state;       /* 0x0: direction, 1-2 forward, 3-4 backward */
    u8 animId;      /* 0x1: the path */
    s16 frame;      /* 0x2: the point on it */
} AnimPlayer;
typedef struct AnimDef {
    s16 mode;       /* 0x0: 0 ping-pong, 1 loop */
    s16 frameCount; /* 0x2: its points */
} AnimDef;
extern AnimDef *CScene__GetPathHeader(void *table, u8 id);

void CPathTrack__Advance(AnimPlayer *player) {
    AnimDef *anim;
    s32 count;

    anim = CScene__GetPathHeader(&gScene, player->animId);
    count = anim->frameCount;
    switch (player->state) {
    case 1:
    case 2:
        player->state = 1;
        player->frame++;
        if (player->frame == count) {
            switch (anim->mode) {
            case 0:
                player->frame = count - 2;
                player->state = 3;
                break;
            case 1:
                player->frame = 0;
                break;
            }
        }
        break;
    case 3:
    case 4:
        player->state = 3;
        player->frame--;
        if (player->frame < 0) {
            switch (anim->mode) {
            case 0:
                player->frame = 1;
                player->state = 1;
                break;
            case 1:
                player->frame = count - 1;
                break;
            }
        }
        break;
    }
}

#include "structs/game_object.h"
#ifdef TARGET_PORT
struct CVector3;
s16 CScene__FindNearestPathPoint(void *scene, s32 index, struct CVector3 *pt);
#else
s16 CScene__FindNearestPathPoint(s32 *);                       /* extern */
#endif

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FindNearestPathPoint gets this function's
 * $a1 (the path index) and $a2 (the position, from the caller CGameObjectInstance__Construct) untouched; the
 * host takes and passes them. */
void CPathTrack__StartAtNearestPoint(CPathTrack *arg0, s8 arg1, struct CVector3 *pt) {
    arg0->m_Mode = 2;
    arg0->m_nCurrentPathIndex = arg1;
#ifdef TARGET_PORT
    arg0->m_nCurrentPointIndex = CScene__FindNearestPathPoint(&gScene, arg1, pt);
#else
    arg0->m_nCurrentPointIndex = CScene__FindNearestPathPoint(&gScene);
#endif
}

sptr CScene__GetPathPoint(s32 *, u8, s16);          /* extern */

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
sptr CPathTrack__GetCurrentPoint(CPathTrack *arg0) {
    return CScene__GetPathPoint(&gScene, arg0->m_nCurrentPathIndex, arg0->m_nCurrentPointIndex);
}
