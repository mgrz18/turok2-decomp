/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_7.c, func_802B43A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef union {
    Awords words;
    long long int force_union_align;
} Acmd;
typedef Acmd *(*ALCmdHandler)(void *, short *, int, int, Acmd *);
typedef int (*ALDMAproc)(int addr, int len, void *state);
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
struct ALFilter_s14;
typedef struct ALFilter_s14 ALFilter_s14;
struct ALFilter_s14;
struct ALFilter_s14 {
    struct ALFilter_s14 *source;
    ALCmdHandler handler;
    void *setParam;
    s16 inp;
    s16 outp;
    s32 type;
};
struct ALLoadFilter48_2;
typedef struct ALLoadFilter48_2 ALLoadFilter48_2;
struct ALRawLoop;
typedef struct ALRawLoop ALRawLoop;
struct ALRawLoop;
struct ALRawLoop {
    u32 start;
    u32 end;
    u32 count;
};
struct ALLoadFilter48_2;
struct ALLoadFilter48_2 {
    ALFilter_s14 filter;
    void *state;
    void *lstate;
    ALRawLoop loop;
    void *table;
    s32 bookSize;
    ALDMAproc dma;
    void *dmaState;
    s32 sample;
    s32 lastsam;
    s32 first;
    s32 memin;
};
typedef s32 (*FuncPtr)(void *);
extern void func_00296340(void *arg0, void *a, void *b, s32 c);
extern s32 func_002912A0(s32, s32, void *, s32, s32);
extern char D_0029F190;
extern char func_0029FB64;
void func_00295304(void *arg0, FuncPtr arg1, s32 arg2) {
    func_00296340(arg0, &D_0029F190, &func_0029FB64, 0);
    ((ALLoadFilter48_2 *)arg0)->state = func_002912A0(0, 0, arg2, 1, 0x20);
    ((ALLoadFilter48_2 *)arg0)->lstate = func_002912A0(0, 0, arg2, 1, 0x20);
    ((ALLoadFilter48_2 *)arg0)->dma = arg1(&((ALLoadFilter48_2 *)arg0)->dmaState);
    ((ALLoadFilter48_2 *)arg0)->lastsam = 0;
    ((ALLoadFilter48_2 *)arg0)->first = 1;
    ((ALLoadFilter48_2 *)arg0)->memin = 0;
}
