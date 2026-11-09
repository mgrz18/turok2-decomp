/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/pickup.h"
#include "globals/camera.h"


s32 CPlayer__AddAmmo(CPlayer *player, s32 index, s32 argument);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, sptr arg4, s32 arg5);
void Sound__PlaySpeech(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(&gEngineApp_m_Game_m_CameraPool, player->m_pCamera, resource);
    }
    if (sound != 0) {
        Sound__Play(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
}

extern char D_800B3C30[];

s32 func_0023F21C(CPlayer *player, CPickupEntry *entry) {
    s32 sound;
    s32 callback;

    if (CPlayer__AddAmmo(player, 2, entry->m_Params.m_Amount) != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        pickup_feedback(player, D_800B3C30, sound, callback);
        return 1;
    }
    return 0;
}
