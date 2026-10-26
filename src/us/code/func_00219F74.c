#include "common.h"

/* Pick, at random, one entry of the actor's animation set (0xBA) whose action
 * is `action`: among the run of equal entries around the one found, leaving
 * out index `skip` (-1: none). */

typedef struct {
    u8 pad0[0xA4];
    s32 unkA4;      /* 0xA4 */
    u8 padA8[0x8];
    s32 unkB0;      /* 0xB0 */
    u8 padB4[0x6];
    s16 setId;      /* 0xBA */
    u8 padBC[0x18];
    u32 flags;      /* 0xD4 */
} Actor;

typedef struct {
    s16 count;      /* 0x00 */
    u8 pad[0x66];
} RandList;         /* 0x68 */

extern u8 D_800A5AC4[];     /* "anim object info" */

s32 *func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 *func_002017D4(s32, s32);
void func_002051F4(s32, s32 *);
s32 func_00266D24(s32 *, s32, s32);
void func_0026D5DC(RandList *);
void func_0026D5E4(RandList *, s16, s32);
s32 func_0026D628(RandList *);

s32 func_00219F74(Actor *actor, s32 action, s32 skip) {
    RandList list;
    s32 *res;
    s32 *data;
    s32 *entries;
    s32 count;
    s32 i;
    s32 found;

    if (actor->flags & 0x40000) {
        res = func_0020367C(0, actor->unkA4, actor->unkB0, 0, 0, 0, (s32) D_800A5AC4, 1);
        if (res != NULL) {
            data = func_002017D4(*res, 1);
            entries = data + 2;
            count = data[1];
            i = func_00266D24(entries, actor->setId, action);
            if (i != -1) {
                found = i;
                while (i > 0 && entries[i - 1] == action) {
                    i--;
                }
                func_0026D5DC(&list);
                if (skip == -1) {
                    while (i < count && entries[i] == action) {
                        func_0026D5E4(&list, i++, 10);
                    }
                } else {
                    while (i < count && entries[i] == action) {
                        if (skip != i) {
                            func_0026D5E4(&list, i, 10);
                        }
                        i++;
                    }
                }
                if (list.count == 0) {
                    func_0026D5E4(&list, found, 10);
                }
                i = (s16) func_0026D628(&list);
            }
            func_002051F4(0, res);
            return i;
        }
    }
    return -1;
}
