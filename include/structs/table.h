#ifndef STRUCTS_TABLE_H
#define STRUCTS_TABLE_H

/* The three table layouts of the engine's resources and level data. */

/* A table of fixed-size records: the size of a record, their count, then
 * the records. The level file's object and actor tables and the world's
 * sections built from them (func_00220D00) have this header. */
typedef struct StrideTable {
    /* 0x0 */ s32 stride;
    /* 0x4 */ s32 count;
    /* 0x8 */ u8 entries[1];
} StrideTable;

/* A sectioned resource block: the count of its sections, then the offset
 * of each one from the start of the block and the offset after the last
 * (func_002017D4 returns a section, func_002017E8 a section and its size). */
typedef struct OffsetTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offsets[1];
} OffsetTable;

/* An array resource: count elements of size bytes, then the data
 * (func_002018F0, func_00201910). */
typedef struct ArrayTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 size;
    /* 0x8 */ u8 data[1];
} ArrayTable;

#endif
