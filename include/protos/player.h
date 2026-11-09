/* player pool / player state.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_PLAYER_H
#define PROTOS_PLAYER_H

#include "common.h"

struct CPlayer;

sptr CPlayerPool__GetFirstActive(void *);
uptr CPlayerPool__GetIndex(void *, uptr);
s32 CPlayer__SetState(struct CPlayer *, void *, s32);
void CPlayer__StartDeathCinema(struct CPlayer *, s32, s32);

#endif
