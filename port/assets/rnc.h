/*
 * rnc.h - RNC ProPack decompressor (methods 1 and 2) for the Turok 2 port.
 *
 * Original code for this repository (same license as the repo), written from Turok 2's own
 * routines (ProPackDecompress, src/us/code/func_00200B00.c; RNC_UnpackMethod1 at 0x200C40
 * and RNC_UnpackMethod2 at 0x2010C0, us/asm/1840.s) and the public RNC header layout. The
 * Python twin is tools/assets/rnc.py; both are checked against every RNC stream of the ROM
 * (docs/assets/ARCHIVE.md). Unlike the game's routines these never read or write outside
 * the buffers they are given.
 */
#ifndef T2_RNC_H
#define T2_RNC_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define RNC_HEADER_LEN 0x12

typedef struct {
    uint8_t method;        /* 1 or 2 */
    uint32_t unpacked;     /* unpacked size */
    uint32_t packed;       /* packed data size (after the header) */
    uint16_t crcUnpacked;  /* CRC-16 of the unpacked bytes */
    uint16_t crcPacked;    /* CRC-16 of the packed data */
    uint8_t leeway;
    uint8_t chunks;
} RncHeader;

enum {
    RNC_OK = 0,
    RNC_ERR_HEADER = -1,     /* no 'RNC' + method 1/2, or the header does not fit */
    RNC_ERR_TRUNCATED = -2,  /* the stream reads past `avail` */
    RNC_ERR_DATA = -3,       /* malformed stream (no matching code, match before start) */
    RNC_ERR_OUTPUT = -4,     /* output does not fit dstCap / differs from the header size */
    RNC_ERR_CRC = -5         /* a CRC-16 does not match the header */
};

/* 1 if p starts with 'RNC' + method 1 or 2 and avail >= RNC_HEADER_LEN. */
int Rnc_IsStream(const uint8_t *p, size_t avail);
/* Parse the header; RNC_OK or RNC_ERR_HEADER. */
int Rnc_ReadHeader(const uint8_t *p, size_t avail, RncHeader *h);
/* CRC-16 as RNC uses it (reflected polynomial 0xA001, initial value 0). */
uint16_t Rnc_Crc16(const uint8_t *p, size_t n);
/* Unpack the stream at src (avail readable bytes) into dst (dstCap bytes). verify != 0 also
 * checks both CRCs. Returns the unpacked size (>= 0) or an RNC_ERR_* code. */
int64_t Rnc_Unpack(const uint8_t *src, size_t avail, uint8_t *dst, size_t dstCap, int verify);
const char *Rnc_ErrorString(int64_t err);

#ifdef __cplusplus
}
#endif

#endif /* T2_RNC_H */
