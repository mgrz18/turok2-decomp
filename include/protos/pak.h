/* the controller pak driver (vm1/Pak) and the text helpers the pak menus call.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_PAK_H
#define PROTOS_PAK_H

#include "common.h"

s32 Controller_PakChanged(s32);
s32 Controller_TakePakChanged(s32);
void Controller_UnwatchPak(sptr);
void Controller_WatchPak(s32);
u32 Crc32_Compute(u8 *, u32, u32);
void Pak_AllocBuffer(void ***, void **, s32);
void Pak_AllocDirectory(void);
u32 Pak_BytesToPages(sptr);
void Pak_ClearDamagedContinued(void);
s32 Pak_CountEmptyNotes(s32, s32 *);
void Pak_DecodeText(u8 *, u8 *, s32);
s32 Pak_DeleteNote(s32, s32);
void Pak_EncodeText(u8 *, u8 *, s32);
s32 Pak_FindFreeExtLetter(s32, u8 *);
s32 Pak_FindNote(s32, s32, s32 *, u8 *, u8 *, s32 *, u8 *, u8 *);
void Pak_FreeDirectory(void);
void Pak_FreeSaveBuffers(void);
char *Pak_GetBufferSize(s32);
int Pak_GetFreePages(int, int *);
s32 Pak_GetNoteExtName(s32, s32, u8 **);
s32 Pak_GetNoteGameName(s32, s32, char **);
s32 Pak_GetNotePages(s32, s32, s32 *);
s32 Pak_GetStatus(s32);
s32 Pak_IsDirectoryAllocated(void);
s32 Pak_IsRumblePak(s32);
s32 Pak_IsTurok2Note(char *, char *);
s32 Pak_Mount(s32);
s32 Pak_NeedsRepair(s32);
void Pak_ProbeAll(void);
s32 Pak_ProbeRumble(s32);
void Pak_ProbeSlot(s32);
s32 Pak_ReadNote(s32, s32, void *, s32);
s32 Pak_RefreshNotes(s32);
s32 Pak_Repair(s32);
s32 Pak_RepairIfDamaged(s32);
s32 Pak_SaveNoteVerified(s32, s32, void *, u8 *, u8 *, u8 *);
void Pak_SetBufferSize(s32, s32);
s32 Pak_TextHasInvalidChar(u8 *, s32);
s32 Pak_WriteNote(s32, s32, void *, u8 *, s32 *, u8 *, u8 *, u8 *);
s32 Str_Compare(u8 *, u8 *);
u8 *Str_Copy(u8 *, u8 *);
s32 Str_Length(u8 *);

#endif
