#include "common.h"

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

extern u8 gScene[];
extern AnimDef *func_00226134(void *table, u8 id);

void CPathTrack__Advance(AnimPlayer *player) {
    AnimDef *anim;
    s32 count;

    anim = func_00226134(gScene, player->animId);
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
