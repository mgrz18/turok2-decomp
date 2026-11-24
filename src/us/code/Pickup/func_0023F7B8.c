/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "globals/camera.h"


s32 CPlayer__AddAmmo(CPlayer *player, s32 index, s32 argument);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s16 sound, CVector3 position, sptr arg4, sptr arg5);
void Sound__PlaySpeech(s32 callback);

/* Flashes the player's screen and plays a pickup's optional resource, sound and callback. */
void Pickup_Feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
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
