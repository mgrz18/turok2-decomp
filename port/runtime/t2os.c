/* t2os: libultra threads / message queues / events on host threads. See t2os.h. */
#include "t2os.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { ST_FREE, ST_STOPPED, ST_READY, ST_RUNNING, ST_WAITING, ST_DEAD };
enum { WAIT_NONE, WAIT_RECV, WAIT_SEND };

typedef struct HThread {
    OSThread *t;
    OSId id;
    OSPri prio;
    int state;
    void (*entry)(void *);
    void *arg;
    pthread_t pt;
    pthread_cond_t cv;
    OSMesgQueue *waitq;
    int waitkind;
    unsigned seq; /* FIFO order among equal priorities */
} HThread;

static pthread_mutex_t sLock = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t sPlatformCv = PTHREAD_COND_INITIALIZER;
static HThread sThreads[T2OS_MAX_THREADS];
static HThread *sCurrent;      /* the thread holding the baton, NULL while the platform runs */
static unsigned sSeq;
static int sSwitches;
static t2os_trace_fn sTrace;
static __thread HThread *tSelf; /* NULL on the platform thread and on any other host thread */

static OSMesgQueue *sEventQueue[T2OS_NUM_EVENTS];
static OSMesg sEventMsg[T2OS_NUM_EVENTS];
static OSMesgQueue *sViQueue;
static OSMesg sViMsg;
static u32 sViCount = 1, sViPhase;

static void fatal(const char *what) {
    fprintf(stderr, "t2os: %s\n", what);
    abort();
}

void t2os_init(void) {
    /* static initialisers do the work; kept as the documented entry point */
}

void t2os_set_trace(t2os_trace_fn fn) {
    sTrace = fn;
}

int t2os_on_game_thread(void) {
    return tSelf != NULL;
}

/* ---- scheduling core (sLock held) ---- */

static HThread *pick(void) {
    HThread *best = NULL;
    for (int i = 0; i < T2OS_MAX_THREADS; i++) {
        HThread *h = &sThreads[i];
        if (h->state != ST_READY && h->state != ST_RUNNING)
            continue;
        if (best == NULL || h->prio > best->prio || (h->prio == best->prio && h->seq < best->seq))
            best = h;
    }
    return best;
}

static void give_baton(HThread *next) {
    HThread *prev = sCurrent;
    sCurrent = next;
    if (next != prev) {
        sSwitches++;
        if (sTrace)
            sTrace(0, prev ? prev->id : -1, next ? next->id : -1);
    }
    if (next != NULL) {
        next->state = ST_RUNNING;
        pthread_cond_signal(&next->cv);
    } else {
        pthread_cond_signal(&sPlatformCv);
    }
}

static void wait_baton(HThread *me) {
    while (sCurrent != me)
        pthread_cond_wait(&me->cv, &sLock);
}

/* me has already been marked READY / WAITING / STOPPED / DEAD: hand over, and unless dead, wait
 * until the scheduler gives the baton back */
static void switch_away(HThread *me) {
    give_baton(pick());
    if (me->state == ST_DEAD)
        return;
    wait_baton(me);
}

/* libultra preempts the running thread only for a thread of strictly higher priority */
static void preempt_check(HThread *me) {
    HThread *best = NULL;
    for (int i = 0; i < T2OS_MAX_THREADS; i++) {
        HThread *h = &sThreads[i];
        if (h->state == ST_READY && (best == NULL || h->prio > best->prio || (h->prio == best->prio && h->seq < best->seq)))
            best = h;
    }
    if (best != NULL && best->prio > me->prio) {
        me->state = ST_READY;
        me->seq = ++sSeq;
        switch_away(me);
    }
}

static void wake_one(OSMesgQueue *mq, int kind) {
    HThread *best = NULL;
    for (int i = 0; i < T2OS_MAX_THREADS; i++) {
        HThread *h = &sThreads[i];
        if (h->state == ST_WAITING && h->waitq == mq && h->waitkind == kind &&
            (best == NULL || h->prio > best->prio || (h->prio == best->prio && h->seq < best->seq)))
            best = h;
    }
    if (best != NULL) {
        best->state = ST_READY;
        best->waitq = NULL;
        best->waitkind = WAIT_NONE;
        best->seq = ++sSeq;
    }
}

static HThread *find(OSThread *t) {
    if (t == NULL)
        return tSelf;
    for (int i = 0; i < T2OS_MAX_THREADS; i++)
        if (sThreads[i].state != ST_FREE && sThreads[i].t == t)
            return &sThreads[i];
    return NULL;
}

/* ---- threads ---- */

