#include "common.h"
#include "structs/pak.h"
#include "globals/pak.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00404018.c, func_00404018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
#include "structs/controller.h"
#include "globals/controller.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC.CPP, func_804041F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
/* one function 0x00425BC0-0x00425E74 (size 0x2B4); splat cut its tail into func_00425D94. MATCHES with that size (twin of Pak_WriteNote_2). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_de, with Turok 2's layouts. */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_8040458C_de, with Turok 2's layouts. */
/* WRONG CUT: the real function is 0x00425E74-0x00426064 (size 0x1F0, 124 words); splat cut its tail (the two text-decoding loops and the
 * epilogue) off as func_00425F84 (0x00425F84, 0xE0, listed in part x4). All 124 words match (checked over the real extent).
 * First-module copy of src/us/code/Pak_FindNote_2.c. */
#include "common.h"
/* WRONG CUT: REAL Pak_DeleteNote size 0x100 (splat cut it into Pak_DeleteNote + func_00426084). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404858_de, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_5.c, func_80404D84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "protos/cache.h"
/* WRONG CUT: REAL Pak_FreeDirectory size 0x4C (splat cut its last word into func_0042647C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404DDC_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/cache.h"
/* WRONG CUT: REAL Pak_ProbeSlot size 0xDC (splat cut it into Pak_ProbeSlot + func_004264B4 + func_004264D0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_7.CPP, func_80404E28_de, with Turok 2's layouts. */
#include "common.h"
/* WRONG CUT: REAL Pak_ReadNote size 0x15C (splat cut it into Pak_ReadNote + func_0042662C + func_004266C0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_read, with Turok 2's layouts. */
#include "common.h"
#include "globals/menu_text.h"
/* WRONG CUT: REAL Pak_CountEmptyNotes size 0x94 (splat cut it into Pak_CountEmptyNotes + func_00426734). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_8.CPP, func_804050CC_de, with Turok 2's layouts. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_80405160_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804051D4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pak.h (round 29, part r2: gPakFileSystems as OSPfs, the host's size too). */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_80405290_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. Over
 * globals/pak.h (round 29, part r2: gPakFileSystems as OSPfs, the host's size too). */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: REAL Pak_FindFreeExtLetter size 0x144 (splat cut it into Pak_FindFreeExtLetter + func_00426AF8 + func_00426BA8). */
#include "common.h"
/* WRONG CUT: REAL Pak_ProbeRumble size 0x74 (splat cut it into Pak_ProbeRumble + func_00426C74). */
#include "common.h"
/* WRONG CUT: REAL Pak_DecodeText size 0x4C (splat cut it into Pak_DecodeText + func_00426CAC). */
#include "common.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC.CPP, func_804042F0_encode, with Turok 2's layouts. */
#include "common.h"
/* WRONG CUT: REAL Str_Compare size 0x38 (splat cut it into Str_Compare + func_00426D5C). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A0294_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "m2c_macros.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A08B8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: REAL Str_CatN size 0x58 (splat cut it into Str_CatN + func_00426F7C + func_00426F9C). */
#include "common.h"
/* WRONG CUT: REAL Str_CompareN size 0x48 (splat cut it into Str_CompareN + func_00426FC4). */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A03F4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8029F888_de_2.c, func_802A0444_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* WRONG CUT: REAL Pak_SaveNoteVerified size 0x278 (splat cut it into Pak_SaveNoteVerified + func_004263A0). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/PAK/80403BCC_6.CPP, func_80404958_de, with Turok 2's layouts. */
#include "common.h"
#include "globals/strings.h"

/* Refreshes the cached state of the 16 notes of a controller pak (osPfsFileState, retried once; an invalid
 * note is cleared) and its free space. */
extern s32 osPfsFileState(OSPfs *pfs, s32 fileNo, OSPfsState *state); /* osPfsFileState */
extern s32 osPfsFreeBlocks(OSPfs *pfs, s32 *bytes);

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
s32 Pak_RefreshNotes(s32 channel) {
    s32 i;
    s32 ret;
    s32 free;

    if (gPakDirectories != NULL) {
        for (i = 0; i < 16; i++) {
            ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            if (ret != 0 && ret != 5) {
                ret = osPfsFileState(&gPakFileSystems[channel], i, &gPakDirectories[channel].notes[i]);
            }
            if (ret == 5) {
                gPakDirectories[channel].notes[i].file_size = 0;
                gPakDirectories[channel].notes[i].game_code = 0;
                gPakDirectories[channel].notes[i].company_code = 0;
                gPakDirectories[channel].notes[i].ext_name[0] = 0;
                gPakDirectories[channel].notes[i].game_name[0] = 0;
            }
            if (osPfsFreeBlocks(&gPakFileSystems[channel], &free) == 0) {
                gPakDirectories[channel].freeBytes = free;
            }
        }
    }
    return osPfsFreeBlocks(&gPakFileSystems[channel], &free);
}

s32 osPfsInitPak(OSMesgQueue *queue, OSPfs *pfs, int channel);

s32 Pak_Mount(s32 pad) {
    s32 i;
    s32 r;
    s32 v;
    s32 code;
    s32 ok;
    s32 ret;
    if (gContStatus[pad].errno != 0) {
        gPakError[pad] = -2;
        ret = 0;
    } else {
        i = 0;
        do {
            r = osPfsInitPak(&gSiMessageQueue, &gPakFileSystems[pad], pad);
            switch (r) {
            case 0:
            case 2:
                gPakError[pad] = -5;
                break;
            case 1:
                gPakError[pad] = -2;
                break;
            case 4:
            case 11:
                code = -3;
                goto st;
            case 10:
                code = -4;
            st:
                gPakError[pad] = code;
                break;
            }
            if (gPakError[pad] == -5) {
                goto got5;
            }
            if (gPakError[pad] == -2) {
                goto fin;
            }
            i++;
        } while (i < 4);
        goto fin2;
    got5:
        gPakState[pad] = 3;
        if (Pak_RefreshNotes(pad) != 0) {
            gPakError[pad] = -4;
            goto fin2;
        }
        gPakError[pad] = 0;
    fin:
    fin2:
        v = gPakError[pad];
        ok = 0;
        if (v == 0 || v == -4) {
            ok = 1;
        }
        ret = ok;
    }
    return ret;
}

extern s32 Controller_Lock(s32 arg0);
extern void Controller_Unlock();
extern void Controller_ReadAll(void);
s32 osMotorInit(OSMesgQueue *mq, OSPfs *pfs, int channel);
extern void Controller_Unlock(void);

void Pak_ProbeAll(void) {
    s32 channel;
    s32 present;
    RumblePak *record;
    Controller_Lock(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    for (channel = 0; channel < 4; channel++) {
        record = &gRumblePaks[channel];
        if (gContStatus[channel].errno != 0) {
            present = 0;
            record->present = 0;
        } else {
            record->present = osMotorInit(&gSiMessageQueue, &record->pfs, channel) == 0;
            present = record->present;
        }
        if (present) gPakState[channel] = 2;
        else if (Pak_Mount(channel) != 0) gPakState[channel] = 3;
        else gPakState[channel] = 1;
    }
    Controller_Unlock();
}

extern void *func_0029E460(void *dst, const void *src, s32 size); /* memcpy */
/* osPfsAllocateFile, osPfsReadWriteFile, osPfsDeleteFile and the directory refresh, in the first module */
extern s32 osPfsAllocateFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name, s32 size, s32 *file_no);
extern s32 osPfsReadWriteFile(OSPfs *pfs, s32 file_no, u8 flag, s32 offset, s32 size, void *data);
extern s32 osPfsDeleteFile(OSPfs *pfs, u16 company_code, u32 game_code, u8 *game_name, u8 *ext_name);

static __inline__ s32 ToUpper(s32 c) {
    if (c - 'a' < 26U) {
        c -= 0x20;
    }
    return c;
}

/* Upper-cases src in place and converts it to pak font codes in dst (0 for a character it lacks). */
static __inline__ void EncodePakText(u8 *dst, u8 *src, s32 n) {
    s32 i;
    s32 j;

    for (i = 0; i < n; i++) {
        src[i] = ToUpper(src[i]);
        for (j = 0; j < 0x42; j++) {
            if (src[i] == gPakCharTable[j]) {
                dst[i] = j;
                break;
            }
        }
        if (j == 0x42) {
            dst[i] = 0;
        }
    }
}

/* Writes data as a note on the pak of channel ch; returns 0, -1 on failure or -2 if the pak is not ready. */
s32 Pak_WriteNote(s32 ch, s32 size, void *data, u8 *game_name, s32 *file_no, u8 *ext_name, u8 *company, u8 *code) {
    s32 result;
    u8 name[16];
    u8 ext[4];
    u16 company_code;
    u32 game_code;
    OSPfs *pfs;
    s32 offset;

    if (gPakState[ch] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        EncodePakText(ext, ext_name, 4);
        size = (size + 0xFF) & ~0xFF;
        func_0029E460(name, game_name, 16);
        EncodePakText(name, game_name, 16);
        pfs = &gPakFileSystems[ch];
        company_code = (company[0] << 8) | company[1];
        game_code = (code[0] << 24) | (code[1] << 16) | (code[2] << 8) | code[3];
        result = osPfsAllocateFile(pfs, company_code, game_code, name, ext, size, file_no);
        if (result != 0) {
            result = -1;
        }
        offset = 0;
        if (result == 0) {
            result = osPfsReadWriteFile(pfs, *file_no, 1, offset, size, data);
            if (result != 0) {
                osPfsDeleteFile(pfs, company_code, game_code, name, ext);
                result = -1;
            }
        }
        Pak_Mount(ch);
    }
    Controller_Unlock();
    return result;
}

static __inline__ void DecodePakText(u8 *dst, u8 *src, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            dst[i] = gPakCharTable[src[i]];
        } else {
            dst[i] = '~';
        }
    }
}

