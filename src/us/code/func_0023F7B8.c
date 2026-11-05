/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/802ABDBC_11.CPP, func_802ACB18_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

extern char D_801198B0[];

s32 func_00253B54(CPlayer *player, s32 index, s32 argument);
void func_00281060(void *ui, s32 red, s32 green, s32 blue, s32 alpha, s32 a5, s32 a6, s32 a7);
void func_0027FC2C(void *arg0, void *ui, void *resource);
s32 func_00275544(s16 sound, CVector3 position, s32 arg4, s32 arg5);
void func_0027580C(s32 callback);

/* Flashes the player's screen and plays a pickup's optional resource, sound and callback. */
void func_0023F7B8(CPlayer *player, void *resource, s32 sound, s32 callback) {
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
