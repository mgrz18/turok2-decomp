#include "common.h"
#include "structs/vector.h"
#include "structs/region.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 pos;
} HitSource;

/* The camera pool gEngineApp_m_Game_m_CameraPool (CCameraPool) seen to the settings gEngineApp_m_Settings
 * (+0x1228): the ROM reads them through this symbol, and through gEngineApp_m_Settings
 * the code no longer matches. */
extern CGameFromCameraPool gEngineApp_m_Game_m_CameraPool; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 gScene_m_WarpState;
extern u8 gScene[];         /* the level */
extern char D_800B66D0[];

s32 Cinema_IsActive(void);
s32 CPlayer__IsInMatchEndState(CPlayer *p);
CRegionInfo *CScene__GetRegionInfo(void *level, void *region);
void CPlayer__Kill(CPlayer *p, HitSource *src, s32 arg3, s32 type);
void CCameraPool__AddText(CGameFromCameraPool *game, void *camera, char *text);
void Sound__PlaySpeech(s32 sound);
void CScene__StartRumbles(void *level, u8 *rumble, s32 id, CVector3 v, f32 f6, f32 f7);
void CCamera__SetHitFlash(void *camera, s32 alpha, s32 time);
void Sound__Play2D(s32 sound);
void Sound__Play(s16 sound, CVector3 pos, s32, s32);

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* Hurts a player: damage (8.8) scaled by the reduced-damage state and the
 * multiplayer handicap, unless the player is protected; then the red flash,
 * the low-health warning, the rumble and the hurt sound. */
void CPlayer__Hurt(CPlayer *p, HitSource *src, s32 damage, s32 arg3, s32 type) {
    s32 old;
    f32 f;
    s32 check;
    CRegionInfo *info;
    s32 prev;
    s32 health;
    s32 dead;
    s32 wasDead;
    s32 alpha;
    s32 time;
    s32 amount;
    CVector3 zero;

    f = 1.0f;
    old = p->m_Data.m_Health;
    if (Cinema_IsActive()) {
        return;
    }
    if (CPlayer__IsInMatchEndState(p)) {
        return;
    }
    if (gScene_m_WarpState) {
        return;
    }
    if ((gEngineApp_m_Game_m_CameraPool.m_Settings.cheatFlags & 1) && type != 0x1D) {
        return;
    }
    if ((u16)p->m_Mode < 2) {
        return;
    }
    if (p->m_pOnEnemy) {
        damage /= 5;
    }
    if (gEngineApp_m_Game_m_CameraPool.m_Settings.multiplayer) {
        if (p->m_pOptions->m_Handicap) {
            damage = damage * (1.0f / 256) / ((p->m_pOptions->m_Handicap + f) * 0.3f) * 256.0f;
        }
    }
    if (gEngineApp_m_Game_m_CameraPool.m_Settings.cheatFlags & 0x10) {
        check = 1;
    } else {
        check = p->m_Data.m_SpecialPickupCount[384] & 0xF;
    }
    if (check && p->m_CharacterActor.header.m_pCurrentRegion != NULL) {
        info = CScene__GetRegionInfo(gScene, ((void *)p->m_CharacterActor.header.m_pCurrentRegion));
        if (info != NULL && (info->m_dwFlags & 0x400000)) {
            return;
        }
    }
    if (p->m_InvincibilityTime > 0.0f && type != 0x1D) {
        return;
    }
    prev = p->m_Data.m_Health;
    wasDead = prev == 0;
    health = prev - damage;
    if (health < 0) {
        health = 0;
    }
    p->m_CharacterActor.m_AI.m_Health = (p->m_Data.m_Health = health);
    p->m_WeaponActor.m_AI.m_Health = p->m_Data.m_Health;
    dead = p->m_Data.m_Health == 0;
    if (dead) {
        if (!wasDead) {
            CPlayer__Kill(p, src, arg3, type);
        }
        alpha = 0xFF;
        time = 0xC;
    } else {
        alpha = ((damage >> 8) * 0xFF) / 20;
        alpha = (MIN(alpha, 0xFF) < 0x80) ? 0x80 : MIN(alpha, 0xFF);
        time = 0x1E;
        if (old > 0xF00 && p->m_Data.m_Health <= 0xF00 && !gEngineApp_m_Game_m_CameraPool.m_Settings.multiplayer) {
            CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, ((void *)p->m_pCamera), D_800B66D0);
            Sound__PlaySpeech(0x156);
        }
        amount = damage >> 8;
        if (amount != 0) {
            f = MIN(amount * (1.0f / 15), 1.0f);
            zero.x = 0;
            zero.y = 0;
            zero.z = 0;
            CScene__StartRumbles(gScene, p->m_ContState.m_pContState->rumblePool, 0x11, zero, 51.199997f, f);
        }
    }
    if (!dead && (s16) (u16)p->m_Mode != 0xF) {
        CCamera__SetHitFlash(((void *)p->m_pCamera), alpha & 0xFF, time);
    }
    if ((old >> 8) != (p->m_Data.m_Health >> 8)) {
        if (gEngineApp_m_Game_m_CameraPool.m_Settings.multiplayer) {
            if (p->m_pOptions->m_TagMode == 1) {
                Sound__Play2D(0x27DA);
            }
        } else if (!(p->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
            Sound__Play(0x3AC0, src->pos, 0, -1);
        }
    }
}
