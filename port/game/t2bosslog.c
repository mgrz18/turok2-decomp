/* t2bosslog.c: DEBUG-ONLY. T2_BOSS_LOG=1 (with T2_STATE_LOG, round 8 w3): the boss fight state of
 * the current level, to follow the fights headless (docs/port/GAMEPLAY.md section 9). Level 1 the
 * Blind One (gBlindOne), 2 the Mantid Queen (gMantidQueen), 3 the Mother (gMotherBoss), 4 the
 * Primagen (gLevelScript). Not a game option. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include <stdio.h>

static void logActor(const char *what, CGameObjectInstance *a) {
    if (a == NULL) {
        printf("boss:   %s none\n", what);
        return;
    }
    printf("boss:   %s %p hp %d/%d mode %d anim %d pos (%.1f %.1f %.1f) fl %x\n", what, (void *) a,
           (int) a->m_AI.m_Health, (int) a->m_AI.m_InitialHealth, (int) a->m_AI.m_Mode,
           (int) a->m_CurrentAnimation.m_nAnimIndex, a->header.m_vPos.x, a->header.m_vPos.y, a->header.m_vPos.z,
           (unsigned) a->m_dwFlags);
}

void T2Port_LogBosses(void) {
    int i;

    switch (gScene.m_nLevel) {
        case 1: {
            BlindOneState *s = &gBlindOne;
            printf("boss: blind one phase %d step %d counts %d/%d/%d alive %d/%d/%d minions %d/%d\n",
                   (int) s->phase, s->script ? (int) (s->script - sBlindOneScript) : -1, (int) s->count1,
                   (int) s->count2, (int) s->count3, (int) s->alive1, (int) s->alive2, (int) s->alive3,
                   (int) s->minionsAlive, (int) s->maxMinions);
            logActor("boss", s->boss);
            break;
        }
        case 2: {
            WaveBossState *s = &gMantidQueen;
            printf("boss: mantid queen step %d toSpawn %d timer %d members %d wave %d pushed %d spots",
                   (int) s->stepIndex, (int) s->toSpawn, (int) s->timer, (int) s->memberCount,
                   (int) s->waveActive, (int) s->pushed);
            for (i = 0; i < 5; i++) {
                printf(" %d", (int) s->spots[i].health);
            }
            printf("\n");
            logActor("boss", s->boss);
            logActor("owner", s->actor);
            break;
        }
        case 3: {
            MotherBossState *s = &gMotherBoss;
            printf("boss: mother phase %d health %d/%d parts %d/%d active %x live %x spots",
                   (int) s->phase, (int) s->health, (int) s->totalHealth, (int) s->partsLeft,
                   (int) s->partCount, (unsigned) s->activeMask, (unsigned) s->liveMask);
            for (i = 0; i < 15; i++) {
                printf(" %d", (int) s->slots[i].health);
            }
            printf("\n");
            logActor("boss", s->obj);
            break;
        }
        case 4: {
            LevelScriptState *s = &gLevelScript;
            printf("boss: primagen stage %d state %d mode %d parts %d/%d spawns %d waves %d alive %d "
                   "scripts A %d/%d B %d/%d C %d/%d\n",
                   (int) s->stage, (int) s->bossState, (int) s->bossMode, (int) s->partsDone,
                   (int) s->part, (int) s->spawnsLeft, (int) s->wavesLeft, (int) s->enemiesAlive,
                   (int) s->script.cursor, (int) s->script.current, (int) s->script2.cursor,
                   (int) s->script2.current, (int) s->script3.cursor, (int) s->script3.current);
            logActor("boss", s->actor);
            logActor("part0", s->found[0]);
            logActor("part1", s->found[1]);
            break;
        }
    }
}
