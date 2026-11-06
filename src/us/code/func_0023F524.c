/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/pickup.h"

extern char D_801198B0[];

void Sound__Play2D(s32 sound);
void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, s32 arg4, s32 arg5);
void Sound__PlaySpeech(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(D_801198B0, player->m_pCamera, resource);
    }
    if (sound != 0) {
        Sound__Play(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
}

/* Life force pickup: every 100 gives an extra life, up to 9. */
s32 Pickup_GiveLifeForce(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;

    player->m_Data.m_LifeForce += entry->m_Params.m_Amount;
    if (player->m_Data.m_LifeForce >= 100) {
        player->m_Data.m_LifeForce -= 100;
        if (player->m_Data.m_Lives < 9) {
            player->m_Data.m_Lives++;
            Sound__Play2D(0x3A9D);
        }
    }
    sound = entry->m_Sound;
    callback = entry->m_Callback;
    resource = entry->m_pMessage;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}
