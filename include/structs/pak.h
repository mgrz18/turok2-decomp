#ifndef STRUCTS_PAK_H
#define STRUCTS_PAK_H

/* The Controller Pak menus: the game keeps one OSPfs per channel
 * (gPakFileSystems) and a cached copy of each pak's note directory
 * (gPakDirectories: free space and the osPfsFileState of its 16 notes),
 * refreshed by Pak_RefreshNotes. The libultra side (OSPfs, OSPfsState,
 * __OSDir...) is in ultra_types.h. The pak menus are Menu windows
 * (structs/menu.h).
 * tools/check_structs.py checks every offset below with the game's compiler. */

#include "common.h"
#include "ultra_types.h"

/* One pak's cached directory; gPakDirectories points to 4 of them (one per
 * channel, 0x810 bytes allocated by Pak_AllocDirectory_2). */
typedef struct PakDirectory {
    /* 0x000 */ s32 freeBytes;      /* what osPfsFreeBlocks returned */
    /* 0x004 */ OSPfsState notes[16];
} PakDirectory; /* size 0x204 */

/* A channel's Rumble Pak: what Pak_ProbeSlot probes into gRumblePaks[4]
 * (the pak states gPakState follow it). */
typedef struct RumblePak {
    /* 0x00 */ u8 present;          /* osMotorInit succeeded */
    /* 0x01 */ u8 _pad1[7];
    /* 0x08 */ OSPfs pfs;
} RumblePak; /* size 0x70 */

/* The multiplayer profile kept in a "T2 MULTI" note (0x18 bytes): a
 * player's counts, character and name, copied from and back to his
 * CPlayerOptions. gPakProfile is the one being saved or loaded, gPakProfileRead
 * the last one read back with a good checksum. */
typedef struct PakProfile {
    /* 0x00 */ u16 suicides;
    /* 0x02 */ u16 deaths;
    /* 0x04 */ u16 kills;
    /* 0x06 */ u16 tags;
    /* 0x08 */ u8 character;
    /* 0x09 */ u8 reserved[3];      /* cleared before a save */
    /* 0x0C */ u8 name[8];
    /* 0x14 */ u32 checksum;        /* Crc32_Compute over the first 0x14 bytes */
} PakProfile; /* size 0x18 */

/* The free-space query of the pak save menus (PakMenu_WriteSaveGame,
 * PakMenu_WriteSaveGame_2), on the stack. */
typedef struct PakQuery {
    /* 0x00 */ s32 channel;
    /* 0x04 */ s32 freeBytes;       /* Pak_GetFreePages */
    /* 0x08 */ s32 freeNotes;       /* Pak_CountEmptyNotes */
} PakQuery; /* size 0xC */

extern RumblePak gRumblePaks[];      /* per channel: its Rumble Pak */
extern s32 gPakState[];            /* per channel: the pak state (3: mounted, directory read) */
extern s32 gPakError[];            /* per channel: the last pak error (0: none) */
extern OSPfs gPakFileSystems[];          /* per channel: the pak's file system */
extern PakDirectory *gPakDirectories;    /* per channel: its cached directory */

#endif
