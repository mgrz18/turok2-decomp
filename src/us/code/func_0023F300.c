/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/pickup.h"

extern char D_801198B0[];

s32 func_00253B54(CPlayer *player, s32 index, s32 argument);
void func_00281060(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void func_0027FC2C(void *arg0, void *ui, void *resource);
s32 func_00275544(s32 sound, CVector3 position, s32 arg4, s32 arg5);
void func_0027580C(s32 callback);

/* Flashes the screen and plays the descriptor's optional resource, sound and callback. */
static __inline__ void pickup_feedback(CPlayer *player, void *resource, s32 sound, s32 callback) {
    func_00281060(player->m_pCamera, 0x80, 0x32, 0x32, 0x4B, 0, 0, 2);
    if (resource != NULL) {
        func_0027FC2C(D_801198B0, player->m_pCamera, resource);
    }
    if (sound != 0) {
        func_00275544(sound, player->m_CharacterActor.header.m_vPos, 0, -1);
    }
    if (callback != 0) {
        func_0027580C(callback);
    }
}

s32 func_0023F300(CPlayer *player, CPickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;

    result = func_00253B54(player, 2, entry->m_Params.m_Amount);
    if (result != 0) {
        sound = entry->m_Sound;
        callback = entry->m_Callback;
        resource = entry->m_pMessage;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}
