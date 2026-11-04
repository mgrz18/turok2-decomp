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
    s16 amount;     /* 0x0C */
} PickupEntry;

extern char D_801198B0[];

void func_00275624(s32 sound);
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

/* Life force pickup: every 100 gives an extra life, up to 9. */
s32 func_0023F524(CPlayer *player, PickupEntry *entry) {
    void *resource;
    s32 sound;
    s32 callback;

    player->m_Data.m_LifeForce += entry->amount;
    if (player->m_Data.m_LifeForce >= 100) {
        player->m_Data.m_LifeForce -= 100;
        if (player->m_Data.m_Lives < 9) {
            player->m_Data.m_Lives++;
            func_00275624(0x3A9D);
        }
    }
    sound = entry->sound;
    callback = entry->callback;
    resource = entry->resource;
    pickup_feedback(player, resource, sound, callback);
    return 1;
}
