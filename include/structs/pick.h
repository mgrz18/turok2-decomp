#ifndef STRUCTS_PICK_H
#define STRUCTS_PICK_H

/* A weighted pick table: a list of {id, weight} ending at id -1.
 * WeightedTable_PickStatic returns one id at random, each with the odds weight / (sum
 * of the weights); the loot drops, the pickups of the boss fights and the
 * boss idle anims use it (Rage Wars: func_80279798_de). */
typedef struct Pick {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 weight;
} Pick; /* size 0x4 */

/* A pick table built at run time (Rage Wars' WeightedTable):
 * WeightedTable_Clear empties it, WeightedTable_Add adds an entry (up to 0x18) and
 * WeightedTable_Pick ends it with -1 and picks from it. */
typedef struct PickTable {
    /* 0x00 */ s16 count;
    /* 0x02 */ Pick entries[0x19];  /* 0x18 and the -1 that ends them */
    /* 0x66 */ s16 reserved;
} PickTable; /* size 0x68 */

#endif
