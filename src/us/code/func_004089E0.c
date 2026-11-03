#include "common.h"

/* As func_0021DA8C (engine), for a model drawn from its cached "lit vertices"
 * resource: the region sets come from func_0021EBE4 and the colour from the
 * module global D_8012FB64. */

typedef struct {
    s32 draw;               /* 0x00 */
    s32 shadow;             /* 0x04 */
    u8 pad8[0x4];
    s32 model;              /* 0x0C */
    u8 pad10[0x4];
    s32 index;              /* 0x14 */
    s32 set0;               /* 0x18 */
    s32 set1;               /* 0x1C */
} DrawParams;

typedef struct {
    u8 pad0[0x98];
    s32 unk98;              /* 0x098 */
    u8 pad9C[0x14C - 0x9C];
    s32 mask;               /* 0x14C (AI +0x0C) */
    u8 pad150[0x280 - 0x150];
    s32 unk280;             /* 0x280 */
} Actor;

extern s32 D_800ACAD8;
extern s32 D_8012FB64;
extern char D_800C0570[];   /* "lit vertices" */

void func_0021EBE4(Actor *, s32, s32 *, s32 *);
void func_0021DC84(Actor *, s32, DrawParams *);
void func_0020B6A0(s32, s32, s32, s32, char *, s32, s32);
s32 **func_00203CA0(s32, s32, s32, s32, s32, Actor *, void *, char *, s32);
void func_0020565C(s32, s32 **);
void func_00214F68(char *, char *);
void func_002051F4(s32, s32 **);
void func_00220208();

void func_004089E0(Actor *actor, s32 arg1, DrawParams *params) {
    s32 **resource;
    s32 *header;
    char *object;
    char *data;

    func_0021EBE4(actor, params->index, &params->set0, &params->set1);
    actor->mask = 1 << params->set0;
    if (params->shadow != 0) {
        func_0021DC84(actor, arg1, params);
    }
    if (params->draw == 0) {
        return;
    }
    resource = func_00203CA0(0, actor->unk280 | D_800ACAD8, params->model, 0, 0, actor,
                             func_00220208, D_800C0570, 1);
    if (resource == NULL) {
        return;
    }
    func_0020565C(0, resource);
    header = *resource;
    if (header[0] == 1) {
        object = (char *) header + header[1];
        func_00214F68(object, (char *) header + header[2]);
        data = object;
    } else {
        data = (char *) (header + 2);
    }
    func_0020B6A0(params->model, actor->unk98, 1, 0, data, D_8012FB64, -1);
    func_002051F4(0, resource);
}
