/*
 * t2asset.h - Turok 2 native port: asset reader (load units keyed by ROM address).
 *
 * The engine asks for assets by ROM address (CCache__Request key, CLoader address,
 * VM_Alloc window). This module answers those requests from one of two backends:
 *
 *   archive  turok2.o2r (a zip; entries t2/u/<KEY 8 hex>, built from the user's ROM by
 *            tools/assets/mkarchive.py or the Torch T2 module). RNC units are stored
 *            unpacked. Read through miniz here; libultraship's ArchiveManager can
 *            replace it later through T2Asset_SetBackend.
 *   rom      the user's ROM image itself (bring-up / development): units are read
 *            straight from the image and RNC streams are unpacked on read (port/assets/rnc.c).
 *
 * Both return the same bytes for every load unit: the ROM bytes, RNC-unpacked where
 * the unit is one RNC stream. Byte order: the archive stores units big-endian (ROM order)
 * together with a swap plan per unit (t2/swap, tools/assets/mkarchive.py from
 * swap_spec.json; docs/assets/ARCHIVE.md, BYTEORDER.md). When a plan table is loaded and the
 * host is little-endian, every unit is swapped to host order right after the backend loads it
 * (whole, before a prefix is cut), so T2Asset_Read / T2Asset_ReadInto return HOST-order
 * fields. The archive backends load t2/swap themselves; the ROM backend takes it from
 * T2Asset_LoadSwapFile (an archive or a bare t2/swap file). Without a table: ROM order.
 *
 * Not thread safe: callers serialize (on the port all game threads run one at a time under
 * t2os's baton, docs/port/RUNTIME.md).
 */
#ifndef T2ASSET_H
#define T2ASSET_H

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
/* u64 as in ultra64.h / libultraship (unsigned long long): uint64_t is unsigned long on LP64
 * Linux/macOS, and a different typedef for the same name is an error when both headers meet. */
typedef unsigned long long u64;

/* The four ROM roots the engine hard-codes (docs/assets/PORT.md 1.4). The first three
 * are read at inner addresses too (VM windows, audio DMA). */
#define T2_ROOT_AUDIO_CTL   0x001E0000u /* VM_Alloc(0x2F194, 0, 0x1E0000) */
#define T2_ROOT_AUDIO_DATA  0x0020F194u /* gAudioRomBase, sample DMA */
#define T2_ROOT_SOUND_DATA  0x00BE1814u /* VM_Alloc(0x3EA7A0, 0, 0xBE1814) */
#define T2_ROOT_CARTRIDGE   0x00FCBFB4u /* m_CartridgeIndex */
#define T2_ASSET_END        0x01FF7AECu

#define T2_ROM_SHA1 "fb0400f21e3f043939ab56500c7b12a3231006f1" /* US, the only supported release */

/* Unit flags: the archive's t2/units / unit header flags (tools/assets/romfs.py UF_*,
 * docs/assets/ARCHIVE.md). */
#define T2U_RNC_UNPACKED 0x0001u /* one RNC stream in the ROM; served unpacked (romSize = slot size) */
#define T2U_INDEX_HEADER 0x0002u /* header of an index set ((count*4+15)&~7 bytes) */
#define T2U_CONTAINER    0x0004u /* raw ISet read whole; its entries are parsed from RAM */
#define T2U_RAW_RANGE    0x0008u /* read piecemeal at inner addresses (audio wave table) */
#define T2U_VM_WINDOW    0x0010u /* VM_Alloc window (paged at inner addresses on N64) */
#define T2U_LOADSYNC     0x0020u /* read by CLoader__LoadSync (persistent defaults) */
#define T2U_NO_READER    0x0040u /* nothing reads it (cartridge entry 20) */
#define T2U_RNC_CHILDREN 0x0080u /* container with RNC entries (none in the US ROM) */
#define T2U_INNER (T2U_RAW_RANGE | T2U_VM_WINDOW) /* may be read at addresses inside the unit */

/* A backend. LUS later: one implemented on Ship::ArchiveManager (ResourceGetDataByName /
 * LoadFile of "t2/u/%08X"). All sizes are in bytes. */
typedef struct T2AssetBackend {
    const char *name;
    /* Find the unit starting at `key`. romSize: the size in ROM (slot size for RNC units),
     * hostSize: size of the bytes the reader returns. 0 = no unit at key. */
    int (*lookup)(void *ctx, u32 key, u32 *romSize, u32 *hostSize, u32 *flags);
    /* Copy hostSize bytes of the unit at `key` into dst (dst holds at least hostSize). */
    int (*load)(void *ctx, u32 key, void *dst, u32 hostSize);
    /* Optional: raw ROM bytes [addr, addr+len) for RAW_RANGE reads at inner addresses. NULL =
     * the front end serves them from a resident copy of the RAW_RANGE unit. */
    int (*read_raw)(void *ctx, u32 addr, void *dst, u32 len);
    /* Number of units and the i-th key (sorted ascending), for tools and tests. */
    u32 (*count)(void *ctx);
    u32 (*key_at)(void *ctx, u32 i);
    void (*close)(void *ctx);
} T2AssetBackend;

