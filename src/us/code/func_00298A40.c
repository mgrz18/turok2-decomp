/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B7E2C_eu_padding_A25BC_14.c, func_802B7880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned long long u64;
struct OSMesgQueue;
typedef struct OSMesgQueue OSMesgQueue;
struct OSMesgQueue {
    void **mtqueue;
    void **fullqueue;
    s32 validCount;
    s32 first;
    s32 msgCount;
    void **msg;
};
struct OSMesgQueue;
struct OSMesgQueue;
extern int D_800B7A30;
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
extern s32 func_0029A080(s32, s32);
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern void func_00299F90();
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 D_801213D8;
extern s32 D_801213C0;
extern u8 D_80121420;
extern void func_0029B030(void *, s32, s32);
extern u64 func_0029B170(void);
extern s8 D_801213DC;
extern void func_00298BE0(s32 arg0);
extern void func_00298C98(s32 *arg0, s32 arg1);
extern void func_0029BA80(void *arg0, u64 arg1, u64 arg2, void *arg3, void *arg4);
s32 func_00298A40(s32 arg0, s32 *arg1, s32 arg2) {
    extern char D_801213E0;
    s32 sp20[8];
    s32 sp40[6];
    s32 sp58[2];
    u64 value;
    s32 result;
    s32 *initialized = &D_800B7A30;
    if (*initialized != 0) {
        return 0;
    }
    *initialized = 1;
    value = func_0029B170();
    if (value <= 0x0165A0BBULL) {
        func_0029B030(sp40, (s32)sp58, 1);
        func_0029BA80(sp20, 0x0165A0BCULL - value, 0, sp40, sp58);
        osRecvMesg(sp40, (s32)sp58, 1);
    }
    D_801213DC = 4;
    func_00298BE0(0);
    func_0029A080(1, &D_801213E0);
    osRecvMesg(arg0, (s32)sp58, 1);
    result = func_0029A080(0, &D_801213E0);
    osRecvMesg(arg0, (s32)sp58, 1);
    func_00298C98(arg1, arg2);
    D_80121420 = 0;
    func_00299F90();
    func_0029B030(&D_801213C0, (s32)&D_801213D8, 1);
    return result;
}
