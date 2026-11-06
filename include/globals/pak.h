#ifndef GLOBALS_PAK_H
#define GLOBALS_PAK_H

/* The controller paks (structs/pak.h) and the pak menus' state. */

#include "structs/pak.h"
#include "common.h"
#include "ultra_types.h"
#include "structs/cache.h"

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

/* The controller pak menus, the save buffers and the pak driver's state. */

extern CMemEntry *gSaveBufferHandle; /* the CCache handle of gSaveBuffer (structs/save.h) */
extern CMemEntry *gLoadBufferHandle; /* the CCache handle of gLoadBuffer */
extern s32 gDamagedPakContinued;    /* Continue picked on the damaged-pak dialog */
extern s32 gPakMenuMode;            /* which notes the pak menu lists */
extern s32 gPakMenuPort;            /* the controller port the pak menus work on */
extern s32 sPakMenuStartLatched;    /* START seen once */
extern s32 gPakDirectoryAllocated;
extern CMemEntry *gPakStateBuffer;   /* the CCache handle of the 0x810-byte directory block (gPakDirectories) */
extern u8 gPakCharTable[];          /* the controller pak character set */
extern s8 gPakQueueState;           /* the pak rescan request / queue state */
extern OSMesgQueue gPakAccessQueue; /* the pak access lock */
extern OSMesg gPakMesgBuf[1];       /* gPakAccessQueue's buffer */
extern u8 gPakWatched[4];
extern s32 gPakInserted[4];
extern s32 gPakChanged[4];
extern s32 gPakRemoved[4];

/* Controller pak note identity */
extern char gPakCharacterNoteName[12];    /* "T2 MULTI" */
extern char gPakGameNoteName[8];          /* "TUROK 2" */
extern char gPakCompanyCode[4];           /* "51" */
extern char gPakGameCode[8];              /* "NT2E" */
/* Fields of gPakProfile with symbols of their own (CharNoteMenu_SelectYes_2 reads them through its own views) */
extern u16 gPakProfile_deaths;          /* gPakProfile.deaths */
extern u16 gPakProfile_kills;           /* gPakProfile.kills */
extern u16 gPakProfile_tags;            /* gPakProfile.tags */
extern u8 gPakProfile_character;        /* gPakProfile.character */
extern u8 gPakProfile_name[8];          /* gPakProfile.name */

#endif
