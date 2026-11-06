/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_8.c, func_8025CC90_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
void SpeechPlayer_Update(PlayerRecAt24 *player) {
    extern void alSndpSetSound(char *, s32);
    extern void *CISet__GetBlock(void *, s32);
    extern char *CAudioWorld__GetSndPlayer(Bank *);
    extern void CAudioWorld__Unlock(Bank *);
    extern void CAudioWorld__Lock(Bank *);
    extern func_80203E78_S1 *sSpeechSound;
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
            sSpeechSound->unk4 = adecStreamOpen(sequence, 0x5622, &rate);
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
