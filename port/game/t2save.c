/* t2save.c: the Controller Pak notes in the console's byte order (round 8, w5; docs/port/INPUT.md
 * section 7).
 *
 * Turok 2 writes two kinds of notes with osPfsReadWriteFile: the single-player save game ("TUROK 2",
 * structs/save.h SaveGame: header, CPlayerLoadSaveData, the first 0x184 bytes of the game settings,
 * the level's load/save persistent bit sections, a Crc32_Compute word) and the multiplayer profile
 * ("T2 MULTI", structs/pak.h PakProfile, 0x18 bytes). The game builds and checks them with native
 * loads and stores, so on a little-endian host they would land on the pak in host order and a pak
 * written by the port could not be read by an emulator or a console, nor the other way round.
 *
 * The note data on the pak is kept big-endian, as the console writes it:
 *   - Pak_SaveNoteVerified (the only writer of notes) writes T2Save_ToPak's image: the record
 *     swapped field by field with the plans below, its checksum recomputed over the big-endian
 *     bytes and stored big-endian (the console computes it over what it writes);
 *   - Pak_ReadNote hands the bytes to T2Save_FromPak: a record whose big-endian checksum holds is
 *     swapped to host order and its checksum recomputed in host order, so the game's own checks
 *     (PakMenu_SelectNote) and readers (SaveGame_Restore, GameNoteMenu_SelectYes, the GameNoteMenu
 *     items) run unchanged. A note whose host-order checksum holds (written by a port build before
 *     this change) is taken as it is; anything else is left alone, and the game calls it damaged.
 * Nothing changes on a big-endian host (the swaps are skipped).
 *
 * The level flags are the "load/save persistent bit sections" unit (tools/assets/swap_spec.json):
 * the archive swapped its ISet headers and bit counts to host order when the level loaded, so the
 * same walk swaps them back here (the flag bits are bytes). */
#include "common.h"
#include "structs/save.h"
#include "structs/pak.h"

#include <stdlib.h>
#include <string.h>

u32 Crc32_Compute(u8 *data, u32 len, u32 seed);

/* the offsets the plans use (structs/save.h, structs/player.h, structs/game.h, structs/pak.h) */
typedef char t2save_check_save[(sizeof(SaveGame) == 0x688 && __builtin_offsetof(SaveGame, player) == 0x18 &&
                                __builtin_offsetof(SaveGame, settings) == 0x500 &&
                                __builtin_offsetof(SaveGame, levelFlags) == 0x684) ? 1 : -1];
typedef char t2save_check_player[(sizeof(CPlayerLoadSaveData) == 0x4E8 &&
                                  __builtin_offsetof(CPlayerLoadSaveData, m_AmmoList) == 0x414 &&
                                  __builtin_offsetof(CPlayerLoadSaveData, m_NukePartLevelFlags) == 0x4DC) ? 1 : -1];
typedef char t2save_check_profile[(sizeof(PakProfile) == 0x18) ? 1 : -1];

#define SAVE_HEADER 0x688 /* SaveGame up to the level flags, plus the checksum word */

static int T2Save_Swapping(void) {
    const u16 one = 1;
    return *(const u8 *) &one == 1;
}

static void swapN(u8 *p, int width, int count) {
    int i;
    u8 t;

    for (i = 0; i < count; i++, p += width) {
        if (width == 2) {
            t = p[0]; p[0] = p[1]; p[1] = t;
        } else if (width == 4) {
            t = p[0]; p[0] = p[3]; p[3] = t;
            t = p[1]; p[1] = p[2]; p[2] = t;
        }
    }
}

static u32 rd32(const u8 *p, int big) {
    return big ? ((u32) p[0] << 24) | ((u32) p[1] << 16) | ((u32) p[2] << 8) | p[3]
               : ((u32) p[3] << 24) | ((u32) p[2] << 16) | ((u32) p[1] << 8) | p[0];
}