/* Reports note index of the pak on channel ch from the cached directory. */
s32 Pak_FindNote(s32 ch, s32 index, s32 *exists, u8 *name, u8 *ext, s32 *size, u8 *company, u8 *code) {
    if (gPakState[ch] != 3) {
        return -2;
    }
    if (gPakDirectories[ch].notes[index].file_size == 0) {
        *exists = 0;
    } else {
        *exists = 1;
        *size = gPakDirectories[ch].notes[index].file_size >> 8;
#ifdef TARGET_PORT
        /* the port: the codes are integers in the cached OSPfsState; the console copies their bytes, which are
         * big-endian there ("NT2E", "51"). Spelled out high byte first so the text compares as on the console. */
        {
            u16 cc = gPakDirectories[ch].notes[index].company_code;
            u32 gc = gPakDirectories[ch].notes[index].game_code;

            company[0] = cc >> 8;
            company[1] = cc;
            code[0] = gc >> 24;
            code[1] = gc >> 16;
            code[2] = gc >> 8;
            code[3] = gc;
        }
#else
        func_0029E460(company, &gPakDirectories[ch].notes[index].company_code, 2);
        func_0029E460(code, &gPakDirectories[ch].notes[index].game_code, 4);
#endif
        DecodePakText(name, gPakDirectories[ch].notes[index].game_name, 16);
        DecodePakText(ext, gPakDirectories[ch].notes[index].ext_name, 4);
        code[4] = 0;
        company[2] = 0;
    }
    return 0;
}

