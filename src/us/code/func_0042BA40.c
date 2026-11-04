/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044C7B8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"

typedef struct LevelRecord {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ u8 pad10[0x10];
    /* 0x20 */ u16 spawnId;
    /* 0x22 */ u16 defId;
    /* 0x24 */ s16 angle;
    /* 0x26 */ u8 pad26[2];
} LevelRecord; /* 0x28 */

typedef struct LevelNode {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ LevelRecord records[1];
} LevelNode;

typedef struct LevelActor {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ s32 *def;
    /* 0x018 */ u8 pad18[0x1A8 - 0x18];
    /* 0x1A8 */ struct LevelActor *owner;
    /* 0x1AC */ u8 pad1AC[0x288 - 0x1AC];
} LevelActor; /* 0x288 */

typedef struct ActorBlock {
    /* 0x00 */ s32 size;
    /* 0x04 */ s32 count;
    /* 0x08 */ LevelActor objects[1];
} ActorBlock;

typedef struct Handle {
    void *data;
} Handle;

typedef struct Placed {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ f32 angle;
    /* 0x10 */ s16 kind;
    /* 0x12 */ s16 id;
} Placed; /* 0x14 */

typedef struct World {
    /* 0x00000 */ u8 pad0[0xE0];
    /* 0x000E0 */ LevelActor *objects;
    /* 0x000E4 */ s32 playerCount;
    /* 0x000E8 */ s32 objectCount;
    /* 0x000EC */ u8 padEC[0xF00 - 0xEC];
    /* 0x00F00 */ s32 unkF00;
    /* 0x00F04 */ u8 padF04[0x18FA8 - 0xF04];
    /* 0x18FA8 */ s32 group;
    /* 0x18FAC */ u8 pad18FAC[0x18FB8 - 0x18FAC];
    /* 0x18FB8 */ s32 spawnsReady;
    /* 0x18FBC */ u8 spawnInfo[0x18FD0 - 0x18FBC];
    /* 0x18FD0 */ s32 spawnKey;
} World;

extern CVector3 D_80119928;
extern s32 D_80119948;
typedef struct Player {
    u8 pad0[0x1328];
} Player;

extern Player *D_80119874;
extern s32 D_80119878;

extern s32 func_00200B00(Handle **handle, s32);
extern s32 func_002242F8(World *world, s32);
extern Handle *func_00205D7C(s32, Handle **handle, s32 size);
extern void func_0020192C(LevelNode *node, void *data, s32);
extern s32 func_0042C560(World *world, CVector3 *pos, s32 group, void *info);
extern s32 func_0042C194(World *world, s32 group, s32 key, Placed *out, s32 max);
extern s32 *func_00225E88(World *world, u16 id);
extern void func_0021E6AC(LevelActor *obj, LevelRecord *rec, s32 mode, s32 index);
extern s32 func_00220A1C(World *world, CVector3 *pos);
extern void func_00210EF0(f32 *angle);
extern void func_0042DA60(Player *player, LevelRecord *rec, s32 mode, s32 index);
extern s32 func_00267DBC(LevelActor *obj);
extern s32 func_00267D94(LevelActor *obj);
extern f32 func_002465F0(LevelActor *obj, CVector3 pos);
extern void func_002057B0(s32, Handle **handle, Handle *block);

/* Builds the level's actor block from its records: places every player on the spawn points found for
   the level (one record per player start), then links each remaining actor to the nearest actor with
   the same definition. The spawn point is indexed afresh on each use and the position copied with
   memcpy (as in Rage Wars): both set loop.c's insn count and giv benefits, which decide the ROM's
   hoisting of the spawns base and of the angle constants. */
