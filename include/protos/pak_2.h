/* The second VM module's copies (_2) of the pak calls the pak menus make (vm2/PakMenu): the prototypes of
 * protos/pak.h and protos/pakmenu.h with the _2 names, in their definitions' types (vm2/Pak, vm2/PakMenu,
 * vm2/MenuItem). Written by hand: tools/port/genprotos.py leaves the _2 copies out. */
#ifndef PROTOS_PAK_2_H
#define PROTOS_PAK_2_H

#include "common.h"

struct Menu;
struct MenuDesc;
struct MenuPool;
struct CScene;

u32 Crc32_Compute_2(u8 *, u32, u32);
void Pak_AllocBuffer_2(void ***, void **, s32);
void Pak_AllocDirectory_2(void);
u32 Pak_BytesToPages_2(s32);
void Pak_ClearDamagedContinued_2(void);
s32 Pak_CountEmptyNotes_2(s32, s32 *);
void Pak_DecodeText_2(u8 *, u8 *, s32);
s32 Pak_DeleteNote_2(s32, s32);
void Pak_EncodeText_2(u8 *, u8 *, s32);
s32 Pak_FindFreeExtLetter_2(s32, u8 *);
s32 Pak_FindNote_2(s32, s32, s32 *, u8 *, u8 *, s32 *, u8 *, u8 *);
void Pak_FreeDirectory_2(void);
void Pak_FreeSaveBuffers_2(void);
char *Pak_GetBufferSize_2(s32);
int Pak_GetFreePages_2(int, int *);
s32 Pak_GetNoteExtName_2(s32, s32, char **);
s32 Pak_GetNoteGameName_2(s32, s32, char **);
s32 Pak_GetNotePages_2(s32, s32, s32 *);
s32 Pak_GetStatus_2(s32);
s32 Pak_IsDirectoryAllocated_2(void);
s32 Pak_IsRumblePak_2(s32);
s32 Pak_IsTurok2Note_2(char *, char *);
s32 Pak_Mount_2(s32);
s32 Pak_NeedsRepair_2(s32);
void Pak_ProbeAll_2(void);
s32 Pak_ProbeRumble_2(s32);
void Pak_ProbeSlot_2(s32);
s32 Pak_ReadNote_2(s32, s32, void *, s32);
s32 Pak_RefreshNotes_2(s32);
s32 Pak_Repair_2(s32);
s32 Pak_RepairIfDamaged_2(s32);
s32 Pak_SaveNoteVerified_2(s32, s32, void *, u8 *, u8 *, u8 *);
void Pak_SetBufferSize_2(s32, s32);
s32 Pak_TextHasInvalidChar_2(u8 *, s32);
s32 Pak_WriteNote_2(s32, s32, void *, u8 *, s32 *, u8 *, u8 *, u8 *);
s32 Str_Compare_2(u8 *, u8 *);
u8 *Str_Copy_2(u8 *, u8 *);
s32 Str_Length_2(u8 *);

void PakMenu_UpdateNoteRows_2(struct Menu *menu);
void *PakMenu_GetReturnMenu_2(struct Menu *c);
s32 PakMenu_CheckPak_2(struct Menu *c, s32 slot, s32 quiet);
s32 PakMenu_WriteSaveGame_2(s32 unused, struct Menu *own);
s32 PakMenu_WriteProfile_2(sptr unused, struct Menu *own, s32 unused3);

sptr MenuPool_OpenMenu_2(struct MenuPool *, struct MenuDesc *, sptr, sptr, sptr);
sptr MenuPool_OpenChildMenu_2(struct MenuPool *, struct Menu *, struct MenuDesc *);
void CScene__RequestWarp_2(struct CScene *, s32, s32);

#endif
