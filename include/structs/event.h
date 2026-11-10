#ifndef STRUCTS_EVENT_H
#define STRUCTS_EVENT_H

/* A level event record: what LevelEvent_Fire fires. It shows a message (an
 * index into the {text, sound} table gLevelMessages), starts a music track,
 * and calls the handler of its kind (gLevelEventHandlers[kind][handler]) on one of
 * the level actors (actor, in level). Fired late, it waits in the world's
 * voice queue first (CScene__QueueDelayedEvent), for its delay. */
typedef struct LevelEvent {
#ifdef TARGET_PORT
    /* 0x00 */ u32 m_Mask;     /* the triggers it answers (Trigger.mask in LevelEvent_ProcessTriggers): a word the archive swaps */
#else
    u8 pad0[4];                /* the same word; as bytes the struct keeps the N64 build's 2-byte alignment */
#endif
    /* 0x04 */ u16 actor;       /* index into the level actors */
    /* 0x06 */ s16 sound;       /* Event_PlaySoundRemote */
    /* 0x08 */ u16 music;       /* track to start (>= 2) */
    /* 0x0A */ u16 level;       /* level the actor lives in */
    /* 0x0C */ u16 delay;       /* packed (HalfToFloat) */
    /* 0x0E */ u8 flags;        /* 2: delay in seconds (x15 frames), 0x10: fired (LevelEvent_Trigger) */
    /* 0x0F */ u8 padF;
    /* 0x10 */ u8 message;      /* index into the {text, sound} table */
    /* 0x11 */ u8 kind;         /* callback table */
    /* 0x12 */ u8 handler;      /* entry in that table */
} LevelEvent; /* size 0x14 */

/* One entry of a gLevelEventHandlers table: the callbacks LevelEvent_Fire calls
 * with the level actor of the event (onActor) or with the event alone (onEvent). */
typedef struct LevelEventHandler {
    void (*onActor)(void *actor, void *ai, LevelEvent *event);
    void (*onEvent)(LevelEvent *event);
} LevelEventHandler; /* size 0x8 */

#endif
