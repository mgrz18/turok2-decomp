/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_19.c, __osInsertTimer.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned int u32;
typedef unsigned long long u64;
typedef void *OSMesg;
typedef struct OSMesgQueue OSMesgQueue;
typedef struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 value;
    OSMesgQueue *mq;
    OSMesg msg;
} OSTimer;
extern OSTimer *D_800B8930;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 mask);
u64 func_0029BE6C(OSTimer *t)
{
    OSTimer *timep;
    u64 tim;
    u32 savedMask = func_0029DFF0();
    timep = D_800B8930->next;
    tim = t->value;
    for (; timep != D_800B8930 && tim > timep->value; timep = timep->next) {
        tim -= timep->value;
    }
    t->value = tim;
    if (timep != D_800B8930) {
        timep->value -= tim;
    }
    t->next = timep;
    t->prev = timep->prev;
    timep->prev->next = t;
    timep->prev = t;
    func_0029E010(savedMask);
    return tim;
}
