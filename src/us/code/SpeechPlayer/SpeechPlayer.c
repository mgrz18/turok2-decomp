/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025CC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"
#include "protos/iset.h"

extern void adecClearStopFlag();
extern QueueRec D_800F55F4[];
extern void adecGetStopFlag(void);
extern s32 adecStreamOpen(void *, s32, s32 *);
extern s32 alSndpGetState(char *);
extern void alSndpSetPitch(char *, f32);
extern void alSndpSetVol(char *, s32);
extern void alSndpSetPan(char *, s32);
extern void alSndpSetPriority(char *, s32, s32);
extern void alSndpSetFXMix(char *, s32);
extern void alSndpPlay(char *);

#ifndef TARGET_PORT /* the port defines SpeechPlayer_Update in port/audio/t2audio_world.c */
void SpeechPlayer_Update(PlayerRecAt24 *player) {
    extern void alSndpSetSound(char *, s32);
    extern void *CISet__GetBlock(void *, s32);
    extern char *CAudioWorld__GetSndPlayer(Bank *);
    extern void CAudioWorld__Unlock(Bank *);
    extern void CAudioWorld__Lock(Bank *);
    s32 rate;
    char *channel;
    s32 index;
    SongHeader *header;
    void *sequence;
    CAudioWorld__Lock(player->bank);
    channel = CAudioWorld__GetSndPlayer(player->bank);
    switch (player->state) {
        case 0:
            break;
        case 3:
            adecGetStopFlag();
            break;
        case 2:
            alSndpSetSound(channel, player->id);
            if (alSndpGetState(channel) == 0) {
                adecClearStopFlag();
                if (player->loop >= 0) {
                    player->state = 1;
                } else {
                    player->state = 0;
                }
            }
            break;
        case 1:
            ((s32 *)D_800F55F4)[0] = 0;
            ((u8 *)D_800F55F4)[4] = 2;
            ((s32 *)D_800F55F4)[2] = 0;
            index = player->song * 2;
            header = CISet__GetBlock(player->bank->songs, index);
            player->tempoScale = header->tempo * 0.01f;
            player->volume = header->volume;
            sequence = CISet__GetBlock(player->bank->songs, index | 1);
            ((void **)D_800F55F4)[-1] = sequence;
            sSpeechSound.envelope->decayTime = adecStreamOpen(sequence, 0x5622, &rate);
            player->state = 3;
            alSndpSetSound(channel, player->id);
            alSndpSetPitch(channel, rate * 4.5351473e-05f);
            alSndpSetVol(channel, (s16)(player->speed * player->tempoScale));
            alSndpSetPan(channel, player->priority);
            alSndpSetPriority(channel, player->id, 0x7F);
            alSndpSetFXMix(channel, (u8)(u32)player->volume);
            alSndpPlay(channel);
            player->loop = -1;
            break;
    }
    CAudioWorld__Unlock(player->bank);
}
#endif /* !TARGET_PORT */

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO_2/8025CCB0_2.CPP, func_8025CED0_de, with Turok 2's layouts. */
/* Start a random song of a group on a music player and return the request id (-1 if the group has none). */

#include "common.h"

typedef struct MusicBank {
    /* 0x0000 */ u8 pad0[0x2B60];
    /* 0x2B60 */ s32 table;
    /* 0x2B64 */ s32 count;
} MusicBank;
typedef struct MusicPlayer {
    /* 0x00 */ MusicBank *bank;
    /* 0x04 */ s32 request;
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 group;
    /* 0x10 */ s32 song;
    /* 0x14 */ s32 speed;
    /* 0x18 */ s32 volume;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ s16 id;
} MusicPlayer;
extern s32 BinaryRange(sptr table, s32 count, s32 group, s32 *first, s32 *last);
extern s32 Random_Next(void);                     /* random */
void *CAudioWorld__GetSndPlayer(void *aw);
void CAudioWorld__Lock(void *aw);
void CAudioWorld__Unlock(void *aw);
void alSndpSetSound(void *sndp, s16 id);
void alSndpStop(void *sndp);

/* One song of the group, each with the same weight. */
static __inline__ s32 pick(MusicBank *bank, s32 group) {
    s32 first;
    s32 last;
    s32 i;
    s32 total;
    s32 roll;

    total = 0;
    if (BinaryRange(bank->table, bank->count, group, &first, &last) == 0) {
        return -1;
    }
    if (first != last) {
        for (i = first; i <= last; i++) {
            total += 100;
        }
        roll = Random_Next() % total;
        total = 0;
        for (i = first; i < last; i++) {
            total += 100;
            if (roll <= total) {
                return i;
            }
        }
        return i;
    }
    return first;
}

static __inline__ void restart(MusicPlayer *player) {
    void *channel;

    if (player->state != 2 && player->state != 0) {
        player->state = 2;
        channel = CAudioWorld__GetSndPlayer(player->bank);
        alSndpSetSound(channel, player->id);
        alSndpStop(channel);
    }
}