s32 Pak_DeleteNote(s32 ch, s32 index) {
    s32 result;

    if (gPakState[ch] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        result = osPfsDeleteFile(&gPakFileSystems[ch], gPakDirectories[ch].notes[index].company_code,
                                 gPakDirectories[ch].notes[index].game_code,
                                 gPakDirectories[ch].notes[index].game_name,
                                 gPakDirectories[ch].notes[index].ext_name);
        if (result != 0) {
            result = -1;
        }
        Pak_Mount(ch);
    }
    Controller_Unlock();
    return result;
}
void Pak_AllocDirectory(void);
void *memset(void *, s32, uptr);

DEFERRED void Pak_AllocDirectory(void) {
    struct Shape_typemap_165 *temp_v0;
    sptr temp_a0;
    temp_v0 = CCache__Allocate(0, 0x810, 0x23, "Mempak pakstate");
    temp_a0 = (sptr) temp_v0->data;
    gPakStateBuffer = (CMemEntry *)temp_v0;
    gPakDirectories = (PakDirectory *)temp_a0;
    memset((void *)temp_a0, 0, 0x810);
    gPakDirectoryAllocated = 1;
}

/* Releases the pak directory buffer and clears its global state. */
DEFERRED void Pak_FreeDirectory(void) {
    ((void (*)(s32))CCache__Flush)(0); /* the ROM passes 0 */
    if (gPakStateBuffer) {
        CCache__Deallocate(0, gPakStateBuffer);
        gPakStateBuffer = 0;
        gPakDirectories = 0;
    }
    gPakDirectoryAllocated = 0;
}

