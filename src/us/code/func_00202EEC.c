/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80250938_de_3.c, func_802511AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern void func_0020656C(s32, s32);
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
extern int D_800D8218;
extern int D_800D8DCC;
struct OSMesgQueue;
struct OSMesgQueue;
extern int D_800D8208[];
extern int D_800D8210[];
struct OSMesgQueue;
extern s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 block);
struct OSMesgQueue;
struct OSMesgQueue;
extern s32 D_800D8DA4[];
extern s32 D_800D8DD0;
extern s32 D_800D8DA4[];
extern s32 D_800D8DD0;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32);
void func_00202EEC(void) {
    extern s32 osSendMesg(void *, s32, s32);
    extern s32 D_800D8DB0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    u32 temp_a0;
    u32 temp_v0;
    var_s1 = 0;
    var_s0 = 0;
    do {
        if (D_800D8DA4[var_s0] != 0) {
            if (D_800D8218 != var_s0) {
                var_s1 += 1;
                D_800D8210[var_s0] = 1;
                goto block_7;
            }
            D_800D8208[var_s0] = 0;
            func_0020656C(0, var_s0);
            var_s0 += 1;
        } else {
block_7:
            var_s0 += 1;
        }
    } while (var_s0 < 2);
    if (var_s1 != 0) {
        temp_v0 = func_0029DFF0();
        temp_v1 = D_800D8DCC - 1;
        D_800D8DCC = temp_v1;
        if (temp_v1 != 0) {
            func_0029E010(temp_v0);
            osSendMesg(&D_800D8DB0, 0, 1);
            var_s0_2 = 0;
        } else {
            func_0029E010(temp_v0);
            var_s0_2 = 0;
        }
        if (var_s1 > 0) {
            do {
                osRecvMesg(&D_800D8DD0, 0, 1);
                var_s0_2 += 1;
            } while (var_s0_2 < var_s1);
        }
        temp_a0 = func_0029DFF0();
        temp_v1_2 = D_800D8DCC + 1;
        D_800D8DCC = temp_v1_2;
        if (temp_v1_2 != 1) {
            func_0029E010(temp_a0);
            osRecvMesg(&D_800D8DB0, 0, 1);
            return;
        }
        func_0029E010(temp_a0);
    }
}
