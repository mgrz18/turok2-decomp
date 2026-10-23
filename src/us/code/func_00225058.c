/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C26C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct LevelActor {
    u8 pad0[0xD4];
    s32 flags;                 /* 0xD4 (0x100) */
    u8 padD8[0x288 - 0xD8];
} LevelActor;                  /* 0x288 bytes (0x2E8) */

typedef struct Record14 {
    u8 data[0x14];
} Record14;

typedef struct Shared_World {
    u8 pad0[0xE0];
    LevelActor *actors;        /* 0xE0 (0x138) */
    s32 unkE4;
    s32 actorCount;            /* 0xE8 (0x140) */
    u8 padEC[0x1168 - 0xEC];
    s32 recordCount;           /* 0x1168 (0x11C0) */
    s32 alternateRecordCount;  /* 0x116C (0x11C4) */
    s32 *recordIndex;          /* 0x1170 (0x11C8) */
    s32 *alternateRecordIndex; /* 0x1174 (0x11CC) */
    Record14 *recordArray;     /* 0x1178 (0x11D0) */
} Shared_World;

extern s32 func_00266D8C(s32 *index, s32 count, s32 key, s32 *first, s32 *last);

/* Index of actor in the world's actor array, or -1 (same as func_002265E8). */
static __inline__ s32 World_ActorIndex(Shared_World *world, LevelActor *actor) {
    LevelActor *base;
    if (actor->flags & 0x80000) {
        return -1;
    }
    base = world->actors;
    if (actor >= base &&
        (char *)actor <= (char *)base + ((u32)world->actorCount * sizeof(LevelActor) - sizeof(LevelActor))) {
        return ((u32)actor - (u32)base) / sizeof(LevelActor);
    }
    return -1;
}

/* Look up the run of records belonging to actor arg1: *arg2 = first record,
 * *arg3 = record count (0 if none). */
void func_00225058(Shared_World *arg0, LevelActor *arg1, Record14 **arg2, s32 *arg3) {
    s32 first;
    s32 last;
    s32 *recordIndex;
    s32 recordCount;
    s32 index = World_ActorIndex(arg0, arg1);
    if (index == -1) {
        *arg3 = 0;
        return;
    }
    recordIndex = arg0->recordIndex;
    recordCount = arg0->recordCount;
    if (func_00266D8C(recordIndex, recordCount, index, &first, &last) != 0) {
        *arg2 = &arg0->recordArray[first];
        *arg3 = (last - first) + 1;
    } else {
        *arg2 = 0;
        *arg3 = 0;
    }
}