/* Probes the pak in slot ch and records its state: 2 for a Rumble Pak, otherwise 3 or 1 depending on Pak_Mount. */
DEFERRED void Pak_ProbeSlot(s32 ch) {
    RumblePak *cur;
    s32 ready;

    Controller_Lock(1);
    Controller_ReadAll();
    gPakQueueState = 2;

    cur = &gRumblePaks[ch];
    if (gContStatus[ch].errno != 0) {
        cur->present = 0;
        ready = 0;
    } else {
        cur->present = osMotorInit(&gSiMessageQueue, &cur->pfs, ch) == 0;
        ready = cur->present;
    }

    if (ready != 0) {
        gPakState[ch] = 2;
    } else if (Pak_Mount(ch) != 0) {
        gPakState[ch] = 3;
    } else {
        gPakState[ch] = 1;
    }
    Controller_Unlock();
}

DEFERRED s32 Pak_GetStatus(s32 i) {
    if (gPakState[i] != 3) {
        return -2;
    }
    return gPakError[i];
}

DEFERRED s32 Pak_IsRumblePak(s32 arg0) {
    return gPakState[arg0] == 2;
}

#ifdef TARGET_PORT
void T2Save_FromPak(void *dst, s32 size);
#endif

/* Reads note file_no of the pak on channel ch into dst. */
DEFERRED s32 Pak_ReadNote(s32 ch, s32 file_no, void *dst, s32 size) {
    s32 result;
    s32 blocks;
    CMemEntry *handle;
    void *buf;

    blocks = (size + 0xFF) & ~0xFF;
    if (gPakState[ch] != 3) {
        return -2;
    }
    handle = (CMemEntry *)CCache__Allocate(0, blocks, 0x23, (sptr)sPakText_MempakReadTempData);
    buf = handle->data;
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        if ((result = osPfsReadWriteFile(&gPakFileSystems[ch], file_no, 0, 0, blocks, buf)) != 0) {
            result = -1;
        }
        if (result == 0) {
            func_0029E460(dst, buf, size);
#ifdef TARGET_PORT
            /* the port: a big-endian save record to host order (port/game/t2save.c) */
            T2Save_FromPak(dst, size);
#endif
        }
    }
    Controller_Unlock();
    ((void (*)(s32))CCache__Flush)(0); /* the ROM passes 0 */
    if (handle != 0) {
        CCache__Deallocate(0, handle);
    }
    return result;
}

/* Counts the empty notes of the pak on channel ch. */
DEFERRED s32 Pak_CountEmptyNotes(s32 ch, s32 *count) {
    s32 i;

    if (gPakState[ch] != 3) {
        return -2;
    }
    *count = 0;
    if (gPakError[ch] == 0) {
        for (i = 0; i < 16; i++) {
            if (gPakDirectories[ch].notes[i].file_size == 0) {
                (*count)++;
            }
        }
    }
    return gPakError[ch];
}

