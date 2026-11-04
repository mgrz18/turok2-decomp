/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de (the pickup feedback), with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

/* Pickup effect descriptor. */
typedef struct PickupEntry {
    void *resource; /* 0x00 */
    char pad4[2];
    s16 sound;      /* 0x06 */
    s16 callback;   /* 0x08 */
    char padA[2];
    s32 amount;     /* 0x0C */
    s32 max;        /* 0x10 */
} PickupEntry;

extern u8 D_8011AAF1;

extern char D_801198B0[];

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

/* Health pickup: add the entry's amount, up to its cap (in multiplayer, up
 * to the player's own maximum). */
s32 func_0023F3E4(CPlayer *player, PickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;
    s32 result;
    s32 max;
    u32 limit;

    max = entry->max << 8;
    result = 0;
    if (player->m_Data.m_Health < max) {
        player->m_Data.m_Health += entry->amount << 8;
        do { /* a block boundary keeps the flag load after the store */
            if (D_8011AAF1) {
                limit = player->m_CharacterActor.header.m_pIntelligence->m_Player.m_StartHealth << 8;
                player->m_CharacterActor.m_AI.m_Health = player->m_Data.m_Health = (u32)player->m_Data.m_Health < limit ? (u32)player->m_Data.m_Health : limit;
            } else if (player->m_Data.m_Health > max) {
                player->m_Data.m_Health = max;
            }
        } while (0);
        result = 1;
    }
    if (result != 0) {
        sound = entry->sound;
        callback = entry->callback;
        resource = entry->resource;
        pickup_feedback(player, resource, sound, callback);
    }
    return result;
}
