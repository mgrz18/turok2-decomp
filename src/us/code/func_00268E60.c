#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u8 pad48[0xA];
    /* 0x52 */ u16 flags2;
    /* 0x54 */ u8 pad54[4];
    /* 0x58 */ u8 kind;
} KeyInfo;

void *func_0022425C(void *table, void *key);
s32 func_0021F1F8(void *arg);
extern u8 D_800F7078[];

s32 func_00268E60(u8 *obj) {
    void *key = *(void **) (obj + 0x10);
    KeyInfo *info;
    s32 result = 0;
    u32 objFlags;
    u8 *other;

    if (key != NULL && (info = func_0022425C(D_800F7078, key)) != NULL) {
        if (info->flags & 0x4000) {
            result = -1;
        } else if (info->flags & 0x400000) {
            result = 7;
        } else if (info->flags2 & 0x80) {
            result = 8;
        } else {
            result = info->kind;
        }
    }
    if (obj[0] == 1) {
        objFlags = *(u32 *) (obj + 0x140);
        if (objFlags & 0x400) {
            result = 1;
        }
        if (objFlags & 0x40000) {
            other = *(u8 **) (obj + 0x30);
            if (other != NULL && other[0] == obj[0]) {
                result = func_0021F1F8(other);
            }
        }
    }
    return result;
}
