/* The pak menus' own functions that other files of the module call (vm1/PakMenu; the definitions'
 * prototypes). Callers that hold the menu through another view cast it to Menu *. */
#ifndef PROTOS_PAKMENU_H
#define PROTOS_PAKMENU_H

#include "common.h"

struct Menu;

void PakMenu_UpdateNoteRows(struct Menu *menu);
void *PakMenu_GetReturnMenu(struct Menu *c);
s32 PakMenu_CheckPak(struct Menu *c, s32 slot, s32 quiet);
s32 PakMenu_WriteSaveGame(s32 unused, struct Menu *own);
s32 PakMenu_WriteProfile(sptr unused, struct Menu *own, s32 unused3);

#endif