static void *trampoline(void *p) {
    HThread *h = p;
    tSelf = h;
    pthread_mutex_lock(&sLock);
    wait_baton(h);
    pthread_mutex_unlock(&sLock);
    h->entry(h->arg);
    /* entry returned: libultra's __osCleanupThread destroys the thread */
    pthread_mutex_lock(&sLock);
    h->state = ST_DEAD;
    give_baton(pick());
    pthread_mutex_unlock(&sLock);
    return NULL;
}

void osCreateThread(OSThread *t, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri) {
    pthread_attr_t attr;
    HThread *h = NULL;
    (void)sp; /* the game's stack is not used: the host thread has its own */
    pthread_mutex_lock(&sLock);
    for (int i = 0; i < T2OS_MAX_THREADS; i++)
        if (sThreads[i].state == ST_FREE) {
            h = &sThreads[i];
            break;
        }
    if (h == NULL)
        fatal("too many threads");
    memset(h, 0, sizeof(*h));
    h->t = t;
    h->id = id;
    h->prio = pri;
    h->entry = entry;
    h->arg = arg;
    h->state = ST_STOPPED;
    pthread_cond_init(&h->cv, NULL);
    if (t != NULL) {
        t->priority = pri;
        t->id = id;
        t->state = 1; /* OS_STATE_STOPPED */
        t->next = NULL;
        t->queue = NULL;
    }
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 8u << 20); /* macOS gives secondary threads 512 KB by default */
    if (pthread_create(&h->pt, &attr, trampoline, h) != 0)
        fatal("pthread_create failed");
    pthread_attr_destroy(&attr);
    pthread_mutex_unlock(&sLock);
}

void osStartThread(OSThread *t) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    if (h != NULL && h->state == ST_STOPPED) {
        h->state = ST_READY;
        h->seq = ++sSeq;
        if (h->t)
            h->t->state = 2; /* OS_STATE_RUNNABLE */
        if (tSelf != NULL)
            preempt_check(tSelf);
    }
    pthread_mutex_unlock(&sLock);
}

void osStopThread(OSThread *t) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    if (h != NULL) {
        if (h == tSelf) {
            h->state = ST_STOPPED;
            switch_away(h);
        } else if (h->state == ST_READY || h->state == ST_WAITING) {
            h->state = ST_STOPPED;
            h->waitq = NULL;
            h->waitkind = WAIT_NONE;
        }
    }
    pthread_mutex_unlock(&sLock);
}

void osDestroyThread(OSThread *t) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    if (h == NULL) {
        pthread_mutex_unlock(&sLock);
        return;
    }
    h->state = ST_DEAD;
    h->waitq = NULL;
    if (h == tSelf) {
        give_baton(pick());
        pthread_mutex_unlock(&sLock);
        pthread_exit(NULL);
    }
    /* another thread: its host thread stays parked on its condition variable (Turok 2 never
     * destroys a live thread; only __osCleanupThread destroys the caller) */
    pthread_mutex_unlock(&sLock);
}

void osYieldThread(void) {
    pthread_mutex_lock(&sLock);
    if (tSelf != NULL) {
        tSelf->state = ST_READY;
        tSelf->seq = ++sSeq;
        switch_away(tSelf);
    }
    pthread_mutex_unlock(&sLock);
}

void osSetThreadPri(OSThread *t, OSPri pri) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    if (h != NULL) {
        h->prio = pri;
        if (h->t)
            h->t->priority = pri;
        if (tSelf != NULL)
            preempt_check(tSelf);
    }
    pthread_mutex_unlock(&sLock);
}

OSPri osGetThreadPri(OSThread *t) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    OSPri p = h ? h->prio : -1;
    pthread_mutex_unlock(&sLock);
    return p;
}

OSId osGetThreadId(OSThread *t) {
    pthread_mutex_lock(&sLock);
    HThread *h = find(t);
    OSId id = h ? h->id : 0;
    pthread_mutex_unlock(&sLock);
    return id;
}

/* ---- message queues ---- */

void osCreateMesgQueue(OSMesgQueue *mq, OSMesg *msg, s32 count) {
    pthread_mutex_lock(&sLock);
    mq->mtqueue = NULL;
    mq->fullqueue = NULL;
    mq->validCount = 0;
    mq->first = 0;
    mq->msgCount = count;
    mq->msg = msg;
    pthread_mutex_unlock(&sLock);
}

static s32 send_locked(OSMesgQueue *mq, OSMesg m, s32 flag, int jam) {
    HThread *me = tSelf;
    while (mq->validCount >= mq->msgCount) {
        if (flag == OS_MESG_NOBLOCK)
            return -1;
        if (me == NULL) {
            fprintf(stderr, "t2os: blocking send from a host thread on a full queue %p: dropped\n", (void *)mq);
            return -1;
        }
        me->state = ST_WAITING;
        me->waitq = mq;
        me->waitkind = WAIT_SEND;
        switch_away(me);
    }
    if (jam) {
        mq->first = (mq->first + mq->msgCount - 1) % mq->msgCount;
        mq->msg[mq->first] = m;
    } else {
        mq->msg[(mq->first + mq->validCount) % mq->msgCount] = m;
    }
    mq->validCount++;
    wake_one(mq, WAIT_RECV);
    if (me != NULL)
        preempt_check(me);
    return 0;
}

