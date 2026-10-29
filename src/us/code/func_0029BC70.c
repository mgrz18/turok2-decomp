/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/UTIL/802C09A0.CPP, func_802BB8B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef unsigned long long u64;
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
struct OSMesgQueue;
typedef void *OSMesg;
extern s32 osSendMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
struct OSMesgQueue;
struct OSMesgQueue;
struct OSMesgQueue;
extern u32 D_80122A30;
extern void *D_800B8930;
struct OSTimer_s;
typedef struct OSTimer_s OSTimer_s;
struct Opaque_OSMesgQueue;
typedef struct Opaque_OSMesgQueue Opaque_OSMesgQueue;
struct OSTimer_s;
struct OSTimer_s {
    struct OSTimer_s *next;
    struct OSTimer_s *prev;
    u64 interval;
    u64 value;
    Opaque_OSMesgQueue *mq;
    void * msg;
};
extern u32 func_0029DFC0(void);
extern void func_0029BE0C(u64 tim);
extern u64 func_0029BE6C(OSTimer_s *t);
extern void func_0029E210(u32 compare);
void func_0029BC70(void)
{
    OSTimer_s *t;
    u32 count;
    u32 elapsed_cycles;
    u32 *counter;
    if (((OSTimer_s *)D_800B8930)->next == D_800B8930) {
        return;
    }
    counter = &D_80122A30;
    for (;;) {
        t = ((OSTimer_s *)D_800B8930)->next;
        if (t == D_800B8930) {
            func_0029E210(0);
            D_80122A30 = 0;
            break;
        }
        count = func_0029DFC0();
        elapsed_cycles = count - *counter;
        *counter = count;
        if (elapsed_cycles < t->value) {
            t->value -= elapsed_cycles;
            func_0029BE0C(t->value);
            break;
        }
        t->prev->next = t->next;
        t->next->prev = t->prev;
        t->next = 0;
        t->prev = 0;
        if (t->mq != 0) {
            osSendMesg(t->mq, t->msg, 0);
        }
        if (t->interval != 0) {
            t->value = t->interval;
            func_0029BE6C(t);
        }
    }
}
void func_0029BE0C(u64 interval);