static void wr32(u8 *p, u32 v, int big) {
    if (big) {
        p[0] = v >> 24; p[1] = v >> 16; p[2] = v >> 8; p[3] = v;
    } else {
        p[3] = v >> 24; p[2] = v >> 16; p[1] = v >> 8; p[0] = v;
    }
}

/* {offset, width, count}: the fields wider than a byte. */
typedef struct { u16 off; u8 width; u8 count; } T2SwapRun;

static const T2SwapRun sSaveHeader[] = {
    { 0x000, 4, 6 },            /* size, levelFlagsSize, the play clock (4 f32) */
};
static const T2SwapRun sPlayerData[] = {   /* CPlayerLoadSaveData, at 0x018 */
    { 0x000, 4, 2 },            /* m_nDeathmatchCharacter, m_Health */
    { 0x008, 2, 2 },            /* m_LifeForce, m_Lives */
    { 0x40C, 4, 2 },            /* m_CheckPointWarpID, m_SavePointWarpID */
    { 0x414, 2, 31 },           /* m_AmmoList */
    { 0x476, 2, 1 },            /* m_CurrentWeapon */
    { 0x478, 2, 3 },            /* m_LastWeapon */
    { 0x480, 4, 6 },            /* m_AdonAmmoFlagHelped */
    { 0x498, 4, 6 },            /* m_AdonHealthFlagHelped */
    { 0x4B0, 4, 11 },           /* the totem and boss flags, m_LevelIntroPlayedFlags, m_dwLevelFirstTimeFlags */
};
static const T2SwapRun sSettings[] = {     /* GameSettingsBlock, at 0x500 (structs/game.h GameSettings) */
    { 0x000, 4, 4 },            /* cheatFlags, unlockedCheats, difficulty, unkC */
    { 0x010, 2, 1 },            /* hudAlphaSetting */
    { 0x01C, 4, 1 },            /* speed */
};
static const T2SwapRun sOptions[] = {      /* CPlayerOptions (x5 from GameSettings + 0x40) */
    { 0x000, 2, 20 },           /* the counts */
};

static void applyRuns(u8 *base, const T2SwapRun *r, int n) {
    int i;

    for (i = 0; i < n; i++) {
        swapN(base + r[i].off, r[i].width, r[i].count);
    }
}

/* The persistent bit sections (swap_spec.json "load/save persistent bit sections"):
 *   top = ISet [ ISet each LevelBits, BitSection ]; LevelBits = ISet each BitSection;
 *   BitSection = ISet [ u32 bitCount + 4 pad, the flag bytes ].
 * An ISet: u32 n, u32 offsets[n + 1] (from the ISet's start), the entries. `big` says the order
 * the bytes are in now (the counts are read before the swap). Returns 0, or -1 if malformed
 * (then some of it may be swapped: the caller works on a copy). */
enum { KIND_TOP, KIND_LEVELS, KIND_LEVEL, KIND_SECTION };

static int swapISet(u8 *p, u32 size, int kind, int big) {
    u32 n;
    u32 i;
    u32 a;
    u32 b;
    int sub;

    if (size < 8) {
        return -1;
    }
    n = rd32(p, big);
    if (n > 0x10000 || 8 + 4 * n > size) {
        return -1;
    }
    for (i = 0; i < n; i++) {
        a = rd32(p + 4 + 4 * i, big);
        b = rd32(p + 8 + 4 * i, big);
        if (a < 8 + 4 * n || b < a || b > size) {
            return -1;
        }
        switch (kind) {
            case KIND_TOP: sub = (i == 0) ? KIND_LEVELS : (i == 1) ? KIND_SECTION : -1; break;
            case KIND_LEVELS: sub = KIND_LEVEL; break;
            case KIND_LEVEL: sub = KIND_SECTION; break;
            default: sub = (i == 0) ? 99 : -1; break; /* the section's bit count; then bytes */
        }
        if (sub == 99) {
            if (b - a >= 4) {
                swapN(p + a, 4, 1);
            }
        } else if (sub >= 0 && b > a) {
            if (swapISet(p + a, b - a, sub, big) != 0) {
                return -1;
            }
        }
    }
    swapN(p, 4, 2 + n);
    return 0;
}