DEFERRED int Pak_GetFreePages(int slot, int *score) {
    if (gPakState[slot] != 3) {
        return -2;
    }
    if (gPakError[slot] == 0) {
        *score = gPakDirectories[slot].freeBytes / 256;
    } else {
        *score = 0;
    }
    return gPakError[slot];
}

s32 osPfsRepairId(OSPfs *pfs);

DEFERRED s32 Pak_RepairIfDamaged(s32 arg0) {
    s32 temp_s0;
    s32 var_s0;
    temp_s0 = arg0 * 4;
    if (gPakState[arg0] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    var_s0 = gPakError[arg0];
    gPakQueueState = 2;
    if (var_s0 == -4) {
        var_s0 = osPfsRepairId(&gPakFileSystems[arg0]);
        if (var_s0 != 0) {
            var_s0 = -1;
        }
        Pak_Mount(arg0);
    }
    Controller_Unlock();
    return var_s0;
}

DEFERRED s32 Pak_Repair(s32 arg0) {
    s32 var_s0;
    if (gPakState[arg0] != 3) {
        return -2;
    }
    Controller_Lock(1);
    Controller_ReadAll();
    gPakQueueState = 2;
    var_s0 = osPfsRepairId(&gPakFileSystems[arg0]);
    if (var_s0 != 0) {
        var_s0 = -1;
    }
    Pak_Mount(arg0);
    Controller_Unlock();
    return var_s0;
}

DEFERRED s32 Pak_GetNoteGameName(s32 bank, s32 slot, char **out) {
    if (gPakState[bank] != 3) {
        return -2;
    }
    *out = gPakDirectories[bank].notes[slot].game_name;
    return 0;
}

DEFERRED s32 Pak_GetNoteExtName(s32 bank, s32 slot, u8 **out) {
    if (gPakState[bank] != 3) {
        return -2;
    }
    *out = (u8 *)gPakDirectories[bank].notes[slot].ext_name;
    return 0;
}
s32 Pak_GetNotePages(s32 index, s32 entry, s32 *out);
s32 Pak_FindFreeExtLetter(s32 ch, u8 *ext);
s32 Pak_NeedsRepair(s32 i);
s32 Pak_ProbeRumble(s32 ch);
void Pak_DecodeText(u8 *src, u8 *dst, s32 n);
void Pak_EncodeText(u8 *src, u8 *dst, s32 n);
s32 Str_Compare(u8 *a, u8 *b);
s32 Str_Length(u8 *arg0);
u8 *Str_CopyN(u8 *arg0, u8 *arg1, s32 arg2);
u8 *Str_Copy(u8 *arg0, u8 *arg1);
s32 Pak_TextHasInvalidChar(u8 *str, s32 len);
u32 Pak_BytesToPages(sptr arg0);

DEFERRED s32 Pak_GetNotePages(s32 index, s32 entry, s32 *out) {
    if (gPakState[index] != 3) {
        return -2;
    }
    *out = Pak_BytesToPages(gPakDirectories[index].notes[entry].file_size);
    return 0;
}

/* Picks the first note extension letter ('A' + n) no note of the pak on channel ch uses. */
DEFERRED s32 Pak_FindFreeExtLetter(s32 ch, u8 *ext) {
    u8 company[8];
    u8 code[8];
    u8 name[16];
    s32 exists;
    s32 size;
    s32 used;
    s32 i;

    used = 0;
    if (gPakState[ch] != 3) {
        return -2;
    }
    for (i = 0; i < 16; i++) {
        if (Pak_FindNote(ch, i, &exists, name, ext, &size, company, code) != 0) {
            return -1;
        }
        if (exists) {
            used |= 1 << (ext[0] - 'A');
        }
    }
    ext[0] = 'A';
    ext[1] = 0;
    ext[2] = 0;
    ext[3] = 0;
    for (i = 0; i < 16; i++) {
        if (!((used & (1 << i)) & 0xFFFF)) {
            break;
        }
        ext[0]++;
    }
    if (i >= 16) {
        return -1;
    }
    return 0;
}

DEFERRED s32 Pak_NeedsRepair(s32 i) {
    if (gPakState[i] != 3) {
        return 0;
    }
    return gPakError[i] == -4;
}

/* Probes slot ch for a Rumble Pak; returns whether one answered. */
DEFERRED s32 Pak_ProbeRumble(s32 ch) {
    RumblePak *cur;
    s32 ready;

    cur = &gRumblePaks[ch];
    if (gContStatus[ch].errno == 0) {
        cur->present = osMotorInit(&gSiMessageQueue, &cur->pfs, ch) == 0;
        ready = cur->present;
    } else {
        cur->present = 0;
        ready = 0;
    }
    return ready;
}

/* Pak font code -> character. */

/* Decodes n pak font codes from src into characters at dst; codes past the table become '~'. */
DEFERRED void Pak_DecodeText(u8 *src, u8 *dst, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (src[i] < 0x42) {
            *dst = gPakCharTable[src[i]];
        } else {
            *dst = '~';
        }
        dst++;
    }
}

