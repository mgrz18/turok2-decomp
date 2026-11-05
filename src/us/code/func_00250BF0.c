#include "common.h"
#include "structs/game.h"
#include "structs/vector.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x22];
    /* 0x22 */ s16 strokeSound;
} Tuning;

typedef struct {
    /* 0x000 */ u8 pad0[0x114];
    /* 0x114 */ CVector3 pos;
} Ui;

extern f32 D_800B6D28;      /* frame time */
extern char D_800ADF00;     /* swimming look tuning */
extern Tuning D_800ADEDC;   /* swimming move tuning */

void func_0024EF70(CPlayer *player, CGameObjectInstance *body, void *tuning);
void func_0024F658(CPlayer *player, CGameObjectInstance *body, void *tuning);
s32 func_0024E5F0(CPlayer *player, CGameObjectInstance *body, s32 mode);
f32 func_002982F0(f32 x);
f32 func_0021149C(f32 scale);
void func_00275544(s16 sound, CVector3 pos, void *source, s32);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

/* Swimming: sway, move with the swimming tuning, and on surfacing (after two
 * time units) gasp for air and leave the state for falling, wading or
 * walking; then pick the animation from the climb and play strokes. */
void func_00250BF0(CPlayer *player, CGameObjectInstance *body) {
    s32 idle;

    if (player->m_TotalSpeed == 0.0f) {
        player->m_Mode = 0xA;
    } else {
        player->m_Mode = 0xB;
    }
    func_0024EF70(player, body, &D_800ADF00);
    player->m_vHeadWobbleRotOffset.x = func_002982F0(player->m_ModeTime * 0.06981318f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.y = func_002982F0(player->m_ModeTime * 0.087266475f) * 2 * 0.017453294f;
    player->m_vHeadWobbleRotOffset.z = func_002982F0(player->m_ModeTime * 0.104719765f) * 2 * 0.017453294f;
    func_0024F658(player, body, &D_800ADEDC);
    if (!(body->m_AI.m_dwFlags & 0x80) && 2.0f < player->m_ModeTime) {
        reset_strokes(&player->m_Barrel);
        body->m_vVelocity.y = 0.0f;
        if (!D_8011AAD8.multiplayer) {
            if (player->m_bOxygenOut) {
                func_00275544(0x3AC3, player->m_CharacterActor.header.m_vPos, &player->m_CharacterActor.header.m_vPos, -1);
            } else if (52.5f < player->m_Oxygen) {
                func_00275544(0x3AC2, ((Ui *)player->m_pCamera)->pos, &((Ui *)player->m_pCamera)->pos, -1);
            }
        }
        if (body->m_AI.m_dwFlags & 0xC0000) {
            func_0024E5F0(player, body, 2);
        } else if (body->m_AI.m_dwFlags & 0x200) {
            func_0024E5F0(player, body, 9);
        } else {
            func_0024E5F0(player, body, 3);
        }
    }
    idle = 0;
    if (player->m_CharAnim == 0x1144) {
        idle = (s8)player->m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted == 0;
    }
    if (!idle) {
        if (1.024f <= player->m_Speed) {
            player->m_CharAnim = 0x8A2;
        } else if (player->m_Speed <= -1.024f) {
            player->m_CharAnim = 0x8A7;
        } else {
            player->m_CharAnim = 0x14;
        }
    }
    player->m_SfxTimer += D_800B6D28;
    if (5.12f < player->m_TotalSpeed && 45.0f < player->m_SfxTimer) {
        player->m_SfxTimer = func_0021149C(15.0f);
        func_00275544(D_800ADEDC.strokeSound, body->header.m_vPos, NULL, -1);
    }
}