/* Swaps a whole save record (size bytes, flagsSize of level flags) between the orders. */
static void swapSave(u8 *p, u32 flagsSize, int big) {
    int i;

    applyRuns(p, sSaveHeader, 1);
    applyRuns(p + 0x018, sPlayerData, sizeof(sPlayerData) / sizeof(sPlayerData[0]));
    applyRuns(p + 0x500, sSettings, sizeof(sSettings) / sizeof(sSettings[0]));
    for (i = 0; i < 5; i++) {
        applyRuns(p + 0x500 + 0x40 + i * 0x40, sOptions, 1);
    }
    if (flagsSize >= 8) {
        u8 *copy = (u8 *) malloc(flagsSize);
        if (copy != NULL) {
            memcpy(copy, p + 0x684, flagsSize);
            if (swapISet(copy, flagsSize, KIND_TOP, big) == 0) {
                memcpy(p + 0x684, copy, flagsSize);
            }
            free(copy);
        }
    }
}

static int checkSave(u8 *p, u32 avail, int big, u32 *total) {
    u32 t = rd32(p, big);
    u32 f = rd32(p + 4, big);

    if (t < SAVE_HEADER || t > avail || f != t - SAVE_HEADER) {
        return 0;
    }
    *total = t;
    return Crc32_Compute(p, t - 4, 0xB) == rd32(p + t - 4, big);
}

static void swapProfile(u8 *p) {
    swapN(p, 2, 4); /* suicides, deaths, kills, tags */
}

/* The note image Pak_SaveNoteVerified writes for `data` (size bytes): big-endian with its checksum
 * over the big-endian bytes. Returns data itself when nothing is swapped (a big-endian host, or a
 * note that is not one of the two records). The image lives until the next call. */
void *T2Save_ToPak(void *data, s32 size) {
    static u8 *sImage;
    static s32 sCap;
    u8 *p = (u8 *) data;
    u32 total;

    if (!T2Save_Swapping() || size <= 0) {
        return data;
    }
    if (size > sCap) {
        free(sImage);
        sImage = (u8 *) malloc(size);
        sCap = (sImage != NULL) ? size : 0;
        if (sImage == NULL) {
            return data;
        }
    }
    if (size == (s32) sizeof(PakProfile)) {
        memcpy(sImage, p, size);
        swapProfile(sImage);
        wr32(sImage + 0x14, Crc32_Compute(sImage, 0x14, 4), 1);
        return sImage;
    }
    if (size >= SAVE_HEADER && checkSave(p, size, 0, &total) && total == (u32) size) {
        memcpy(sImage, p, size);
        swapSave(sImage, total - SAVE_HEADER, 0);
        wr32(sImage + total - 4, Crc32_Compute(sImage, total - 4, 0xB), 1);
        return sImage;
    }
    return data;
}

/* After Pak_ReadNote: dst holds size bytes of a note as the pak has them; turns a big-endian
 * record into host order (see the top of the file). */
void T2Save_FromPak(void *dst, s32 size) {
    u8 *p = (u8 *) dst;
    u32 total;

    if (!T2Save_Swapping() || size <= 0) {
        return;
    }
    if (size == (s32) sizeof(PakProfile)) {
        if (Crc32_Compute(p, 0x14, 4) == rd32(p + 0x14, 1)) {
            swapProfile(p);
            wr32(p + 0x14, Crc32_Compute(p, 0x14, 4), 0);
        }
        return;
    }
    if (size >= SAVE_HEADER && checkSave(p, size, 1, &total)) {
        swapSave(p, total - SAVE_HEADER, 1);
        wr32(p + total - 4, Crc32_Compute(p, total - 4, 0xB), 0);
    }
}
