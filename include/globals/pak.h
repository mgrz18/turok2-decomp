#ifndef GLOBALS_PAK_H
#define GLOBALS_PAK_H

/* The controller paks (structs/pak.h) and the pak menus' state. */

#include "structs/pak.h"

/* Per channel (4 controllers). */
extern RumblePak gRumblePaks[4];      /* its Rumble Pak */
extern s32 gPakState[4];              /* the pak state (3: mounted, directory read) */
extern s32 gPakError[4];              /* the last pak error (0: none) */
extern OSPfs gPakFileSystems[4];      /* the pak's file system */

/* The boot-time pak check. */
extern s32 gPakCheckHasPak;
extern s32 gPakCheckDone;

/* The profiles: gPakProfile is the one being saved or loaded, gPakProfileRead
 * the last one read back with a good checksum. */
extern PakProfile gPakProfile;
extern PakProfile gPakProfileRead;

/* The pak menus' state. */
extern s32 gPakMenuStartPressed;
extern s32 sCharNoteLoad;
extern s32 sDamagedPakInGame;
extern s32 sSavePortalChosen;
extern s32 gPakMenuIsSave;
extern s32 gPakOverwriteNote;
extern s32 sPakMessageRumbleB;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuIsLoad;
extern s32 gSavePortalActive;
extern s32 sDamagedPakDone;
extern s32 gPakMenuWrites;
extern s32 sPakConfirmDone;
extern s32 gPakConfirmKind;
extern s32 gPakMenuIsProfile;
extern s32 sGameNoteLoad;
extern s32 gPakMenuLeaving;
extern s32 gPakMessageKind;
extern s32 gPakNoteIndex;

#endif