/* ---- opening ------------------------------------------------------------------------- */
/* Each returns 0 on success, <0 on error (T2Asset_Error() says why). Opening replaces any
 * backend that was open. */
int T2Asset_OpenArchive(const char *o2rPath);
/* ROM backend: accepts .z64/.v64/.n64, normalizes byte order and checks the SHA1.
 * unitsTsv (optional, may be NULL): the units list (romfs.py --manifest TSV, or t2/units.tsv
 * of an archive): columns key, size[, host_size, kind, type, flags], hex, header line
 * allowed. Without it, only RNC streams are recognized as units (raw reads still work). */
int T2Asset_OpenRom(const char *romPath, const char *unitsTsv);
void T2Asset_SetBackend(const T2AssetBackend *backend, void *ctx);
void T2Asset_Close(void);
const char *T2Asset_Error(void);
const char *T2Asset_BackendName(void);

/* ---- reading ------------------------------------------------------------------------- */
/* Read the load unit whose ROM key is romKey.
 *   in:  *size = the size the engine asks for (CCache__Request size, VM_Alloc size), or 0
 *        for "the whole unit".
 *   out: *size = number of valid bytes returned (host size).
 * A request smaller than the unit's ROM size is a prefix probe (the 4-byte count probe,
 * the 0x18-byte object-index probe): the prefix is returned. A request >= the unit's ROM
 * size returns the whole unit, which for an unpacked RNC unit is LARGER than requested.
 * The buffer is 16-byte aligned and followed by 16 zero bytes (code that over-reads, as
 * LUS's Blob does). Free with T2Asset_Release. NULL if no unit starts at romKey. */
const void *T2Asset_Read(u32 romKey, u32 *size);
void T2Asset_Release(const void *data);

/* Host size the cache must allocate for CCache__Request(key, size): hostSize when a unit
 * starts at key and size >= its romSize, otherwise size (prefix probe / unknown key). */
u32 T2Asset_HostSize(u32 key, u32 size);

/* Copy-into form for the loader (CLoader__LoadSync / CLoader__Main / the audio DMA
 * callback, the VM pager): bytes [offset, offset+len) of the unit at `address`, or, when
 * `address` lies inside (or at the start of) a RAW_RANGE / VM_WINDOW unit, the ROM bytes at address+offset. Bytes past the unit's
 * end are zero-filled. Returns 0 on success, <0 if address is not served. */
int T2Asset_ReadInto(u32 address, u32 offset, void *dst, u32 len);

/* Enumeration (sorted keys). */
u32 T2Asset_UnitCount(void);
u32 T2Asset_UnitKey(u32 i);
int T2Asset_UnitInfo(u32 key, u32 *romSize, u32 *hostSize, u32 *flags);

/* ---- byte order ------------------------------------------------------------------------ */
/* Install a swap-plan table (the bytes of an archive's t2/swap entry; copied). Validated
 * (magic, format major 2, sorted keys, offsets inside the table). Returns 0 or <0. Closing
 * or replacing the backend drops it, so call it after opening. */
int T2Asset_SetSwapTable(const void *data, size_t len);
/* t2/swap out of an .o2r (zip) or a bare 'T2SW' file, for the ROM backend. */
int T2Asset_LoadSwapFile(const char *path);
/* 1 when units are being swapped to host order (table loaded and the host is little-endian). */
int T2Asset_SwapActive(void);
/* Swap one unit in place from ROM order to host order with its plan (a no-op on a big-endian
 * host or without a table). The reader calls it on every load; tests and tools may call it
 * on bytes they read themselves. Returns 1 if a plan was applied, 0 if the unit has none,
 * <0 if the plan does not fit the buffer (size must be the unit's host size). */
int T2Asset_SwapUnit(u32 key, void *data, u32 size);
/* Counters: plans applied, units loaded without a plan, plans refused (size mismatch: a mod
 * entry of a different size keeps ROM order). */
void T2Asset_SwapStats(u32 *applied, u32 *noPlan, u32 *refused);

/* ---- helpers shared with tools ------------------------------------------------------- */
/* Normalize a ROM image to .z64 order in place; returns 0 if it looks like an N64 image. */
int T2Rom_Normalize(u8 *rom, size_t len);
void T2_Sha1(const void *data, size_t len, u8 out[20]);
/* Unpack one RNC stream (methods 1 and 2) at `src` (avail bytes readable, including 16
 * bytes of slack after the stream for method 1's prefetch). Returns the unpacked size, or
 * 0 on error. If dst is NULL only the header is checked and the unpacked size returned. */
u32 T2Rnc_Unpack(const u8 *src, size_t avail, u8 *dst, u32 dstCap);

#ifdef __cplusplus
}
#endif

#endif /* T2ASSET_H */