void func_0042BA40(World *world, Handle **handle) {
    Placed spawns[4];
    LevelActor *objects;
    s32 mode;
    Handle *block;
    s32 nspawns;
    LevelNode *node;
    LevelRecord *records;
    ActorBlock *hdr;
    s32 n;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 spawnIndex;
    LevelRecord *rec;
    LevelRecord *recWalk;
    LevelActor *objWalk;
    Player *player;
    LevelActor *obj;
    LevelActor *other;
    f32 angle;
    f32 best;
    f32 dist;
    s16 value;

    if (func_00200B00(handle, 1) != 0) {
        mode = func_002242F8(world, 0);
        node = (*handle)->data;
        n = node->count;
        block = func_00205D7C(0, handle, n * sizeof(LevelActor) + 8);
        if (block != NULL) {
            func_0020192C(node, block->data, 1);
            hdr = block->data;
            hdr->size = sizeof(LevelActor);
            hdr->count = n;
            records = node->records;
            objects = hdr->objects;
            if (world->spawnsReady == 0) {
                world->spawnKey = func_0042C560(world, &node->records[0].pos, world->group, world->spawnInfo);
                if (world->spawnKey != 0) {
                    world->spawnsReady = 1;
                }
            }
            spawnIndex = 0;
            nspawns = 0;
            if (world->spawnsReady != 0) {
                nspawns = func_0042C194(world, world->group, world->spawnKey, spawns, 4);
            }
            world->objects = objects;
            world->objectCount = n;
            i = 0;
            if (i < n) {
                objWalk = objects;
                recWalk = records;
                while (i < n) {
                    rec = recWalk;
                    obj = objWalk;
                    i++;
                    obj->def = func_00225E88(world, rec->defId);
                    objWalk = obj + 1;
                    recWalk = rec + 1;
                }
            }
            world->playerCount = 0;
            for (i = 0; i < n; i++) {
                rec = &records[i];
                obj = &objects[i];
                func_0021E6AC(obj, rec, mode, i);
                if (*func_00225E88(world, rec->defId) != 0xB && i != 0) {
                    continue;
                }
                world->playerCount++;
                memcpy(&D_80119928, &rec->pos, sizeof(CVector3));
                D_80119928.y += 204.79999f;
                D_80119948 = func_00220A1C(world, &rec->pos);
                for (playerIndex = 0; playerIndex < D_80119878; playerIndex++) {
                    player = &D_80119874[playerIndex];
                    if (nspawns != 0) {
                        rec->spawnId = spawns[spawnIndex].id;
                        func_00210EF0(&spawns[spawnIndex].angle);
                        angle = spawns[spawnIndex].angle;
                        if (!(angle > 3.1415927f)) {
                            if (!(angle < -3.1415927f)) {
                                if (!(angle > 3.1415927f)) {
                                    value = angle * 10430.06f;
                                } else {
                                    value = 0x7FFF;
                                }
                            } else {
                                value = -0x7FFF;
                            }
                        } else {
                            value = 0x7FFF;
                        }
                        rec->angle = value;
                        memcpy(&rec->pos, &spawns[spawnIndex].pos, sizeof(CVector3));
                        spawnIndex++;
                        if (spawnIndex >= nspawns) {
                            spawnIndex = 0;
                        }
                    }
                    func_0042DA60(player, rec, mode, i);
                }
            }
            for (i = world->playerCount; i < n; i++) {
                obj = &objects[i];
                best = 3.4028235e38f;
                obj->owner = NULL;
                if (func_00267DBC(obj) == 0) {
                    continue;
                }
                for (j = world->playerCount; j < n; j++) {
                    other = &objects[j];
                    if (*obj->def == *other->def && func_00267D94(other) != 0 && obj != other) {
                        if (obj->owner != NULL) {
                            dist = func_002465F0(obj, other->pos);
                            if (dist < best) {
                                obj->owner = other;
                                best = dist;
                            }
                        } else {
                            obj->owner = other;
                            best = func_002465F0(obj, other->pos);
                        }
                    }
                }
            }
        }
        func_002057B0(0, handle, block);
        world->unkF00 = 0;
    }
}