/* Pak font code -> character. */

static __inline__ s32 toUpper(s32 c) {
    if ((u32)(c - 'a') < 26) {
        c -= 0x20;
    }
    return c;
}

/* Upper-cases n characters of src in place and encodes them as pak font codes at dst (0 when none fits). */
DEFERRED void Pak_EncodeText(u8 *src, u8 *dst, s32 n) {
    s32 i;
    s32 j;

    for (i = 0; i < n; i++) {
        src[i] = toUpper(src[i]);
        for (j = 0; j < 0x42; j++) {
            if (src[i] == gPakCharTable[j]) {
                dst[i] = j;
                break;
            }
        }
        if (j == 0x42) {
            dst[i] = 0;
        }
    }
}

/* strcmp. */
DEFERRED s32 Str_Compare(u8 *a, u8 *b) {
    while (*a == *b) {
        if (*a++ == 0) {
            return 0;
        }
        b++;
    }
    return *a - *b;
}

DEFERRED s32 Str_Length(u8 *arg0) {
    u8 *var_v1;
    u8 temp;
    var_v1 = arg0 + 1;
    if (*arg0 != 0) {
        do {
            temp = *var_v1;
            var_v1 += 1;
        } while (temp != 0);
    }
    return (var_v1 - arg0) - 1;
}

DEFERRED u8 *Str_CopyN(u8 *arg0, u8 *arg1, s32 arg2) {
    s32 var_a2;
    u8 *temp_v1;
    u8 *var_a0;
    u8 *var_a1;
    u8 temp_v0;
    var_a0 = arg0;
    var_a1 = arg1;
    var_a2 = arg2;
    temp_v1 = var_a0;
    if (var_a2 != 0) {
    loop_1:
        temp_v0 = *var_a1;
        var_a1 += 1;
        *var_a0 = temp_v0;
        var_a0 += 1;
        if (temp_v0 & 0xFF) {
            var_a2 -= 1;
            if (var_a2 != 0) {
                goto loop_1;
            }
        }
        if (var_a2 != 0) {
            var_a2 -= 1;
            if (var_a2 != 0) {
                do {
                    *var_a0 = 0;
                    var_a2 -= 1;
                    var_a0 += 1;
                } while (var_a2 != 0);
            }
        }
    }
    return temp_v1;
}

DEFERRED u8 *Str_Copy(u8 *arg0, u8 *arg1)
{
  u8 *var_v1;
  u8 temp_v0;
  int new_var;
  u8 temp_v0_2;
  temp_v0 = *arg1;
  arg1 = arg1 + 1;
  *arg0 = temp_v0;
  new_var = 1;
  var_v1 = arg0 + new_var;
  if (temp_v0 & 0xFF)
  {
    do
    {
      temp_v0_2 = *arg1;
      arg1 += 1;
      *var_v1 = temp_v0_2;
      var_v1 += 1;
    }
    while (temp_v0_2 & 0xFF);
  }
  return arg0;
}