s32 osSendMesg(OSMesgQueue *mq, OSMesg m, s32 flag) {
    pthread_mutex_lock(&sLock);
    s32 r = send_locked(mq, m, flag, 0);
    pthread_mutex_unlock(&sLock);
    return r;
}

s32 osJamMesg(OSMesgQueue *mq, OSMesg m, s32 flag) {
    pthread_mutex_lock(&sLock);
    s32 r = send_locked(mq, m, flag, 1);
    pthread_mutex_unlock(&sLock);
    return r;
}

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *m, s32 flag) {
    HThread *me = tSelf;
    pthread_mutex_lock(&sLock);
    while (mq->validCount == 0) {
        if (flag == OS_MESG_NOBLOCK || me == NULL) {
            if (me == NULL && flag != OS_MESG_NOBLOCK)
                fprintf(stderr, "t2os: blocking receive from a host thread on an empty queue %p\n", (void *)mq);
            pthread_mutex_unlock(&sLock);
            return -1;
        }
        me->state = ST_WAITING;
        me->waitq = mq;
        me->waitkind = WAIT_RECV;
        switch_away(me);
    }
    if (m != NULL)
        *m = mq->msg[mq->first];
    mq->first = (mq->first + 1) % mq->msgCount;
    mq->validCount--;
    wake_one(mq, WAIT_SEND);
    if (me != NULL)
        preempt_check(me);
    pthread_mutex_unlock(&sLock);
    return 0;
}

/* ---- events ---- */

void osSetEventMesg(OSEvent e, OSMesgQueue *mq, OSMesg msg) {
    if (e >= T2OS_NUM_EVENTS)
        return;
    pthread_mutex_lock(&sLock);
    sEventQueue[e] = mq;
    sEventMsg[e] = msg;
    pthread_mutex_unlock(&sLock);
}

void osViSetEvent(OSMesgQueue *mq, OSMesg msg, u32 retraceCount) {
    pthread_mutex_lock(&sLock);
    sViQueue = mq;
    sViMsg = msg;
    sViCount = retraceCount ? retraceCount : 1;
    sViPhase = 0;
    pthread_mutex_unlock(&sLock);
}

s32 t2os_post(OSMesgQueue *mq, OSMesg msg) {
    if (mq == NULL)
        return -1;
    pthread_mutex_lock(&sLock);
    s32 r = send_locked(mq, msg, OS_MESG_NOBLOCK, 0);
    pthread_mutex_unlock(&sLock);
    return r;
}

void t2os_post_event(OSEvent e) {
    if (e < T2OS_NUM_EVENTS)
        t2os_post(sEventQueue[e], sEventMsg[e]);
}

void t2os_vi_retrace(void) {
    /* libultra's VI manager posts OS_EVENT_VI to the queue osSetEventMesg registered and the
     * osViSetEvent client every retraceCount retraces */
    t2os_post_event(OS_EVENT_VI);
    if (sViQueue != NULL && ++sViPhase >= sViCount) {
        sViPhase = 0;
        t2os_post(sViQueue, sViMsg);
    }
}

int t2os_run_until_idle(void) {
    if (tSelf != NULL)
        fatal("t2os_run_until_idle called from a game thread");
    pthread_mutex_lock(&sLock);
    int before = sSwitches;
    HThread *b = pick();
    if (b != NULL) {
        give_baton(b);
        while (sCurrent != NULL)
            pthread_cond_wait(&sPlatformCv, &sLock);
    }
    int n = sSwitches - before;
    pthread_mutex_unlock(&sLock);
    return n;
}

u32 __osDisableInt(void) {
    return 1;
}

void __osRestoreInt(u32 mask) {
    (void)mask;
}

void t2os_dump(void) {
    static const char *names[] = { "free", "stopped", "ready", "running", "waiting", "dead" };
    pthread_mutex_lock(&sLock);
    for (int i = 0; i < T2OS_MAX_THREADS; i++) {
        HThread *h = &sThreads[i];
        if (h->state == ST_FREE)
            continue;
        fprintf(stderr, "  thread id %3d pri %3d %-8s waitq %p (%s)\n", (int)h->id, (int)h->prio, names[h->state],
                (void *)h->waitq, h->waitkind == WAIT_RECV ? "recv" : h->waitkind == WAIT_SEND ? "send" : "-");
    }
    pthread_mutex_unlock(&sLock);
}