#ifndef TARGET_PORT /* the port defines SpeechPlayer_Play in port/audio/t2audio_world.c */
s32 SpeechPlayer_Play(MusicPlayer *player, s32 group, f32 scale) {
    s32 song;
    s32 request;

    song = pick(player->bank, (s16) group);
    if (song == -1) {
        return -1;
    }
    CAudioWorld__GetSndPlayer(player->bank);
    CAudioWorld__Lock(player->bank);
    player->group = group;
    player->song = song;
    request = ++player->request;
    pick(player->bank, (s16) player->group);
    player->speed = scale * 32767.0f;
    player->volume = 0x40;
    if (player->state == 3 || player->state == 1) {
        restart(player);
    } else {
        player->state = 1;
    }
    CAudioWorld__Unlock(player->bank);
    return request;
}
#endif /* !TARGET_PORT */

extern s16 alSndpAllocate(void *, void *);

#ifndef TARGET_PORT /* the port defines SpeechPlayer_Construct in port/audio/t2audio_world.c */
void SpeechPlayer_Construct(struct Shape_typemap_110 *record, sptr resource) {
    record->field_0 = resource;
    record->field_4 = -1;
    record->field_C = -1;
    record->field_8 = 0;
    record->field_14 = 0;
    record->field_18 = 0x40;
    record->field_1C = alSndpAllocate(CAudioWorld__GetSndPlayer(record->field_0), &sSpeechSound);
}
#endif /* !TARGET_PORT */

/* What this reads and writes. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 sound;
} SoundSlot;

#ifndef TARGET_PORT /* the port defines SpeechPlayer_Stop in port/audio/t2audio_world.c */
void SpeechPlayer_Stop(SoundSlot *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    CAudioWorld__Lock(arg0->handle);
    temp_v0 = arg0->state;
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        arg0->state = 2;
        temp_v0_2 = CAudioWorld__GetSndPlayer(arg0->handle);
        alSndpSetSound(temp_v0_2, arg0->sound);
        alSndpStop(temp_v0_2);
    }
    CAudioWorld__Unlock(arg0->handle);
}
#endif /* !TARGET_PORT */
s32 SpeechPlayer_PickEntry(void **arg0, s16 id, s16 avoid);
extern s32 BinaryRange(sptr, s32, s32, s32 *, s32 *);

#ifndef TARGET_PORT /* the port defines SpeechPlayer_PickEntry in port/audio/t2audio_world.c */
s32 SpeechPlayer_PickEntry(void **arg0, s16 id, s16 avoid) {
    s32 first;
    s32 last;
    s32 total;
    s32 entry;
    s32 draw;
    ContextRecAt2BB8 *table;
    s32 result;
    total = 0;
    table = *arg0;
    if (BinaryRange(table->unk2B60, table->unk2B64, id, &first, &last) == 0) {
        return -1;
    }
    result = first;
    if (result != last) {
        for (entry = first; entry <= last; entry++) {
            total += 100;
        }
        draw = Random_Next() % total;
        total = 0;
        for (entry = first; entry < last; entry++) {
            total += 100;
            if (total >= draw) {
                break;
            }
        }
        if (avoid != 0 && entry == avoid) {
            if (entry == last) {
                entry--;
            } else {
                entry++;
            }
        }
        result = entry;
    }
    return result;
}
#endif /* !TARGET_PORT */

#include "structs/game_object.h"

/* The block the instance's first word points at; 0x2B50 is a table
 * handle read through CISet__GetBlock. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B50];
    /* 0x2B50 */ s32 table;
} Owner00276F90;
typedef struct {
    /* 0x0 */ u32 value;
    /* 0x4 */ u16 unk4;
} Entry00276F90;

void SpeechPlayer_ReadHeader(CGameObjectInstance *arg0, s32 arg1) {
    f64 var_f2;
    s32 temp_s1;
    s32 temp_v0_2;
    Entry00276F90 *temp_v0;

    temp_s1 = (arg1 << 1);
    temp_v0 = CISet__GetBlock((*(Owner00276F90 **)arg0)->table, temp_s1);
    temp_v0_2 = temp_v0->value;
    var_f2 = (f64) temp_v0_2;
    if (temp_v0_2 < 0) {
        var_f2 += 4294967296.0;
    }
    arg0->m_vVelocity.z = (f32) ((f32) var_f2 * 0.01f);
    arg0->m_vWaterDisplacement.x = (f32) temp_v0->unk4;
    CISet__GetBlock((*(Owner00276F90 **)arg0)->table, temp_s1 | 1);
}

/* A playing sound. */
typedef struct {
    /* 0x00 */ s32 handle;
    /* 0x04 */ u8 pad4[4];
    /* 0x08 */ s32 state;
    /* 0x0C */ u8 padC[0x1E - 0xC];
    /* 0x1E */ s16 unk1E;
} Sound;

#ifndef TARGET_PORT /* the port defines SpeechPlayer_StopUnlocked in port/audio/t2audio_world.c */
void SpeechPlayer_StopUnlocked(Sound *arg0) {
    s32 temp_v0;
    s32 temp_v0_2;

    temp_v0 = arg0->state;
    if ((temp_v0 != 2) && (temp_v0 != 0)) {
        arg0->state = 2;
        temp_v0_2 = CAudioWorld__GetSndPlayer(arg0->handle);
        alSndpSetSound(temp_v0_2, arg0->unk1E);
        alSndpStop(temp_v0_2);
    }
}
#endif /* !TARGET_PORT */