DEFERRED s32 Pak_TextHasInvalidChar(u8 *str, s32 len) {
    do {
        if (*str++ == '~') {
            return 1;
        }
    } while (--len != -1);
    return 0;
}

DEFERRED u32 Pak_BytesToPages(sptr arg0) {
    return (u32) (arg0 + 0xFF) >> 8;
}
u32 Crc32_Compute(u8 *arg0, u32 arg1, u32 arg2);

DEFERRED u32 Crc32_Compute(u8 *arg0, u32 arg1, u32 arg2) {
    u32 table[256];
    u32 i;
    u32 j;
    u32 value;
    u32 shifted;
    u8 *end;
    for (i = 0; i < 256; i++) {
        value = i;
        for (j = 0; j < 8; j++) {
            shifted = value >> 1;
            if (value & 1) {
                shifted ^= 0xEDB88320U;
            }
            value = shifted;
        }
        table[i] = value;
    }
    end = arg0 + arg1;
    arg2 = ~arg2;
    while (arg0 < end) {
        arg2 = table[(arg2 ^ *arg0++) & 0xFF] ^ ((arg2 >> 8) & 0xFFFFFF);
    }
    return ~arg2;
}

DEFERRED u8 *Str_Cat(u8 *arg0, u8 *arg1) {
    u8 *var_a1;
    u8 *var_v1;
    u8 temp_v0;

    var_a1 = arg1;
    var_v1 = arg0;
    if (*arg0 != 0) {
        do {
            var_v1 += 1;
        } while (*var_v1 != 0);
    }
    do {
        temp_v0 = *var_a1;
        var_a1 += 1;
        *var_v1 = temp_v0;
        var_v1 += 1;
    } while (temp_v0 & 0xFF);
    return arg0;
}

/* strncat. */
DEFERRED u8 *Str_CatN(u8 *dst, u8 *src, s32 n) {
    u8 *p;

    p = dst;
    while (*p++) {
    }
    p--;
    while (n--) {
        if ((*p++ = *src++) == 0) {
            return dst;
        }
    }
    *p = 0;
    return dst;
}

/* strncmp. */
DEFERRED s32 Str_CompareN(u8 *a, u8 *b, s32 n) {
    if (n == 0) {
        return 0;
    }
    n--;
    while (n != 0 && *a != 0 && *a == *b) {
        n--;
        a++;
        b++;
    }
    return *a - *b;
}
s32 Str_CompareNoCase(u8 *arg0, u8 *arg1);

DEFERRED s32 Str_CompareNoCase(u8 *arg0, u8 *arg1)
{
  s32 c2;
  int new_var;
  s32 c1;
  c2 = *arg1;
  arg1 += 1;
  new_var = 0x41;
  if (c2 >= new_var)
  {
    if (c2 < 0x5B)
    {
      c2 += 0x20;
    }
  }
  c1 = *arg0;
  arg0 += 1;
  if (c1 >= new_var)
  {
    if (c1 < 0x5B)
    {
      c1 += 0x20;
    }
  }
  if (c2 != 0)
  {
    if (c2 == c1)
    {
      return Str_CompareNoCase(arg0, arg1);
    }
  }
  return c1 - c2;
}
u8 *Str_FindLastChar(u8 *s, int c);

DEFERRED u8 *Str_FindLastChar(u8 *s, int c) {
    u8 *start = s;
    while (*s++ != 0) {
    }
    s--;
    for (; s != start; s--) {
        if (*s == (u8)c) {
            break;
        }
    }
    if (*s == (u8)c) {
        return s;
    }
    return 0;
}

