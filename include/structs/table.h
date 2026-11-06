#ifndef STRUCTS_TABLE_H
#define STRUCTS_TABLE_H

/* The three table layouts of the engine's resources and level data. */

/* A table of fixed-size records: the size of a record, their count, then
 * the records. The level file's object and actor tables and the world's
 * sections built from them (CScene__DecompressGridSection) have this header. */
typedef struct StrideTable {
    /* 0x0 */ s32 stride;
    /* 0x4 */ s32 count;
    /* 0x8 */ u8 entries[1];
} StrideTable;

/* A StrideTable of halfword ids (a stride of 2): the level's object type
 * table (CScene's m_pObjectTypes; Rage Wars' TableRecAt8). */
typedef struct IdTable {
    /* 0x0 */ s32 stride;
    /* 0x4 */ s32 count;
    /* 0x8 */ u16 ids[1];
} IdTable;

/* A sectioned resource block: the count of its sections, then the offset
 * of each one from the start of the block and the offset after the last
 * (CISet__GetBlock returns a section, CISet__GetBlockAndSize a section and its size). */
typedef struct OffsetTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offsets[1];
} OffsetTable;

/* An array resource: count elements of size bytes, then the data
 * (CUSet__GetSetAndSize, CUSet__GetArrayAndSize). */
typedef struct ArrayTable {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 size;
    /* 0x8 */ u8 data[1];
} ArrayTable;

#endif
