#include "common.h"

typedef struct {
    /* 0x0 */ s32 a;
    /* 0x4 */ s32 b;
    /* 0x8 */ s32 c;
    /* 0xC */ s32 d;
} QueueEntry; /* size 0x10 */

s32 func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);
s32 func_0020185C(s32, s32, s32, s32 *);
s32 *func_002017D4(s32, s32);
void func_0020A568(void);
void func_00205110(s32, s32);
void func_002051F4(s32, s32);
extern u8 D_800A53D8[];
extern u8 D_800A53EC[];
extern void func_0020B4B0();
extern s32 D_800D8E10;
extern s32 D_800D8E14;
extern s32 D_800D8E28;
extern s32 D_800D8E34;
extern s32 D_800DD170;
extern s32 D_800DD178[];
extern QueueEntry D_800DD578[];

void func_0020B9F4(s32 res, s32 arg1, s32 arg2, s32 index, s32 arg4) {
    s32 size;
    s32 file;
    s32 handle;
    QueueEntry *e;
    s32 *count;
    s32 n;

    file = func_0020367C(0, res, 0x18, 0, 0, 0, (s32) D_800A53D8, 0);
    if (file != 0) {
        handle = func_0020367C(0, func_0020185C(*(s32 *) file, res, 1, &size), size, 0, 0,
                               (s32) func_0020B4B0, (s32) D_800A53EC, 0);
        if (handle != 0) {
            count = func_002017D4(*(s32 *) handle, 2);
            if (index >= 0 && index < *count) {
                if (D_800D8E28 != handle || D_800D8E14 != index || D_800D8E10 != arg4 || D_800D8E34 == 32) {
                    func_0020A568();
                    if (D_800DD170 == 256) {
                        goto done;
                    }
                    func_00205110(0, handle);
                    D_800DD178[D_800DD170] = handle;
                    D_800DD170++;
                }
                n = D_800D8E34;
                D_800D8E28 = handle;
                D_800D8E14 = index;
                D_800D8E10 = arg4;
                e = &D_800DD578[n++];
                D_800D8E34 = n;
                e->a = arg1;
                e->d = 0;
                e->b = arg2;
                e->c = 0;
            }
        done:
            func_002051F4(0, handle);
        }
        func_002051F4(0, file);
    }
}