/* In-place upper-casing of a NUL-terminated string. */
DEFERRED u8 *Str_ToUpper(u8 *str) {
    u8 *p = str;

    if (str == 0) {
        return 0;
    }
    while (*p != 0) {
        if ((u8) (*p - 'a') < 26) {
            *p -= 0x20;
        }
        p++;
    }
    return str;
}

DEFERRED u8 *Str_ToLower(u8 *arg0) {
    u8 *var_v1;
    u8 temp_a1;

    var_v1 = arg0;
    if (arg0 == NULL) {
        return NULL;
    }
    if (*arg0 != 0) {
        do {
            temp_a1 = *var_v1;
            if ((u32) (temp_a1 - 0x41) < 0x1AU) {
                *var_v1 = temp_a1 + 0x20;
            }
            var_v1 += 1;
        } while (*var_v1 != 0);
    }
    return arg0;
}

DEFERRED s32 Char_ToUpper(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if ((u32) (var_a0 - 0x61) < 0x1AU) {
        var_a0 -= 0x20;
    }
    return var_a0;
}

DEFERRED s32 Char_ToLower(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    if ((u32) (var_a0 - 0x41) < 0x1AU) {
        var_a0 += 0x20;
    }
    return var_a0;
}

DEFERRED s32 Pak_IsDirectoryAllocated(void) {
    return gPakDirectoryAllocated;
}

extern s32 memcmp(void *a, void *b, s32 size);
#ifdef TARGET_PORT
void *T2Save_ToPak(void *data, s32 size);
#endif

/* Reads note file_no of the pak on channel ch into dst. */
static __inline__ s32 readNote(s32 ch, s32 file_no, void *dst, s32 size) {
    s32 result;
    s32 blocks;
    CMemEntry *handle;
    void *buf;

    blocks = (size + 0xFF) & ~0xFF;
    if (gPakState[ch] != 3) {
        return -2;
    }
    handle = (CMemEntry *)CCache__Allocate(0, blocks, 0x23, (sptr)"Mempak read temp data");
    buf = handle->data;
    Controller_Lock(1);
    Controller_ReadAll();
    result = gPakError[ch];
    gPakQueueState = 2;
    if (result == 0) {
        if ((result = osPfsReadWriteFile(&gPakFileSystems[ch], file_no, 0, 0, blocks, buf)) != 0) {
            result = -1;
        }
        if (result == 0) {
            func_0029E460(dst, buf, size);
        }
    }
    Controller_Unlock();
    ((void (*)(s32))CCache__Flush)(0); /* the ROM passes 0 */
    if (handle != 0) {
        CCache__Deallocate(0, handle);
    }
    return result;
}

/* Saves data as a note on the pak on channel ch and reads it back to verify it, up to four times. */
s32 Pak_SaveNoteVerified(s32 ch, s32 size, void *data, u8 *name, u8 *ext, u8 *code) {
    s32 result;
    s32 file_no;
    CMemEntry *handle;
    void *buf;
    s32 blocks;
    s32 tries;

#ifdef TARGET_PORT
    /* the port: the note goes to the pak big-endian, as the console writes it (port/game/t2save.c) */
    data = T2Save_ToPak(data, size);
#endif
    blocks = (size + 0xFF) & ~0xFF;
    if (gPakState[ch] != 3) {
        return -2;
    }
    handle = (CMemEntry *)CCache__Allocate(0, blocks, 0x23, (sptr)"Mempak write temp data");
    buf = handle->data;
    for (tries = 0; tries < 4; tries++) {
        result = Pak_WriteNote(ch, size, data, name, &file_no, ext, sPakCompanyCode, code);
        if (result != 0) {
            break;
        }
        if (readNote(ch, file_no, buf, blocks) == 0 && memcmp(buf, data, size) == 0) {
            break;
        }
        if (Pak_DeleteNote(ch, file_no) != 0) {
            break;
        }
    }
    ((void (*)(s32))CCache__Flush)(0); /* the ROM passes 0 */
    if (handle != 0) {
        CCache__Deallocate(0, handle);
    }
    return result;
}
