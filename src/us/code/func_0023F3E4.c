/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/pickup.h"
#include "globals/game.h"


extern char gEngineApp_m_Game_m_CameraPool[];

void CCamera__StartFlash(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void CCameraPool__AddText(void *arg0, void *ui, void *resource);
s32 Sound__Play(s32 sound, CVector3 position, s32 arg4, s32 arg5);
void Sound__PlaySpeech(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    CCamera__StartFlash(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        CCameraPool__AddText(gEngineApp_m_Game_m_CameraPool, player->m_pCamera, resource);
    }
    if (sound != 0) {
        Sound__Play(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        Sound__PlaySpeech(callback);
    }
}

/* Health pickup: add the entry's amount, up to its cap (in multiplayer, up
 * to the player's own maximum). */
s32 Pickup_GiveHealth(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 max;
    u32 limit;

    max = entry->m_Params.m_Health.m_Max << 8;
    result = 0;
    if (player->m_Data.m_Health < max) {
        player->m_Data.m_Health += entry->m_Params.m_Health.m_Amount << 8;
        do { /* a block boundary keeps the flag load after the store */
            if (gEngineApp_m_Settings.multiplayer) {
                limit = player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
                player->m_CharacterActor.m_AI.m_Health = player->m_Data.m_Health = (u32)player->m_Data.m_Health < limit ? (u32)player->m_Data.m_Health : limit;
            } else if (player->m_Data.m_Health > max) {
                player->m_Data.m_Health = max;
            }
        } while (0);
        result = 1;
    }
    if (result != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        resource = entry->m_pMessage;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}
