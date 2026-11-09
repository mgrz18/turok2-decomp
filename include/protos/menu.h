/* menu pool.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_MENU_H
#define PROTOS_MENU_H

#include "common.h"

struct Menu;
struct MenuDesc;
struct MenuPool;

sptr MenuPool_OpenChildMenu(struct MenuPool *, struct Menu *, struct MenuDesc *);
sptr MenuPool_OpenMenu(struct MenuPool *, struct MenuDesc *, sptr, sptr, sptr);
s32 Menu_StepValue(struct Menu *, s32, s32, s32, s32, s32);

#endif
