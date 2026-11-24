#ifndef PROTOS_WEAPON_MODE_H
#define PROTOS_WEAPON_MODE_H

/* The calls code/WeaponMode makes, one prototype each, as its callers compile in the ROM
 * (Sound__Play's sound is passed as an s32 here although Sound.c takes an s16). */

#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"

s32 Sound__Play(s32 sound, CVector3 position, CVector3 *source, sptr arg5);
void Sound__StopSoundId(s32 sound);
void CCameraPool__AddText(void *pool, void *camera, sptr text);
s32 CPlayer__FireWeapon(void *player, s32 type);
s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
void CPlayer__StartWeaponFiringSound(CPlayer *player, s32 id);
void CPlayer__StartWeaponBackgroundSound(CPlayer *player, s32 id);
void CMtxF__PreRotateY(void *mtx, f32 angle);

#endif
