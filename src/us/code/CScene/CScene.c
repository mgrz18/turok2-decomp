#include "common.h"
#include "structs/world.h"
#include "protos/cache.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "globals/engine.h"
#include "protos/iset.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285DB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "structs/player.h"
#include "structs/vector.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285F58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
#include "globals/swoosh.h"
#include "globals/math.h"
#include "gfx.h"
/* WRONG CUT: REAL CScene__LoadLightSettings size 0x18C (splat cut it into CScene__LoadLightSettings + func_0042AB2C). */
#include "common.h"
#include "structs/light.h"
/* the real function is 0x0042AC0C-0x0042B3E4 (size 0x7D8, 502 words); splat cut it into CScene__LoadLevel +
 * D_0042B304. All 502 words match (checked over the real extent). Reusing one scratch pointer for the
 * CPlayerPool__GetFirstActive result and the actor array keeps the last loop's load in $v0, `actor += 0x288` gives the player
 * loop its register order, and the sound switch with one shared call keeps w in $s3. */
#include "common.h"
#include "globals/render.h"
#include "globals/sysrc.h"
#include "globals/audio.h"
#include "structs/table.h"
/* The port: the scene by its typed fields (the view below has N64 offsets: 0x1C =
 * m_GraphicSetsKey, 0x48 = m_pGraphicSets, 0x7C = m_pGraphicSetTypes); with the view the
 * font's graphic set (COnScreen__LoadFontTexture) was never found and no text was drawn. */
#include "structs/world.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_26.CPP, func_8044C108_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
#include "protos/list.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_27.CPP, func_8044C3D4_de, with Turok 2's layouts. */
#include "common.h"
#include "globals/camera.h"
#include "structs/collision.h"
#include "structs/twin_sceneload.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044C7B8_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
/* The port: the runtime regions CGameRegion__Unpack built, at the host stride (as CScene__GetRegion). */
#include "structs/region.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028784C.c, func_8028787C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/scene.h"
/* FAKEMATCH (round 21 audit): 1 register pin (`register ... asm("$n")`); without them:
 * CScene__DecompressGridSection 278 of 324 words (size 0x50c vs 0x510). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028795C.c, func_0028795C, with Turok 2's layouts. */
#include "common.h"
#include "structs/level_actor.h"
/* WRONG CUT: REAL CScene__StreamAllSections size 0x24C (splat cut it into CScene__StreamAllSections + func_0042C0B4). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_13.CPP, func_8044CD8C_de, with Turok 2's layouts. */
#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00287EE8.c, func_00287EE8, with Turok 2's layouts. */
#include "common.h"
#include "structs/intelligence.h"
#include "globals/strings.h"
#include "structs/rect.h"
/* do { } while (0) blocks around the region setup and the second table lookup raise those pseudos' priority (GCC weights refs by loop depth) without a scheduling barrier at the call's argument moves; RW-shaped object loop (off = k), do/while loops with explicit k = 0 before the count load. */
/* Loop shapes after thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
#include "gbi_t2.h"
#include "structs/camera.h"
/* FAKEMATCH (round 21 audit): 1 register pin (`register ... asm("$n")`); without them: CScene__DrawGeometry
 * 54 of 385 words. */
/* The do { } while (0) around the region-loop setup raises the region counter's priority (GCC weights refs by loop depth) so it gets fp and the object-table base is spilled, as in the ROM. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00289054.c, func_00289054, with Turok 2's layouts. */
#include "common.h"
/* WRONG CUT: REAL CScene__FindWarpDests size 0x228 (splat cut it into CScene__FindWarpDests + func_0042C2DC + func_0042C338 + func_0042C38C). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D220_de, with Turok 2's layouts. */
#include "common.h"
/* FAKEMATCH (round 21 audit): 1 register pin (`register ... asm("$n")`) and 1 empty `__asm__` barrier;
 * without them: CScene__CheckAutoActivation 14 of 263 words. */
#include "structs/vector.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028A258.c, func_0028A258.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "protos/ai.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SCENE/CODE_80289790.CPP, Scene_UpdateSelectionTransition, with Turok 2's layouts. */

#include "common.h"
#include "m2c_macros.h"
#include "structs/cache.h"
/* The two inlined helpers after AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD/8028E6FC.CPP, func_8028E860_de and func_8028E78C_de (Blast_GetStrength and Blast_IsLivingEnemyInRange here), with Turok 2's layouts. */

#include "common.h"
#include "protos/player.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B004.c, func_0028B004.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B1F8.c, func_0028B1F8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B274_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B2F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B350_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/region.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C.CPP, func_8028B670_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B964.c, func_0028B964.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BAF8.c, func_0028BAF8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BBE8.c, func_0028BBE8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BC98.c, func_0028BC98.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028BD88.CPP, func_8028BDAC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/iset.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028BFB4.c, func_0028BFB4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C1B0.c, func_0028C1B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C26C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/event.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028C6B0.c, func_0028C6B0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CAE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CCDC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* De-faked in round 21 (j4): the N64 body used to test an uninitialised local
 * (`if (temp_s2)` with two identical arms, which clang -O2 on the host compiled to a
 * trap, round 7 v4) and had a separate host body; the plain loop matches. */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_6.CPP, func_8028CDF0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CE70.c, func_0028CE70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028CF7C.c, func_0028CF7C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D0E4.c, func_0028D0E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* WRONG CUT: REAL CScene__FindNearestWarpDest size 0x1A0 (splat cut it into CScene__FindNearestWarpDest + func_0042C620 + func_0042C6BC). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_29.CPP, func_8044D3E8_de, with Turok 2's layouts. */
#include "common.h"
/* WRONG CUT: the function starts at 0x0042C758 (versions/cuts.us.txt calls its two first words,
 * the prologue addiu sp,-24 / sw s0, a stray; the body at 0x42C760 saves ra at 20(sp) and pops 24).
 * REAL CScene__SetupLighting size 0x44 at CScene__RequestWarp+0x58 */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_30.CPP, func_8044D5C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/vector.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E2B8.c, func_0044E2B8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/vector.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E2B8.c, func_0044E3E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/list.h"
#include "protos/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8044D668_de.c, func_8044D804_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D268.c, func_0028D268.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_8.CPP, func_8028D74C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/game_object.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D7A0.c, func_0028D7A0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028DC18.c, func_0028DC18.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023F43C_de_7.c, func_802405FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E768.c, func_0028E768.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028E83C.c, func_0028E83C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* WRONG CUT: REAL CScene__InitPersistentData size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"
/* WRONG CUT: REAL CScene__LoadBitSections2 size 0x4C (splat cut it into CScene__LoadBitSections2 + func_0042C480). */
#include "common.h"
/* WRONG CUT: REAL CScene__ConstructMap size 0x174 (splat cut it into CScene__ConstructMap + func_0042CEB0; the word after it, a nop, is padding). */
#include "common.h"

void MtxPool_Reset(void *pool);
void guMtxIdent(void *);

/* Sets every block of the pool (arg2 blocks at arg1) to identity; two halves of arg2 / 2. */
VM_TEXT void MatrixHeap_Construct(MtxPool *arg0, MtxBlock *arg1, s32 arg2) {
    MtxBlock *var_s0;
    s32 var_s1;

    var_s1 = 0;
    arg0->base = arg1;
    arg0->n = (s32) (arg2 / 2);
    if (arg2 > 0) {
        var_s0 = arg1;
        do {
            guMtxIdent(var_s0);
            var_s1 += 1;
            var_s0 += 1;
        } while (var_s1 < arg2);
    }
    MtxPool_Reset(arg0);
}

/* The request CCache__LoadPersistantSubBlock leaves in v0 (protos/cache.h declares the N64 definition
 * void): the N64 symbol by another C name, returning it; the port's definition returns it. */
#ifdef TARGET_PORT
#define CCache__LoadPersistantSubBlock_v(...) ((void **) CCache__LoadPersistantSubBlock(__VA_ARGS__))
#else
void **CCache__LoadPersistantSubBlock_v() __asm__("CCache__LoadPersistantSubBlock");
#endif

/* Level start: resets the world, opens the cartridge index and the level resource tables (levels, texture
 * sets, object types, objects, intelligence, particle effects, binaries, swooshes, lights, curves, rumbles,
 * graphic sets, cinemas, link groups), then loads five geometry headers (objects 0xDAC-0xDB0). */

s32 ProPack__Decompress(s32 unused, void **res);
void Model_DecompressCallback(sptr arg0, sptr arg1);
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 count;
    /* 0x8 */ u8 entries[1];
} Table00224D54;
typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s32 unk18;
    /* 0x1C */ u8 pad1C[0x44 - 0x1C];
    /* 0x44 */ s32 unk44;
    /* 0x48 */ u8 pad48[0x78 - 0x48];
    /* 0x78 */ Table00224D54 *table;
} Unk00224D54;
sptr CScene__LoadBinaryBlock(Unk00224D54 *arg0, s32 arg1, s32 arg2, s32 arg3);
typedef CScene Owner;
sptr CScene__LoadGraphicSet(Owner *owner, s32 id, s32 flags, s32 arg3);
void CScene__SpawnPoolActor(CScene *lv, s32 a1, CVector3 v, s32 x, f32 f);
void CScene__GetTextureSet(CScene *arg0, s32 arg1, s32 *arg2, sptr arg3);
s32 CScene__GetObject(CScene *arg0, s32 arg1);
void CScene__GetRegionEventRecords(CScene *lv, u16 *idp, CSceneRecord **outRecs, s32 *outCount);
void CScene__GetInstanceEventRecords(CScene *arg0, CGameObjectInstance *arg1, CSceneRecord **arg2, s32 *arg3);
void CScene__FireEventsByKey(void *arg0, s32 arg1);
s32 CScene__AreEventsDone(char *object, s32 type);
void CScene__FireEventsByKeyOnce(void *arg0, s32 arg1);
void CScene__QueueDelayedEvent(CScene *scene, LevelEvent *event);
void CScene__UpdateDelayedEvents(CScene *scene);
void CScene__GetLightsAt(CScene *lv, CVector3 *a1, CLights1 *out);
void CScene__FileInstance(CScene *arg0, CGameObjectInstance *arg1);
CGameObjectInstance *CScene__FindNearestActor(CScene *world, CGameObjectInstance *self, s32 team, s32 kind, s32 activeOnly);
CGameObjectInstance *CScene__FindNearestIdleThrowable(CScene *world, CGameObjectInstance *target);
void *CScene__FindNearestCover(void *arg0, void *arg1);
CGameObjectInstance *CScene__FindNearestIdleThrowableB(CScene *world, CGameObjectInstance *target);
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CC34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * The draft had only a prototype; body written from the Turok 2 asm. */

typedef struct ActorRec {
    char pad[0x1A8];
    struct ActorRec *link; /* 0x1A8 (0x1D8 in Rage Wars) */
} ActorRec;
typedef struct ActorList {
    char pad[0xBF8];
    ActorRec *actors[0x80]; /* 0xBF8 */
    int count;              /* 0xDF8 */
} ActorList;
ActorRec *CScene__ReplaceLeader(ActorList *obj, ActorRec *id);
/* A table of up to 0x80 actors at 0xBF8 with its count at 0xDF8. */
typedef struct {
    /* 0x000 */ u8 pad0[0xBF8];
    /* 0xBF8 */ CGameObjectInstance *actors[0x80];
    /* 0xDF8 */ s32 count;
} ActorTable;
void CScene__DetachChildren(ActorTable *arg0, s32 arg1);
void CScene__DamageAllEnemies(CScene *world, s32 damage, s32 flags);
void CScene__BroadcastNoise(CScene *scene, s32 kind, CVector3 *pos);
void CScene__MarkGeneratorsSaved(void *arg0);
void CScene__UnmarkGeneratorsSaved(void *arg0);
void *CScene__GetCurve(CScene *arg0, s32 arg1);
void CScene__StartRumbles(CScene *lv, sptr a1, s32 id, CVector3 v, f32 f6, f32 f7);
sptr CScene__GetIntelligence(CScene *scene, s32 arg1);
typedef struct { s32 key; u8 pad4[8]; s16 unkC; } Ent;
Ent *CScene__FindIntelligence(CScene *lv, s32 a, s32 b);
s32 CScene__FindInstances(void *arg0, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max);
void CScene__DrawWarpFade(CScene *lv);
void * CScene__GetPathHeader(CScene *arg0, s32 arg1);
void *CScene__GetPathPoint(CScene *arg0, s32 arg1, s32 arg2);
s32 CScene__FindNearestWarpDest(void *world, void *position, s32 type, SearchPoint *out);
void CScene__RequestWarp(CScene *arg0, s32 arg1, s32 arg2);
void CScene__SetupLighting(CScene *menu);
void CScene__RestoreFiredEvents(CScene *w);
void CScene__ReapplyFiredEvents(CScene *w);
void CScene__FlushDelayedEvents(CScene *scene);
typedef struct LevelState {
    /* 0x00 */ u8 pad0[0x98];
    /* 0x98 */ void *handle98;
    /* 0x9C */ void *handle9C;
    /* 0xA0 */ void *handleA0;
    /* 0xA4 */ void *handleA4;
    /* 0xA8 */ void *handleA8;
    /* 0xAC */ void *handleAC;
    /* 0xB0 */ void *handleB0;
    /* 0xB4 */ void *handleB4;
    /* 0xB8 */ u8 padB8[0x10];
    /* 0xC8 */ void *handleC8;
    /* 0xCC */ void *handleCC;
} LevelState;
void CScene__UnloadLevel(LevelState *level);
void CScene__DecompressParticles(sptr arg0, void ***arg1);
s32 CScene__FindNearestPathPoint(CScene *lv, s32 unused, CVector3 *pt);
void CScene__DecompressIntelligence(sptr unused, void ***resource);
s32 CScene__FindObjectType(CScene *lv, s32 id);
void CScene__GetLevelName(struct CScene *arg0, s32 arg1, void *arg2, s32 arg3);
void CScene__ClearListCounts(CScene *arg0);
extern void CScene__LoadLevel(CScene *w, s32, s32);
void CScene__InitPersistentData(CScene *menu);
void CScene__ConstructWarp(CScene *arg0);
void CPlayerPool__LevelStart(void *pool);

/* Copies the header of object id's geometry (or clears it when the object is missing). */
VM_TEXT static __inline__ void loadGeomHeader(CScene *w, CGeomHeader *out, s32 id) {
    s32 size;
    s32 index;
    s32 file;
    void **objIndex;
    void **geometry;

    index = CScene__FindObjectType(w, id);
    if (index == -1) {
        out->m_Data[2] = 0;
        return;
    }
    file = CScene__GetObject(w, index);
    objIndex = CCache__Request(0, file, 0x18, 0, 0, NULL, "object index", 1);
    if (objIndex != NULL) {
        geometry = CCache__Request(0, CISet__GetROMAddressAndSize((sptr)*objIndex, file, 1, &size), size, 0, 0, Model_DecompressCallback,
                                 "geometry", 1);
        if (geometry != NULL) {
            *out = *(CGeomHeader *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*geometry, 2), 0), 0), 0);
            CCache__Deallocate(0, geometry);
        }
        CCache__Deallocate(0, objIndex);
    }
}

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__ClearListCounts receives CScene__Construct's own argument
 * registers (pass-through); the host passes them. */
VM_TEXT void CScene__Construct(CScene *w, s32 file) {
    sptr cart;
    sptr res;
    f32 *vec;

    CScene__ClearListCounts(w);
    w->m_Mode = 0;
    CScene__ConstructWarp(w);
    w->unk_04 = 0;
    w->unk_08 = 0;
    w->m_nLevel = -1;
    w->unk_18EB8 = 0;
    w->m_SkyAngle = 0;
    CPlayerPool__LevelStart(&gEngineApp_m_Game.m_PlayerList);
    vec = &w->m_qSky.x;
    w->m_qSky.x = vec[1] = vec[2] = 0.0f;
    w->m_qSky.w = 1.0f;
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    CCache__LoadPersistantIndex(0, &cart, file, "cartridge index");
    res = CISet__GetROMAddress(cart, file, 5);
    w->m_LevelsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pLevels, res, "levels index");
    res = CISet__GetROMAddress(cart, file, 3);
    w->m_TextureSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pTextureSets, res, "texture sets index");
    w->m_pObjectTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 2, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 0);
    w->m_ObjectsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pObjects, res, "objects index");
    w->m_pIntelligence = *CCache__LoadPersistantSubBlock_v(0, cart, 1, file, w, CScene__DecompressIntelligence, "intelligence");
    w->m_pParticleEffects = (s32 *)CCache__LoadPersistantSubBlock_v(0, cart, 4, file, w, CScene__DecompressParticles, "particle effects");
    w->m_pObjectTypes2 = *CCache__LoadPersistantSubBlock_v(0, cart, 0xA, file, w, NULL, "object types");
    res = CISet__GetROMAddress(cart, file, 9);
    w->m_BinariesKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pBinaries, res, "binaries index");
    w->m_pSwooshes = *CCache__LoadPersistantSubBlock_v(0, cart, 0xB, file, w, NULL, "particle swooshes");
    w->m_pLights = *CCache__LoadPersistantSubBlock_v(0, cart, 0xC, file, w, NULL, "dynamic lights");
    w->m_pBitSections = gLoadSavePersistentData;
    w->unk_28 = CISet__GetROMAddressAndSize(cart, file, 6, &w->unk_30);
    w->m_pBitSections2 = gGamePersistentData;
    w->unk_2C = CISet__GetROMAddressAndSize(cart, file, 7, &w->unk_34);
    w->m_pCurves = *CCache__LoadPersistantSubBlock_v(0, cart, 0xD, file, w, ProPack__Decompress, "curves");
    w->m_pRumbles = *CCache__LoadPersistantSubBlock_v(0, cart, 0xE, file, w, ProPack__Decompress, "rumbles");
    w->m_pRumbleTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0xF, file, w, ProPack__Decompress, "rumble types");
    w->m_pGraphicSetTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0x11, file, w, NULL, "graphicset type");
    res = CISet__GetROMAddress(cart, file, 0x10);
    w->m_GraphicSetsKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pGraphicSets, res, "graphicset index");
    res = CISet__GetROMAddress(cart, file, 0x12);
    w->m_CinemasKey = res;
    CCache__LoadPersistantIndex(0, (sptr *)&w->m_pCinemas, res, "Cinemas index");
    w->m_pLinkGroupTypes = *CCache__LoadPersistantSubBlock_v(0, cart, 0x13, file, w, NULL, "link group types");
    ((void (*)(CScene *, s32)) CScene__InitPersistentData)(w, file); /* the definition takes one */
    gPersistentDataLoaded = 1;
    loadGeomHeader(w, &w->m_Geom[0], 0xDAC);
    loadGeomHeader(w, &w->m_Geom[1], 0xDAD);
    loadGeomHeader(w, &w->m_Geom[2], 0xDAE);
    loadGeomHeader(w, &w->m_Geom[3], 0xDAF);
    loadGeomHeader(w, &w->m_Geom[4], 0xDB0);
    CScene__LoadLevel(w, 0, 0);
}

/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__SetInstanceSavedBit(CScene *arg0, void *arg1, s32 arg2);

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */


/* Set or clear the actor's bit in the current level's saved-actor bitmap. */
void CScene__SetInstanceSavedBit(CScene *arg0, void *arg1, s32 arg2) {
    CGameObjectInstance *entry;
    CIntelligence *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 level;
    s32 mask;
    sptr index;
    sptr idx;
    sptr idx2;
    s32 lastOffset;
    uptr first;
    if (!(((CGameObjectInstance *)arg1)->m_dwFlags & 0x80000)) {
        first = (uptr)arg0->m_pActors;
        index = -1;
        if ((uptr) arg1 >= first) {
            lastOffset = arg0->m_nActors * (s32)sizeof(CGameObjectInstance);
            lastOffset -= (s32)sizeof(CGameObjectInstance);
            if (first + lastOffset >= (uptr) arg1) {
                index = ((uptr) arg1 - first) / (s32)sizeof(CGameObjectInstance);
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return;
    }
    level = arg0->m_nLevel;
    if (arg0->m_Mode == 2) {
        return;
    }
    entry = (CGameObjectInstance *)&arg0->m_pActors[index];
    cond = entry->header.m_pIntelligence;
    if ((u32) (cond->m_Type - 9) < 2) {
        if (cond->m_dwFlags & 0x200) {
            return;
        }
    }
    field80 = arg0->m_pBitSections;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), level), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    base1 = (u8 *) CISet__GetBlock(temp_v0, 1);
    base2 = base1;
    mask = 1 << (index & 7);
    if (arg2 != 0) {
        idx = index;
        if (index < 0) {
            idx = index + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = index;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}
s32 CScene__GetInstanceSavedBit(void *arg0, void *arg1);

s32 CScene__GetInstanceSavedBit(void *arg0, void *arg1) {
    CScene *world = arg0;
    CGameObjectInstance *actor = arg1;
    void *field80;
    void *temp_v0;
    u8 *base;
    s32 field1B40C;
    s32 mask;
    sptr index;
    sptr i;
    s32 lastOffset;
    uptr first;
    if (!(actor->m_dwFlags & 0x80000)) {
        first = ((uptr)world->m_pActors);
        index = -1;
        if ((uptr) arg1 >= first) {
            lastOffset = world->m_nActors * (s32)sizeof(CGameObjectInstance);
            lastOffset -= (s32)sizeof(CGameObjectInstance);
            if (first + lastOffset >= (uptr) arg1) {
                index = ((uptr) arg1 - first) / (s32)sizeof(CGameObjectInstance);
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return 0;
    }
    field80 = world->m_pBitSections;
    field1B40C = world->m_nLevel;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), field1B40C), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    base = (u8 *) CISet__GetBlock(temp_v0, 1);
    mask = 1 << (index & 7);
    i = index;
    if (i < 0) {
        i += 7;
    }
    return (base[i >> 3] & mask) != 0;
}
void CScene__AdvanceListedInstances(void *arg0);
void CScene__AdvanceInstances(CScene *world);
void CScene__BuildActorLists(CScene *w);
void CScene__CollectSectorObjects(CScene *world);
void CScene__LoadVisibleGridSections(CScene *world);
s32 CScene__GetInstanceIndex(void *arg0, void *arg1);
void CScene__GetEventRecords(CScene *lv, s32 which, s32 id, CSceneRecord **outRecs, s32 *outCount);
void CScene__ReleaseSectors(CScene *scene);
void CScene__Update(CScene *world);
void CScene__UpdateWarp(CScene *scene);
/* The input state the controller reader fills (gEngineApp_m_Settings, 0x184 bytes). */

/* The camera pool as bytes (globals/camera.h declares the CCameraPool): this view keeps the ROM's code. */
extern u8 gCameraPoolBytes[] __asm__("gEngineApp_m_Game_m_CameraPool");
void RibbonVtxPool_Reset(void *);
void CPlayerPool__PreUpdate(void *);
void CPlayerPool__AnimateWeapons(void *);
void CCameraPool__Draw(void *, CScene *);
void CParticleSystem__Update(CParticleSystem *);
void SwooshSystem_Update(void *);
void CLevelActorPool__Update(CLevelActorPool *);
f32 func_002982F0(f32);     /* sinf */
f32 func_002974C0(f32);     /* cosf */

void CScene__Update(CScene *world) {
    GameSettings *input;
    s32 old;
    f32 s;

    old = gLitVerticesKey;
    input = &gEngineApp_m_Settings;
    gSceneGeometryDL = 0;
    gLitVerticesKey = input->unkC;
    if (input->cheatFlags & 0x4000) {
        gLitVerticesKey |= 0x100;
    }
    if (input->cheatFlags & 0x8000) {
        gLitVerticesKey |= 0x200;
    }
    if (old != gLitVerticesKey) {
        gLitVerticesKeyChanged = 1;
        ((void (*)(s32, s32)) CCache__SetFrameDelay)(8, old); /* the definition takes one argument */
    } else {
        gLitVerticesKeyChanged = 0;
    }
    CScene__UpdateWarp(world);
    CScene__ClearListCounts(world);
    MtxPool_Reset(world->m_Heap);
    CCache__LockForDisplayList(0, world->m_hHeap);
    RibbonVtxPool_Reset(&gEngineApp_m_SwooshSystem);
    CPlayerPool__PreUpdate(&gEngineApp_m_Game.m_PlayerList);
    CScene__BuildActorLists(world);
    CScene__LoadVisibleGridSections(world);
    CScene__CollectSectorObjects(world);
    if (world->m_WarpState == 0) {
        CPlayerPool__AnimateWeapons(&gEngineApp_m_Game.m_PlayerList);
    }
    CCameraPool__Draw(gCameraPoolBytes, world);
    CParticleSystem__Update(&world->m_ParticleSystem);
#ifdef TARGET_PORT
    /* The port: GameSettings.paused by member (CGame + 0x40 + 0x13F4 on the N64; on the host the
     * literal offset read inside the camera pool and every level stood still: no AdvanceInstances,
     * no CPlayerPool__Update, no player input). */
    if (world->m_WarpState == 0 && gEngineApp_m_Settings.paused == 0) {
#else
    if (world->m_WarpState == 0 && *(s32 *)(gCameraPoolBytes + 0x13F4) == 0) {
#endif
        SwooshSystem_Update(&gEngineApp_m_SwooshSystem);
        CScene__AdvanceInstances(world);
        CScene__AdvanceListedInstances(world);
        CLevelActorPool__Update(&world->m_ActorPool);
        world->m_SkyAngle += gFrameIncrement * 0.087266475f;
        s = func_002982F0(world->m_SkyAngle * 0.5f);
        gQuaternionSin = s;
        world->m_qSky.x = 0;
        world->m_qSky.y = s;
        world->m_qSky.z = 0;
        world->m_qSky.w = func_002974C0(world->m_SkyAngle * 0.5f);
        CScene__UpdateDelayedEvents(world);
    }
    CScene__ReleaseSectors(world);
    CScene__DrawWarpFade(world);
}

#ifdef TARGET_PORT
#include "structs/world.h"
#include "structs/level_actor.h"
/* The port: the list is CScene.m_nDeferred / m_pDeferred (the caller passes the world). The N64
 * reads them at 0x1064 / 0x1068; on the host those offsets fall elsewhere in CScene, so no
 * translucent (flag 4) instance or static object was ever drawn (the Iguana cinema's iguanas:
 * round 9 x3, docs/port/FIDELITY.md). An item is a CStaticObject (type 0: its box) or an
 * instance (its header position); the views at the host offsets. */
typedef struct Drawable {
    union {
        struct { u8 type; };                                   /* 0x00: 0 box, 1 point, 3 ... */
        struct { u8 _hp_pos[__builtin_offsetof(CInstanceHeader, m_vPos)]; CVector3 pos; }; /* N64 0x04 */
        struct { u8 _hp_min[__builtin_offsetof(CStaticObject, m_Bounds.m_vMin)]; CVector3 min; }; /* N64 0x68 */
        struct { u8 _hp_max[__builtin_offsetof(CStaticObject, m_Bounds.m_vMax)]; CVector3 max; }; /* N64 0x74 */
    };
} Drawable;

typedef struct SortEntry {
    Drawable *item;             /* 0x00 */
    f32 dist;                   /* 0x04: squared, doubled */
} SortEntry;

typedef struct DrawList {
    union {
        struct { u8 _hp_count[__builtin_offsetof(CScene, m_nDeferred)]; s32 count; }; /* N64 0x1064 */
        struct { u8 _hp_items[__builtin_offsetof(CScene, m_pDeferred)]; Drawable *items[0x40]; }; /* N64 0x1068 */
    };
} DrawList;
#else
typedef struct Drawable {
    u8 type;                    /* 0x00: 0 box, 1 point, 3 ... */
    u8 pad1[3];
    CVector3 pos;                   /* 0x04 */
    u8 pad10[0x68 - 0x10];
    CVector3 min;                   /* 0x68 */
    CVector3 max;                   /* 0x74 */
} Drawable;

typedef struct SortEntry {
    Drawable *item;             /* 0x00 */
    f32 dist;                   /* 0x04: squared, doubled */
} SortEntry;

typedef struct DrawList {
    u8 pad0[0x1064];
    s32 count;                  /* 0x1064 */
    Drawable *items[1];         /* 0x1068 */
} DrawList;
#endif

CVector3 CCamera__GetViewOrigin(void *cam);  /* camera position */
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
void func_002267A0(CScene *arg0, s32 arg1, s32 arg2, s32 arg3);
void CScene__SetSavedBit(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
s32 CScene__GetSavedBit(CScene *lv, s32 sel, s32 c, s32 b, s32 bit);
/* Only the field read here: a resource walked with CISet__GetBlock. */
typedef struct {
    /* 0x00 */ u8 pad0[0x70];
    /* 0x70 */ s32 *unk70;
} BitTableOwner;
s32 CScene__GetLevelBitInLevel(BitTableOwner *arg0, s32 arg1, s32 arg2);
s32 CScene__GetInstanceSavedBitInLevel(CScene *arg0, s32 arg1, s32 arg2);
#ifdef TARGET_PORT
/* the port: the scene's bit sections at their host offset (CScene.m_pBitSections) */
#include "structs/world.h"
typedef struct {
    u8 pad0[__builtin_offsetof(CScene, m_pBitSections)];
    s32 *data;
} Unk00226B34;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x6C];
    /* 0x6C */ s32 *data;
} Unk00226B34;
#endif
s32 CScene__IsLevelActorKilledInLevel(Unk00226B34 *arg0, s32 arg1, s32 arg2);
s32 CScene__IsEventFiredInLevel(CScene *arg0, s32 arg1, s32 arg2);
sptr CScene__GetLinkGroupCountPtr(CScene *arg0, s32 arg1);
void SortRecord_Swap(RenderSortRecord *left, RenderSortRecord *right);
void Render_ResetBatches(void);
void Render_FlushBatches(void);
void CStaticObject__Draw(void *object, void *camera);
void CGameObjectInstance__Draw(void *actor, void *camera);
void CLevelActor__Draw(void *actor);
typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ f32 key;
} SortItem;
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ f32 key;
} SortEntry_00226D0C;
s32 SortRecord_CompareDescending(SortEntry_00226D0C *arg0, SortEntry_00226D0C *arg1);
s32 SortRecord_Compare(SortItem *arg0, SortItem *arg1);

/* Sorts the list by distance from the camera, then draws it. */
void CScene__DrawSortedList(DrawList *list, void *cam) {
    SortEntry entries[0x800];
    s32 count;
    s32 i;
    f32 cx, cy, cz;
    f32 dx, dy, dz;
    Drawable *item;

    count = list->count;
    if (count == 0) {
        return;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB040004;
        g->w1 = 1;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB04000C;
        g->w1 = 1;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB040014;
        g->w1 = 0xFFFF;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDB04001C;
        g->w1 = 0xFFFF;
    }
    cx = CCamera__GetViewOrigin(cam).x * -2.0f;
    cy = CCamera__GetViewOrigin(cam).y * -2.0f;
    cz = CCamera__GetViewOrigin(cam).z * -2.0f;
    for (i = 0; i < count; i++) {
        item = list->items[i];
        switch (item->type) {
        case 0:
            dx = item->min.x + item->max.x + cx;
            dy = item->min.y + item->max.y + cy;
            dz = item->min.z + item->max.z + cz;
            break;
        case 1:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        case 3:
        default:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        }
        entries[i].item = item;
        entries[i].dist = dx * dx + dy * dy + dz * dz;
    }
    Sort_Quick(entries, count, sizeof(SortEntry), SortRecord_Compare, SortRecord_Swap);
    Render_ResetBatches();
    for (i = 0; i < count; i++) {
        item = entries[i].item;
        switch (item->type) {
        case 0:
            CStaticObject__Draw(item, cam);
            break;
        case 3:
            CLevelActor__Draw(item);
            break;
        case 1:
            CGameObjectInstance__Draw(item, cam);
            break;
        }
    }
    Render_FlushBatches();
}

/* The level's light settings (colours as bytes, direction as signed bytes). */


extern f32 func_00298470(f32);
void CVector3__Scale(void *out, void *in, f32 scale);

/* Loads the level's light colours into the scene light and its direction, normalised to 127, into the menu. */
VM_TEXT void CScene__LoadLightSettings(CScene *menu) {
    s32 i;
    f32 dir[3];
    f32 len;

    for (i = 0; i < 3; i++) {
        gSceneLights.m_AmbColCopy[i] = gSceneLights.m_AmbCol[i] = menu->m_pLevelHeader->m_AmbientColor[i];
        gSceneLights.m_ColCopy[i] = gSceneLights.m_Col[i] = menu->m_pLevelHeader->m_LightColor[i];
        gSceneLights.m_Dir[i] = menu->m_pLevelHeader->m_LightDir[i];
    }
    dir[0] = menu->m_pLevelHeader->m_LightDir[0];
    dir[1] = menu->m_pLevelHeader->m_LightDir[1];
    dir[2] = menu->m_pLevelHeader->m_LightDir[2];
    len = func_00298470(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
    if (len != 0.0f) {
        CVector3__Scale(dir, dir, 127.0f / len);
        menu->m_LightDirection[0] = dir[0];
        menu->m_LightDirection[1] = dir[1];
        menu->m_LightDirection[2] = dir[2];
    }
}
void CScene__DecompressAnimatedInstances(CScene *world, void *handleArg);
void CScene__DecompressCollision(CollisionBuildWorld *world, CollisionBuildMeshHandle **handle);
void CScene__DecompressLinks(void *worldArg, void ***handle);
s32 func_00226D54(void *arg0);
void *CScene__GetRumble(CScene *arg0, s32 arg1);
void CScene__DecompressPaths(CScene *level, s32 ***handle);
void CScene__LoadLevel(struct CScene *, s32, s32);
void CScene__StreamAllSections(CScene *level);
#ifdef TARGET_PORT
#include "globals/onscreen.h"
#endif

extern void CCollisionInfo__Reset(void);
extern void CDecompressor__Flush(CDecompressor *thisx);
void CGameObjectInstance__UpdateAnimation(void *actor);
extern void CGameObjectInstance__InitAIType(u8 *actor);
void CGameObjectInstance__UpdateAI(void *actor);
extern void Sound__StopFlags(s32);
extern void Sound__DetachAll(void);
extern void LoopSounds_Construct(void *, void *, s32);
extern void LoopSounds_StopAll(sptr);
extern s32 Demo_GetPlayerCount(void);
extern void CCameraPool__Advance(void *);
extern s32 Cinema_IsActive(void);
extern void Cinema_RequestOnce(s32 sound);
extern void SceneActorPool_Construct(CLevelActorPool *);
extern void CInstancePool__Construct(void *);
extern void CDLightSet__Construct(void *);
/* the particle data is a pointer: m_pParticleEffects is the cache handle (read pointer-wide) */
extern void CParticleSystem__Construct(CParticleSystem *, sptr);
extern void SwooshSystem_Construct(void *);
extern void CPlayer__StopLoopingSounds(void *);
extern void COnScreen__ResetBossBars(void *);
extern void CCameraPool__ResetEmitterPools(void *);
#ifdef TARGET_PORT
void T2Port_RngAtLoad(int level);
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS CScene__FlushDelayedEvents receives CScene__LoadLevel's own argument
 * registers (pass-through); the host passes them. */
VM_TEXT void CScene__LoadLevel(CScene *w, s32 level, s32 announce) {
    void **block;
    u8 *data;
    u8 *player;
    u8 *actor;
    u8 *p;
    u8 *ptr; /* scratch pointer: the leaving player's state, then the actor array */
    s32 key;
    s32 size;
    s32 i;
    s32 count;
    s32 sound;

    T2Port_RngAtLoad(level); /* DEBUG-ONLY: T2_RNG_LOAD=T:HEX (port/game/t2debug.c, round 17 f3) */
    if (w->m_nLevel != -1) {
        CScene__FlushDelayedEvents(w);
    }
    if (Cinema_IsActive() == 0 && Demo_GetPlayerCount() == 0 && announce != 0 && w->m_bWarpSpawn != 0) {
        switch (w->m_LevelID) {
            case 2000:
                sound = 0x8E;
                break;
            case 3000:
                sound = 0x8F;
                break;
            case 4000:
                sound = 0x90;
                break;
            case 5000:
                sound = 0x91;
                break;
            case 6000:
                sound = 0x92;
                break;
            default:
                goto loaded;
        }
        Cinema_RequestOnce(sound);
    }
loaded:
    CScene__ReleaseSectors(w);
    w->m_Mode = 0;
    CDecompressor__Flush(&gDecompressor);
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    if (w->m_nLevel != -1) {
        ptr = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (ptr != NULL) {
            CPlayer__StopLoopingSounds(ptr);
        }
        LoopSounds_StopAll(LoopSounds_Get());
        Sound__DetachAll();
        Sound__StopFlags(0x1000);
        CCache__Deallocate(0, w->m_hLevelIndex);
        CCache__Deallocate(0, w->m_hRegionData);
        CCache__Deallocate(0, w->m_hCollision);
        CCache__Deallocate(0, w->m_hLevelInfo);
        CCache__Deallocate(0, w->m_hLevelHeader);
        CCache__Deallocate(0, w->m_hActors);
        CCache__Deallocate(0, w->m_hLinks);
        CCache__Deallocate(0, w->m_hPaths);
        CCache__Deallocate(0, w->m_hSections);
        CCache__Deallocate(0, w->m_hHeap);
    }
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    gFrameCountGameplay = 0;
    CCache__SetFrameDelay(8);
    CScene__ClearListCounts(w);
    CParticleSystem__Construct(&w->m_ParticleSystem, *(sptr *) w->m_pParticleEffects);
    SceneActorPool_Construct(&w->m_ActorPool);
    CInstancePool__Construct(&w->m_InstancePool);
    SwooshSystem_Construct(&gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    /* the ROM finds COnScreen 0x1C8 bytes before the swoosh system (its N64 size); on the host
     * the two are not that far apart: the boss-bar reset ran on the middle of COnScreen (f4) */
    COnScreen__ResetBossBars(&gEngineApp_m_OnScreen);
    if (*w->m_pLevels < level) {
        level = *w->m_pLevels - 1;
        announce = 0;
    }
    w->m_nLevel = level;
    if (announce == 0) {
        w->m_bWarpSpawn = 0;
    }
    CScene__GetLevelName(w, level, w->unk_18E5C, 0x3F);
    switch (w->m_LevelID) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB1:
        case 0x1DB2:
            w->m_bStreamAll = 1;
            break;
        default:
            w->m_bStreamAll = 0;
            break;
    }
    switch (w->m_LevelID) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB2:
            size = 0xC80;
            break;
        case 0x1DB1:
            size = 0x4B0;
            break;
        default:
            size = 0x320;
            break;
    }
    if (gEngineApp_m_Settings.multiplayer != 0) {
        size += 0x200;
    }
    block = CCache__Allocate(0, size << 6, 3, "Matrices");
    w->m_hHeap = block;
    MatrixHeap_Construct(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths, "level paths");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        CGameObjectInstance__InitAIType((u8 *)w->m_pActors + i * (s32) sizeof(CGameObjectInstance));
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    actor = player;
    while (player != NULL) {
        CGameObjectInstance__InitAIType(actor);
        actor += T2_OFFSETOF(CPlayer, m_WeaponActor);
        CGameObjectInstance__InitAIType(actor);
        player = *(u8 **)(player + T2_OFFSETOF(CPlayer, m_pNext));
        actor = player;
    }
    CScene__ReapplyFiredEvents(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
        CGameObjectInstance__UpdateAnimation(p);
    }
    w->m_Mode = 4;
    CPlayerPool__LevelStart(&gEngineApp_m_Game.m_PlayerList);
    CCameraPool__ResetEmitterPools(gEngineApp_m_Game.m_CameraPool);
    CCameraPool__Advance(gEngineApp_m_Game.m_CameraPool);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        CScene__StreamAllSections(w);
    }
}
#else
VM_TEXT void CScene__LoadLevel(CScene *w, s32 level, s32 announce) {
    void **block;
    u8 *data;
    u8 *player;
    u8 *actor;
    u8 *p;
    u8 *ptr; /* scratch pointer: the leaving player's state, then the actor array */
    s32 key;
    s32 size;
    s32 i;
    s32 count;
    s32 sound;

    if (w->m_nLevel != -1) {
        ((void (*)(void)) CScene__FlushDelayedEvents)(); /* see the ABI note above */
    }
    if (Cinema_IsActive() == 0 && Demo_GetPlayerCount() == 0 && announce != 0 && w->m_bWarpSpawn != 0) {
        switch (w->m_LevelID) {
            case 2000:
                sound = 0x8E;
                break;
            case 3000:
                sound = 0x8F;
                break;
            case 4000:
                sound = 0x90;
                break;
            case 5000:
                sound = 0x91;
                break;
            case 6000:
                sound = 0x92;
                break;
            default:
                goto loaded;
        }
        Cinema_RequestOnce(sound);
    }
loaded:
    CScene__ReleaseSectors(w);
    w->m_Mode = 0;
    CDecompressor__Flush(&gDecompressor);
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    if (w->m_nLevel != -1) {
        ptr = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
        if (ptr != NULL) {
            CPlayer__StopLoopingSounds(ptr);
        }
        LoopSounds_StopAll(LoopSounds_Get());
        Sound__DetachAll();
        Sound__StopFlags(0x1000);
        CCache__Deallocate(0, w->m_hLevelIndex);
        CCache__Deallocate(0, w->m_hRegionData);
        CCache__Deallocate(0, w->m_hCollision);
        CCache__Deallocate(0, w->m_hLevelInfo);
        CCache__Deallocate(0, w->m_hLevelHeader);
        CCache__Deallocate(0, w->m_hActors);
        CCache__Deallocate(0, w->m_hLinks);
        CCache__Deallocate(0, w->m_hPaths);
        CCache__Deallocate(0, w->m_hSections);
        CCache__Deallocate(0, w->m_hHeap);
    }
    ((void (*)(s32)) CCache__Flush)(0); /* the definition takes none */
    gFrameCountGameplay = 0;
    CCache__SetFrameDelay(8);
    CScene__ClearListCounts(w);
    CParticleSystem__Construct(&w->m_ParticleSystem, *w->m_pParticleEffects);
    SceneActorPool_Construct(&w->m_ActorPool);
    CInstancePool__Construct(&w->m_InstancePool);
    SwooshSystem_Construct(&gEngineApp_m_SwooshSystem);
    CCollisionInfo__Reset();
    CDLightSet__Construct(&w->m_DLightSet);
    LoopSounds_Construct(&gLoopSounds, gLoopSoundEntries, 0xF);
    COnScreen__ResetBossBars((u8 *) &gEngineApp_m_SwooshSystem - 0x1C8);
    if (*w->m_pLevels < level) {
        level = *w->m_pLevels - 1;
        announce = 0;
    }
    w->m_nLevel = level;
    if (announce == 0) {
        w->m_bWarpSpawn = 0;
    }
    CScene__GetLevelName(w, level, w->unk_18E5C, 0x3F);
    switch (w->m_LevelID) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB1:
        case 0x1DB2:
            w->m_bStreamAll = 1;
            break;
        default:
            w->m_bStreamAll = 0;
            break;
    }
    switch (w->m_LevelID) {
        case 0x123A:
        case 0x1BBC:
        case 0x1C20:
        case 0x1C84:
        case 0x1CE8:
        case 0x1CE9:
        case 0x1D4C:
        case 0x1D4D:
        case 0x1DB2:
            size = 0xC80;
            break;
        case 0x1DB1:
            size = 0x4B0;
            break;
        default:
            size = 0x320;
            break;
    }
    if (gEngineApp_m_Settings.multiplayer != 0) {
        size += 0x200;
    }
    block = CCache__Allocate(0, size << 6, 3, "Matrices");
    w->m_hHeap = block;
    MatrixHeap_Construct(w->m_Heap, *block, size);
    key = CISet__GetROMAddress(w->m_pLevels, w->m_LevelsKey, level);
    w->m_hLevelIndex = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelFile, key, "level index");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 1, key, w, CScene__DecompressCollision, "collision");
    w->m_hRegionData = block;
    w->m_pRegionData = *block;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 4, key, w, ProPack__Decompress, "grid bounds");
    w->m_hCollision = block;
    w->m_pCollision = *block;
    w->m_LevelInfoKey = CISet__GetROMAddress(w->m_pLevelFile, key, 5);
    w->m_hLevelInfo = CCache__LoadPersistantIndex(0, (void **)&w->m_pLevelInfo, w->m_LevelInfoKey, "grid sections index");
    w->m_hLinks = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 7, key, w, CScene__DecompressLinks, "links");
    w->m_hPaths = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 2, key, w, CScene__DecompressPaths, "level paths");
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 0, key, w, NULL, "level info");
    w->m_hLevelHeader = block;
    w->m_pLevelHeader = *block;
    CScene__LoadLightSettings(w);
    gFogEnabled = (w->m_pLevelHeader->m_Flags >> 2) & 1;
    block = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 6, key, w, ProPack__Decompress, "sky layers");
    w->m_hSections = block;
    data = *block;
    w->m_pSections = data + 8;
    w->m_nSections = *(s32 *)(data + 4);
    w->m_Mode = 1;
    w->m_pActors = NULL;
    w->m_nActors = 0;
    w->m_hActors = CCache__LoadPersistantSubBlock_v(0, w->m_pLevelFile, 3, key, w, CScene__DecompressAnimatedInstances, "animated instances");
    w->m_Mode = 2;
    CScene__RestoreFiredEvents(w);
    count = w->m_nActors;
    i = w->m_nFirstActor;
    w->m_Mode = 3;
    for (; i < count; i++) {
        CGameObjectInstance__InitAIType((u8 *)w->m_pActors + i * (s32) sizeof(CGameObjectInstance));
    }
    player = gEngineApp_m_Game.m_PlayerList.m_ActiveList.m_pHead;
    actor = player;
    while (player != NULL) {
        CGameObjectInstance__InitAIType(actor);
        actor += T2_OFFSETOF(CPlayer, m_WeaponActor);
        CGameObjectInstance__InitAIType(actor);
        player = *(u8 **)(player + T2_OFFSETOF(CPlayer, m_pNext));
        actor = player;
    }
    CScene__ReapplyFiredEvents(w);
    count = w->m_nActors;
    for (i = w->m_nFirstActor; i < count; i++) {
        ptr = (u8 *)w->m_pActors;
        p = ptr + i * (s32) sizeof(CGameObjectInstance);
        CGameObjectInstance__UpdateAI(p);
        CGameObjectInstance__UpdateAnimation(p);
    }
    w->m_Mode = 4;
    CPlayerPool__LevelStart(&gEngineApp_m_Game.m_PlayerList);
    CCameraPool__ResetEmitterPools(gEngineApp_m_Game.m_CameraPool);
    CCameraPool__Advance(gEngineApp_m_Game.m_CameraPool);
    if (gEngineApp_m_Game.m_Settings.multiplayer != 0 || w->m_bStreamAll != 0) {
        CScene__StreamAllSections(w);
    }
}
#endif

s32 BinarySearch(void *array, u32 count, u32 key);

DEFERRED sptr CScene__LoadBinaryBlock(Unk00224D54 *arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 sp20;
    sptr temp_v0;
    Table00224D54 *temp_a0;

    temp_a0 = arg0->table;
    temp_v0 = BinarySearch((sptr) temp_a0->entries, temp_a0->count, arg1);
    if (temp_v0 != -1) {
        return CCache__Request(0, CISet__GetROMAddressAndSize(arg0->unk44, arg0->unk18, temp_v0, &sp20), sp20, arg2, 0, &ProPack__Decompress, (sptr) "binary block", arg3);
    }
    return 0;
}

#define unk1C m_GraphicSetsKey
#define unk48 m_pGraphicSets
#define table_of(o) ((StrideTable *)(o)->m_pGraphicSetTypes)

DEFERRED sptr CScene__LoadGraphicSet(Owner *owner, s32 id, s32 flags, s32 arg3) {
    s32 index;
    s32 out;

    index = BinarySearch(table_of(owner)->entries, table_of(owner)->count, id);
    if (index == -1) {
        return 0;
    }
    switch (id) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
        flags |= 0x18;
        break;
    default:
        flags |= 4;
        break;
    }
    return CCache__Request(0, CISet__GetROMAddressAndSize(owner->unk48, owner->unk1C, index, &out), out, flags, 0,
                         ProPack__Decompress, "graphicset", arg3);
}

#undef unk1C

struct IntPair_0042B3E4;
struct IntPair_0042B3E4 {
    int field_0;
    int field_4;
};
struct Bucket;
typedef struct Bucket Bucket;
struct Bucket {
    struct Bucket *prev;
    struct Bucket *next;
    struct Record14 *object;
    f32 remaining;
};
struct Face;
struct Face {
    char pad0[0xE];
    u8 flags;
    u8 hidden;
    char pad10[3];
    u8 group;
};
typedef struct Face Face;
#ifdef TARGET_PORT /* the same fields at the host offsets of CScene (structs/world.h) */
#include "structs/world.h"
struct WorldRecAt1B40C {
    char pad0[__builtin_offsetof(CScene, m_pBitSections)];
    void *tree;
    char pad70[__builtin_offsetof(CScene, m_nRecords) - __builtin_offsetof(CScene, m_pBitSections) - sizeof(void *)];
    s32 nfaces;
    s32 nedges;
    void *verts;
    void *edgeData;
    struct Face *faces;
    struct Face *edges;
    CList listA;
    CList listB;
    CDelayedEvent buckets[0x30];
    char pad14A8[__builtin_offsetof(CScene, m_nLevel) - __builtin_offsetof(CScene, m_nSectors)];
    s32 group;
};
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, listA) == __builtin_offsetof(CScene, m_DelayedEvents), "listA");
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, faces) == __builtin_offsetof(CScene, m_pRecords), "faces");
_Static_assert(__builtin_offsetof(struct WorldRecAt1B40C, group) == __builtin_offsetof(CScene, m_nLevel), "group");
#define BUCKET_LINKS T2_OFFSETOF(CDelayedEvent, m_pLast), T2_OFFSETOF(CDelayedEvent, m_pNext)
#else
#define BUCKET_LINKS 0, 4
struct WorldRecAt1B40C;
struct WorldRecAt1B40C {
    char pad0[0x6C];
    void *tree;              /* 0x6C (0x80 in Rage Wars) */
    char pad70[0x1168 - 0x70];
    s32 nfaces;              /* 0x1168 (0x11C0): Turok 2 tables sit 0x58 earlier */
    s32 nedges;
    void *verts;
    void *edgeData;
    struct Face *faces;
    struct Face *edges;
    CList listA;     /* 0x1180 */
    CList listB;     /* 0x1194 */
    Bucket buckets[0x30];    /* 0x11A8 */
    char pad14A8[0x18FA8 - 0x14A8];
    s32 group;               /* 0x18FA8 (0x1B40C) */
};
#endif

struct WorldRecAt1B40C;
typedef struct WorldRecAt1B40C WorldRecAt1B40C;
s32 ProPackDecompress(void *handle, s32 arg1);

VM_TEXT static __inline__ s32 groupHas(WorldRecAt1B40C *world, s32 n) {
    s32 index;
    void *g;
    u8 *bits;
    s32 mask;
    index = world->group;
    g = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->tree, 0), index), 2);
    CISet__GetBlock(g, 0);
    CISet__GetBlockSize(g, 1);
    bits = CISet__GetBlock(g, 1);
    mask = 1 << (n & 7);
    return bits[n / 8] & mask;
}

#define world ((WorldRecAt1B40C *) worldArg)

/* the definition takes the callers' type (one prototype for the merged object) (its view's type differs per build); the Rage Wars view reads it */
VM_TEXT void CScene__DecompressLinks(void *worldArg, void ***handle) {
    void *node;
    struct IntPair_0042B3E4 *l0;
    struct IntPair_0042B3E4 *l1;
    struct IntPair_0042B3E4 *l2;
    struct IntPair_0042B3E4 *l3;
    s32 k;
    s32 i;
    s32 n;
    s32 count;
    Face *face;
    if (ProPackDecompress(handle, 0) != 0) {
        node = **handle;
        l0 = CISet__GetBlock(node, 0);
        l1 = CISet__GetBlock(node, 1);
        l2 = CISet__GetBlock(node, 2);
        l3 = CISet__GetBlock(node, 3);
        count = l0->field_4;
        world->verts = &((ResidentDataChunk *)(l0))->data[0];
        world->faces = (Face *)&((ResidentDataChunk *)(l1))->data[0];
        world->nfaces = count;
        count = l2->field_4;
        world->edgeData = &((ResidentDataChunk *)(l2))->data[0];
        world->edges = (Face *)&((ResidentDataChunk *)(l3))->data[0];
        world->nedges = count;
        CList__Construct(&world->listA, BUCKET_LINKS);
        CList__Construct(&world->listB, BUCKET_LINKS);
        for (k = 0; k < 0x30; k++) {
            CList__AddTail(&world->listB, &world->buckets[k]);
        }
        n = world->nfaces;
        for (i = 0; i < n; i++) {
            face = &world->faces[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
        n = world->nedges;
        for (i = 0; i < n; i++) {
            face = &world->edges[i];
            if (face->hidden != 0) {
                face->flags &= 0xFE;
            }
            if (groupHas(world, face->group)) {
                face->flags |= 0xF0;
            }
        }
    }
}

#undef world
#define WORLD_TREE(w) (((CScene *)(w))->m_pBitSections2)
#define WORLD_GROUP(w) (((CScene *)(w))->m_nLevel)

extern void UnpackScaledVec4(CollisionBuildVertex *v);
extern void CGameRegion__Unpack(CollisionBuildTriangle *tri, CollisionBuildFace *face, CollisionBuildVertex *verts,
                          s32 nverts, CollisionBuildTriangle *tris, s32 ntris);
extern s32 Cinema_PathPlaying(void);

VM_TEXT static __inline__ s32 testBit(s32 n, u8 *bits) {
    s32 mask = 1 << (n & 7);

    return bits[n / 8] & mask;
}

/* Packs a collision mesh into one allocated block: copies the node's vertex list, the triangle source
   and the trailing chunk behind a five-offset header, builds a 0x20-byte triangle record per source
   face and flags triangles whose group bit, the cinema state or neighbours mark them with the default
   surface mask. */
VM_TEXT void CScene__DecompressCollision(CollisionBuildWorld *world, CollisionBuildMeshHandle **handle) {
    s32 size0;
    s32 size1;
    s32 size3;
    void *chunk;
    s32 tableSize;
    char *base;
    CollisionBuildVertex *verts;
    CollisionBuildTriangle *tris;
    CollisionBuildMeshHandle *block;
    void *node;
    void *first;
    CollisionBuildVertexList *vlist;
    CollisionBuildList *flist;
    s32 ntris;
    CollisionBuildHeader *hdr;
    s32 offset;
    char *dst;
    char *vdst;
    CollisionBuildVertexList *copiedVertices;
    CollisionBuildTriangleList *copiedTriangles;
    s32 nverts;
    s32 i;
    s32 j;
    s32 k;
    CollisionBuildTriangle *tri;
    CollisionBuildFace *face;
    void *group;
    s32 index;
    u8 *bits;

    if (ProPackDecompress(handle, 1) != 0) {
        node = (*handle)->data;
        first = CISet__GetBlockAndSize(node, 0, &size0);
        vlist = CISet__GetBlockAndSize(node, 1, &size1);
        flist = CISet__GetBlock(node, 2);
        ntris = flist->count;
        chunk = CISet__GetBlockAndSize(node, 3, &size3);
#ifdef TARGET_PORT /* host triangles (CGameRegion) are larger than 0x20 */
        tableSize = ntris * sizeof(CollisionBuildTriangle) + 8;
#else
        tableSize = (ntris << 5) | 8;
#endif
        block = CCache__AllocateMemoryForDecompression(0, handle, size0 + 0x18 + size1 + tableSize + size3);
        if (block != 0) {
            offset = 0x18;
            hdr = block->data;
            i = 0;
            hdr->count = 0;
            hdr->offsets[0] = offset;
            CUSet__Transpose(first, (char *)hdr + offset, 0);
            hdr->count = 1;
            offset += size0;
            vdst = (char *)hdr + offset;
            hdr->offsets[1] = offset;
            CUSet__Transpose(vlist, vdst, 0);
            hdr->count = 2;
            offset += size1;
            dst = (char *)hdr + offset;
            hdr->offsets[2] = offset;
            CUSet__Transpose(flist, dst, 1);
            face = flist->items;
            copiedTriangles = (void *)dst;
            copiedTriangles->kind = 0x20;
            copiedTriangles->count = ntris;
            base = (char *)hdr;
            tris = copiedTriangles->items;
            nverts = vlist->count;
            copiedVertices = (void *)vdst;
            verts = copiedVertices->items;
            for (i = 0; i < nverts; i++) {
                UnpackScaledVec4(&verts[i]);
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                CGameRegion__Unpack(tri, &face[j], verts, nverts, tris, ntris);
                index = WORLD_GROUP(world);
                group = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(WORLD_TREE(world), 0), index), 0);
                CISet__GetBlock(group, 0);
                CISet__GetBlockSize(group, 1);
                bits = CISet__GetBlock(group, 1);
                if (testBit(j, bits)) {
                    tri->flags |= 0x400;
                }
                if (!(tri->mask & gDefaultVisBits)) {
                    tri->mask = gDefaultVisBits;
                }
                if (Cinema_PathPlaying() != 0) {
                    tri->mask = gDefaultVisBits;
                }
                if (tri->flags & 0x2000) {
                    tri->mask = gDefaultVisBits;
                }
            }
            for (j = 0; j < ntris; j++) {
                tri = &tris[j];
                for (k = 0; k < 3; k++) {
                    if (tri->adj[k] != 0 && (tri->adj[k]->flags & 0x2000)) {
                        { s32 *surface = &tri->mask; *surface = gDefaultVisBits; }
                    }
                }
            }
            offset += tableSize;
            hdr->count = 3;
            hdr->offsets[3] = offset;
            CUSet__Transpose(chunk, base + offset, 0);
            hdr->count = 4;
            offset += size3;
            hdr->offsets[4] = offset;
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
    }
}

typedef struct Handle {
    void *data;
} Handle;
extern sptr CScene__GetRegion(CScene *world, s32);
/* called with the record's u16 id and its result read as a pointer, as the ROM's code has it */
#define CScene__GetIntelligence_u16 ((s32 *(*)(CScene *, u16)) CScene__GetIntelligence)

extern void CGameObjectInstance__ConstructFromRecord(CGameObjectInstance *obj, CObjectRecord *rec, s32 mode, s32 index);
void Angle_Wrap(f32 *value);
extern void CPlayer__SetupActors(CPlayer *player, CObjectRecord *rec, s32 mode, s32 index);
extern s32 GameObject_IsFollower(CGameObjectInstance *obj);
extern s32 GameObject_IsLeader(CGameObjectInstance *obj);
extern f32 AI_DistSqToPointAtHeight(CGameObjectInstance *obj, CVector3 pos);
/* Builds the level's actor block from its records: places every player on the spawn points found for
   the level (one record per player start), then links each remaining actor to the nearest actor with
   the same definition. The spawn point is indexed afresh on each use and the position copied with
   memcpy (as in Rage Wars): both set loop.c's insn count and giv benefits, which decide the ROM's
   hoisting of the spawns base and of the angle constants. */
#define handle ((Handle **) handleArg)
typedef struct Placed {
    /* 0x00 */ s32 data[4];
    /* 0x10 */ s16 kind;
    /* 0x12 */ s16 unk12;
} Placed; /* 0x14 */
s32 CScene__FindWarpDests(void *world, s32 kind, s32 key, Placed *out, s32 max);
typedef CGameRegion Floor;
Floor *CScene__FindRegionAt(CScene *world, f32 *pos);

/* the definition takes the callers' type (one prototype for the merged object); its handle view reads it */
VM_TEXT void CScene__DecompressAnimatedInstances(CScene *world, void *handleArg) {
    CWarpPoint spawns[4];
    CGameObjectInstance *objects;
    sptr mode;
    Handle *block;
    s32 nspawns;
    CObjectRecordBlock *node;
    CObjectRecord *records;
    CInstanceBlock *hdr;
    s32 n;
    s32 i;
    s32 j;
    s32 playerIndex;
    s32 spawnIndex;
    CObjectRecord *rec;
    CObjectRecord *recWalk;
    CGameObjectInstance *objWalk;
    CPlayer *player;
    CGameObjectInstance *obj;
    CGameObjectInstance *other;
    f32 angle;
    f32 best;
    f32 dist;
    s16 value;

    if (ProPackDecompress(handle, 1) != 0) {
        mode = CScene__GetRegion(world, 0);
        node = (*handle)->data;
        n = node->count;
        block = CCache__AllocateMemoryForDecompression(0, handle, n * sizeof(CGameObjectInstance) + 8);
        if (block != NULL) {
            CUSet__Transpose(node, block->data, 1);
            hdr = block->data;
            hdr->size = sizeof(CGameObjectInstance);
            hdr->count = n;
            records = node->records;
            objects = hdr->objects;
            if (world->m_bWarpSpawn == 0) {
                world->m_LevelID = CScene__FindNearestWarpDest(world, &node->records[0].m_vPos, world->m_nLevel, &world->m_WarpSpawn);
                if (world->m_LevelID != 0) {
                    world->m_bWarpSpawn = 1;
                }
            }
            spawnIndex = 0;
            nspawns = 0;
            if (world->m_bWarpSpawn != 0) {
                nspawns = CScene__FindWarpDests(world, world->m_nLevel, world->m_LevelID, spawns, 4);
            }
            world->m_pActors = (CGameObjectInstance *)objects;
            world->m_nActors = n;
            i = 0;
            if (i < n) {
                objWalk = objects;
                recWalk = records;
                while (i < n) {
                    rec = recWalk;
                    obj = objWalk;
                    i++;
                    obj->header.m_pIntelligence = CScene__GetIntelligence_u16(world, rec->m_nIntelligence);
                    objWalk = obj + 1;
                    recWalk = rec + 1;
                }
            }
            world->m_nFirstActor = 0;
            for (i = 0; i < n; i++) {
                rec = &records[i];
                obj = &objects[i];
                CGameObjectInstance__ConstructFromRecord(obj, rec, mode, i);
                if (*CScene__GetIntelligence_u16(world, rec->m_nIntelligence) != 0xB && i != 0) {
                    continue;
                }
                world->m_nFirstActor++;
                memcpy(&gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos, &rec->m_vPos, sizeof(CVector3));
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_vPos.y += 204.79999f;
                gEngineApp_m_Game_m_CameraPool.m_MainCamera.m_ViewParams.m_pCurrentRegion = CScene__FindRegionAt(world, &rec->m_vPos);
                for (playerIndex = 0; playerIndex < gEngineApp_m_Game.m_PlayerList.m_nPlayers; playerIndex++) {
                    player = &gEngineApp_m_Game.m_PlayerList.m_Players[playerIndex];
                    if (nspawns != 0) {
                        rec->m_nRegion = spawns[spawnIndex].m_nID;
                        Angle_Wrap(&spawns[spawnIndex].m_RotY);
                        angle = spawns[spawnIndex].m_RotY;
                        if (!(angle > 3.1415927f)) {
                            if (!(angle < -3.1415927f)) {
                                if (!(angle > 3.1415927f)) {
                                    value = angle * 10430.06f;
                                } else {
                                    value = 0x7FFF;
                                }
                            } else {
                                value = -0x7FFF;
                            }
                        } else {
                            value = 0x7FFF;
                        }
                        rec->m_Angle = value;
                        memcpy(&rec->m_vPos, &spawns[spawnIndex].m_vPos, sizeof(CVector3));
                        spawnIndex++;
                        if (spawnIndex >= nspawns) {
                            spawnIndex = 0;
                        }
                    }
                    CPlayer__SetupActors(player, rec, mode, i);
                }
            }
            for (i = world->m_nFirstActor; i < n; i++) {
                obj = &objects[i];
                best = 3.4028235e38f;
                obj->m_AI.m_pParent = NULL;
                if (GameObject_IsFollower(obj) == 0) {
                    continue;
                }
                for (j = world->m_nFirstActor; j < n; j++) {
                    other = &objects[j];
                    if (obj->header.m_pIntelligence->m_Type == other->header.m_pIntelligence->m_Type && GameObject_IsLeader(other) != 0 && obj != other) {
                        if (obj->m_AI.m_pParent != NULL) {
                            dist = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                            if (dist < best) {
                                obj->m_AI.m_pParent = other;
                                best = dist;
                            }
                        } else {
                            obj->m_AI.m_pParent = other;
                            best = AI_DistSqToPointAtHeight(obj, other->header.m_vPos);
                        }
                    }
                }
            }
        }
        CCache__DeallocAndReplaceCompressedData(0, handle, block);
        world->m_Kind1Cursor = 0;
    }
}

#undef handle

typedef struct FloorCell {
    f32 minX, minZ;             /* 0x00 */
    f32 maxX, maxZ;             /* 0x08 */
    u32 count;                  /* 0x10: floors in this cell, in order */
} FloorCell;                    /* 0x14 bytes */
typedef struct FloorGrid {
    s32 pad0;
    s32 count;                  /* 0x04 */
    FloorCell cells[1];         /* 0x08 */
} FloorGrid;
s32 CGameRegion__ContainsXZ(Floor *floor, f32 x, f32 z);      /* point over floor */
f32 CGameRegion__GetHeight(Floor *floor, f32 x, f32 z);      /* floor height at */

/* The floor under pos whose height is closest to it, or NULL. */
Floor *CScene__FindRegionAt(CScene *world, f32 *pos) {
    Floor *floors;
    Floor *floor;
    Floor *best;
    FloorGrid *grid;
    FloorCell *cell;
    FloorCell *cells;
    s32 cellCount;
    s32 k;
    s32 i;
    u32 j;
    f32 d;
    f32 bestDist;

    if (gPersistentDataLoaded == 0) {
        return NULL;
    }
    floors = (Floor *)((u8 *)CISet__GetBlock(world->m_pRegionData, 2) + 8);
    k = 0;
    i = 0;
    bestDist = 0.0f;
    best = NULL;
    grid = CISet__GetBlock(world->m_pRegionData, 3);
    cellCount = grid->count;
    cells = grid->cells;
    for (; i < cellCount; i++) {
        cell = &cells[i];
        if (pos[0] >= cell->minX && pos[0] <= cell->maxX && pos[2] >= cell->minZ && pos[2] <= cell->maxZ) {
            j = 0;
            if (cell->count != 0) {
                floor = (Floor *)(k * sizeof(Floor) + (uptr)floors);
                do {
                    if (CGameRegion__ContainsXZ(floor, pos[0], pos[2])) {
                        d = pos[1] - CGameRegion__GetHeight(floor, pos[0], pos[2]);
                        if (d < 0.0f) {
                            d = -d;
                        }
                        if (best == NULL || d < bestDist) {
                            best = floor;
                            bestDist = d;
                        }
                    }
                    floor++;
                    k++;
                    j++;
                } while (j < cell->count);
            }
        } else {
            k += cell->count;
        }
    }
    return best;
}

struct World_func_8028787C_de;
typedef struct World_func_8028787C_de World_func_8028787C_de;
struct World_func_8028787C_de {
    char pad0[0x6C];
    void *levels;                    /* 0x6C in Turok 2 (0x80 in Rage Wars) */
    char pad70[0x18FA8 - 0x70];
    s32 level;                       /* 0x18FA8 (0x1B40C in Rage Wars) */
};
struct Entity;
typedef struct Entity Entity;
struct Entity {
    char pad0[0xF];
    unsigned char flag;
    char pad10[3];
    unsigned char index;
};
void LevelEvent_Fire(void *event);
#ifdef TARGET_PORT
/* The port (round 13 b2): the world by member. The N64 view reads the bit sections at 0x6C and the
 * level at 0x18FA8, other members of the host CScene: no fired event was ever restored when a level
 * loaded, so what they had done (a used key slot s collision off, ...) was lost on a later visit. */
#include "structs/world.h"
#include "globals/scene.h"
void CScene__RestoreFiredEvent(void *world_, void *entityArg) {
    CScene *world = (CScene *) world_;
    Entity *entity = (Entity *) entityArg;
    s32 index = entity->index;
    void *mask;
    unsigned char *bits;
    void *flags;

    mask = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->m_pBitSections, 0), world->m_nLevel), 2);
    bits = (unsigned char *) CISet__GetBlock(mask, 1);
    if (bits[index / 8] & (1 << (index & 7))) {
        if (entity->flag != 0) {
            flags = CISet__GetBlock(gScene.m_pBitSections, 1);
            if (((unsigned char *) CISet__GetBlock(flags, 1))[entity->flag] != 0) {
                return;
            }
        }
        LevelEvent_Fire(entity);
    }
}
#else
#define world ((World_func_8028787C_de *) worldArg)
#define entity ((Entity *) entityArg)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
void CScene__RestoreFiredEvent(void *worldArg, void *entityArg) {
    s32 index;
    s32 level;
    void *mask;
    unsigned char *bits;
    void *flags;
    s32 flag;
    s32 bit;
    index = entity->index;
    level = world->level;
    mask = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->levels, 0), level), 2);
    CISet__GetBlock(mask, 0);
    CISet__GetBlockSize(mask, 1);
    bits = CISet__GetBlock(mask, 1);
    bit = 1 << (index & 7);
    if (bits[index / 8] & bit) {
        if (entity->flag != 0) {
            flag = entity->flag;
            flags = CISet__GetBlock(gScene.m_pBitSections, 1);
            CISet__GetBlock(flags, 0);
            if (((unsigned char *)CISet__GetBlock(flags, 1))[flag] != 0) {
                return;
            }
        }
        LevelEvent_Fire(entity);
    }
}
#undef world
#undef entity
#endif


void CScene__DecompressGridSection(CScene *world, s32 ***file);
/* Instantiates a level's actor data: from the level file's four tables
 * (two kinds of placed objects, the level actors, and a fourth kind of
 * 0x48-byte entries), builds one block with a directory of section offsets
 * and a {stride, count} header per section, fills each record from its
 * source entry, marks the actors already killed in this level (the level's
 * bit array, flag 1 at +0x16C), then links every actor to the 0xC0 records.
 *
 * The empty do { } while (0) blocks are what the ROM needs: GCC weights a
 * pseudo's references by loop depth, so they decide which of the loop
 * counters and record pointers get $s0-$s2 (m before qpd, as in the ROM). */

typedef struct {
    s32 count;              /* 0x00: sections so far */
    s32 offsets[5];         /* 0x04 */
} Block;
typedef struct { u8 pad0[8]; u16 type; u8 padA[0x12]; } SrcA;          /* 0x1C */
typedef struct { u8 pad0[0x28]; u16 type; u8 pad2A[0xA]; } SrcB;       /* 0x34 */
typedef struct { u8 pad0[0x14]; u16 type; u8 pad16[0xA]; } SrcC;       /* 0x20 */
typedef struct { u8 pad0[0x48]; } SrcD;                                /* 0x48 */
/* The port: the built records at their host sizes (CStaticObject and CLevelActor hold pointers;
 * the 0xC0 light volumes do not). The readers index them as typed arrays. */
#define GRID_STATIC_SIZE ((s32) sizeof(CStaticObject))
#define GRID_ACTOR_SIZE ((s32) sizeof(CLevelActor))

typedef struct { u8 pad0[GRID_STATIC_SIZE]; } ItemA;                    /* 0x90 */
typedef struct { u8 pad0[0xC0]; } ItemD;                               /* 0xC0 */
void CStaticObject__ConstructCompact(ItemA *, SrcA *, s32, void *, f32);
void CStaticObject__Construct(ItemA *, SrcB *, s32, void *);
void CLevelActor__Construct(CLevelActor *, SrcC *, s32, void *);
void LightVolume_Construct(ItemD *, SrcD *);
f32 GameObject_GetCollisionHeight(void *actor);
typedef struct B19bV3f { f32 x, y, z; } B19bV3f;
typedef struct B19bLObj {
              u8 pad0[4];
              f32 f4;
              B19bV3f pos;
               u8 pad14[0xA4];
               u8 fB8;
               u8 padB9;
               u8 fBA;
} B19bLObj;
void CScene__ComputeLights(B19bLObj **ents, s32 n, f32 *pos, u8 *out);

void CScene__DecompressGridSection(CScene *world, s32 ***file) {
    StrideTable *tE;
    void *links[128];
    void **pp;
    f32 pos[3];
    StrideTable *tA;
    StrideTable *tB;
    StrideTable *tC;
    StrideTable *tD;
    u8 *base;
    StrideTable *tAux;
    StrideTable *bits;
    s32 nA;
    s32 nB;
    s32 nC;
    s32 nD;
    s32 n;
    s32 szA;
    s32 szC;
    s32 szD;
    s32 tot;
    s32 tt;
    register s32 szx asm("$2");
    u8 *nv;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    ItemD *lp;              /* walks the 0xC0 records for links[] */
    s32 mask;
    u8 *killed;
    s32 level;
    s32 off;
    s32 sn;
    void *root;
    Block *block;
    u8 *sec;
    u8 *secC;
    u8 *secD;
    ItemA *itemsA;
    ItemD *itemsD;
    SrcA *pA;
    SrcC *pC;
    SrcD *pD;
    SrcB *qb;
    ItemD *qid;
    SrcD *qpd;
    s32 index;
    SrcB *pB;
    CLevelActor *actors;
    void *aux;
    void **handle;

    if (ProPackDecompress(file, 1) != 0) {
        root = **file;
        tA = CISet__GetBlock(root, 0);
        nA = tA->count;
        tB = CISet__GetBlock(root, 1);
        nB = tB->count;
        n = nA + nB;
        szA = (n * GRID_STATIC_SIZE) | 8;
        tC = CISet__GetBlock(root, 2);
        nC = tC->count;
        szx = nC * GRID_ACTOR_SIZE;
        szC = szx + 8;
        tD = CISet__GetBlock(root, 3);
        nD = tD->count;
        szD = (nD * 0xC0) | 8;
        tot = szC + 0x18;
        tt = szA + tot;
        handle = CCache__AllocateMemoryForDecompression(0, file, tt + szD);
        if (handle != NULL) {
            block = *handle;
            off = 0x18;
            sn = 1;
            sec = (u8 *)block + off;
            block->count = 0;
            block->offsets[0] = off;
            CUSet__Transpose(tA, sec, 1);
            CUSet__Transpose(tB, sec, 1);
            nv = tA->entries;
            ((StrideTable *)sec)->stride = GRID_STATIC_SIZE;
            ((StrideTable *)sec)->count = n;
            tAux = CISet__GetBlock(world->m_pRegionData, 2);
            pB = (SrcB *)tB->entries;
            itemsA = (ItemA *)((u8 *)block + 0x20);
            base = (u8 *)block;
            aux = tAux->entries;
            pA = (SrcA *)nv;
            i = 0;
            if (nA > 0) {
                do {
                    CStaticObject__ConstructCompact(&itemsA[i], &pA[i], CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, pA[i].type), aux, world->m_pLevelHeader->m_Scale);
                    i++;
                } while (i < nA);
            }
            if (nB > 0) {
                qb = pB;
                j = 0;
                do {
                    CStaticObject__Construct(&itemsA[j + nA], qb, CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, qb->type), aux);
                    qb++;
                    j++;
                } while (j < nB);
            }
            off += szA;
            block->count = sn;
            sn++;
            secC = base + off;
            block->offsets[block->count] = off;
            CUSet__Transpose(tC, secC, 1);
            ((StrideTable *)secC)->stride = GRID_ACTOR_SIZE;
            ((StrideTable *)secC)->count = nC;
            actors = (CLevelActor *)(secC + 8);
            pC = (SrcC *)tC->entries;
            k = 0;
            if (nC > 0) {
                do {
                    CLevelActor__Construct(&actors[k], &pC[k], CISet__GetROMAddress(world->m_pObjects, world->m_ObjectsKey, pC[k].type), aux);
                    index = actors[k].m_nKilledIndex;
                    level = world->m_nLevel;
                    bits = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->m_pBitSections, 0), level), 1);
                    do {
                    } while (0);
                    CISet__GetBlock(bits, 0);
                    CISet__GetBlockSize(bits, 1);
                    killed = CISet__GetBlock(bits, 1);
                    mask = 1 << (index & 7);
                    if (killed[index / 8] & mask) {
                        actors[k].m_wFlags |= 1;
                    }
                    k++;
                } while (k < nC);
            }
            off += szC;
            block->count = sn;
            sn++;
            secD = base + off;
            block->offsets[block->count] = off;
            CUSet__Transpose(tD, secD, 1);
            ((StrideTable *)secD)->stride = 0xC0;
            m = 0;
            ((StrideTable *)secD)->count = nD;
            pD = (SrcD *)tD->entries;
            itemsD = (ItemD *)(secD + 8);
            if (nD > 0) {
                qpd = pD;
                qid = itemsD;
                do {
                    LightVolume_Construct(qid, qpd);
                    qpd++;
                    qid++;
                    m++;
                } while (m < nD);
            }
            off += szD;
            block->count = sn;
            sn++;
            block->offsets[block->count] = off;
            m = 0;
            do {
                if (nD > 0) {
                    pp = links;
                    lp = itemsD;
                    do {
                        *pp++ = lp;
                        lp++;
                        m++;
                    } while (m < nD);
                }
            } while (0);
            do {
                do {
                    k = 0;
                } while (0);
            } while (0);
            if (nC > 0) {
                do {
                    pos[0] = actors[k].header.m_vPos.x;
                    pos[1] = actors[k].header.m_vPos.y + GameObject_GetCollisionHeight(&actors[k]) * 0.5f;
                    pos[2] = actors[k].header.m_vPos.z;
                    CScene__ComputeLights(links, nD, pos, &actors[k].m_Lights);
                    k++;
                } while (k < nC);
            }
        }
        CCache__DeallocAndReplaceCompressedData(0, file, handle);
    }
}

#ifdef TARGET_PORT
/* The port: the grid sections hold their static objects and level actors at the host sizes
 * (CScene__DecompressGridSection), so these views are host twins of CStaticObject (block =
 * m_pOwner) and CLevelActor. With the N64 stride and offset the third attract demo's level load
 * (level 109) wrote the handle into the wrong objects and passed a garbage m_pOwner to
 * CCache__RequestDecompression. */
#include "structs/level_actor.h"
typedef struct ObjA {
    u8 pad0[__builtin_offsetof(CStaticObject, m_pOwner)];
    void **block;
    u8 pad28[sizeof(CStaticObject) - __builtin_offsetof(CStaticObject, m_pOwner) - sizeof(void **)];
} ObjA;
_Static_assert(sizeof(ObjA) == sizeof(CStaticObject), "ObjA size");

typedef struct ObjB {
    u8 pad0[sizeof(CLevelActor)];
} ObjB;
#else
typedef struct ObjA {
    /* 0x00 */ u8 pad0[0x24];
    /* 0x24 */ void **block;
    /* 0x28 */ u8 pad28[0x68];
} ObjA; /* 0x90 */

typedef struct ObjB {
    /* 0x000 */ u8 pad0[0x198];
} ObjB; /* 0x198 */
#endif

void CStaticObject__RequestLitVertices(void *object, void *camera);
extern void *CStaticObject__LoadGeometry(ObjA *a);
extern void *CLevelActor__LoadGeometry(ObjB *b);
extern void Model_RequestTextures(void *handle);

/* Streams in every grid cell of the level that is not loaded yet and spawns its objects. The loops run over
 * a rectangle of cells (firstRow..lastRow, firstCol..lastCol), here the whole grid: the column loop starting
 * from the variable firstCol, not the constant 0, is what keeps its entry test as the ROM's slt (Rage Wars'
 * decomp matches the same function with a lone `first` local it flags as a fake match). */
VM_TEXT void CScene__StreamAllSections(CScene *level) {
    s32 size;
    s32 width;
    s32 row;
    s32 lastCol;
    s32 lastRow;
    s32 rows;
    StreamCell *cells;
    StreamCell *cell;
    s32 col;
    s32 firstCol;
    s32 firstRow;
    sptr key;
    void **block;
    void *node;
    Counted *list;
    s32 i;
    s32 j;
    s32 n;
    s32 m;
    ObjA *a;
    ObjA *objsA;
    ObjB *objsB;
    ObjB *b;
    void *handle;

    width = ((Counted *)CISet__GetBlock(level->m_pCollision, 0))->count;
    rows = ((Counted *)CISet__GetBlock(level->m_pCollision, 2))->count;
    cells = (StreamCell *)((Counted *)CISet__GetBlock(level->m_pCollision, 4) + 1);
    firstRow = 0;
    lastRow = rows - 1;
    firstCol = 0;
    lastCol = width - 1;
    for (row = firstRow; row <= lastRow; row++) {
        for (col = firstCol; col <= lastCol; col++) {
            cell = &cells[col + row * width];
            if (cell->occupied != 0) {
                continue;
            }
            key = CISet__GetROMAddressAndSize(level->m_pLevelInfo, level->m_LevelInfoKey, col + row * width, &size);
            block = CCache__Request(0, key, size, 4, level, CScene__DecompressGridSection, "grid section", 1);
            if (block == 0) {
                continue;
            }
            node = *block;
            if (*(s32 *)node != 0) {
                list = CISet__GetBlock(node, 0);
                n = list->count;
                objsA = (ObjA *)(list + 1);
                for (i = 0; i < n; i++) {
                    a = &objsA[i];
                    a->block = block;
                    CStaticObject__RequestLitVertices(a, 0);
                    handle = CStaticObject__LoadGeometry(a);
                    if (handle != 0) {
                        Model_RequestTextures(handle);
                        CCache__Unlock(0, handle);
                    }
                }
                list = CISet__GetBlock(node, 1);
                m = list->count;
                objsB = (ObjB *)(list + 1);
                for (j = 0; j < m; j++) {
                    b = &objsB[j];
                    handle = CLevelActor__LoadGeometry(b);
                    if (handle != 0) {
                        Model_RequestTextures(handle);
                        CCache__Unlock(0, handle);
                    }
                }
            }
            CCache__Unlock(0, block);
        }
    }
}

#ifdef TARGET_PORT
#include "structs/game.h"
#include "globals/camera.h"
#endif

/* Builds the frame's actor lists: every live level actor (flag 0x100) whose
 * type is shown in the current view is tested against the view's bounding
 * box (and frustum), or against the wider active box when it is not drawn;
 * the visible ones are animated (CGameObjectInstance__ClearDrawMatrices) and put in the world's lists
 * (drawn, ..., all, enemies, flagged 2, type 5, and actor types 0x64F and
 * 0x64D); the others lose their "visible" flag 0x200. */

/* gEngineApp_m_Settings (structs/game.h) seen from its difficulty, gEngineApp_m_Settings_difficulty: the
 * ROM bases the CGame and camera-pool reads below on this symbol, and with
 * gEngineApp_m_Settings / gEngineApp_m_Game the code no longer matches. */
typedef struct {
    s32 mask;               /* 0x00 */
    u8 pad4[0xD];
    u8 unk11;               /* 0x11 */
} ViewState_00221210;
extern ViewState_00221210 gEngineApp_m_Settings_difficulty; /* local type: the field's real type (or the member path) changes the ROM's code here */
void CPlayerPool__RegisterActors(void *, CScene *);
s32 Cinema_PathPlaying(void);
s32 GameObject_IsSolid(void *actor);
s32 GameObject_IsMover(void *actor);
s32 Frustum_TestBox(void *frustum, void *box);
void CGameObjectInstance__ClearDrawMatrices(void *actor);
void CInstancePool__UpdateVisibility(void *, CScene *);

void CScene__BuildActorLists(CScene *w) {
    s32 lim, mask, sflag, i, s2, t;
    s32 t0, t1, t2, t3, t4, t5, t6, t7;
    u8 *view;
    CGameObjectInstance *actors;
    CGameObjectInstance *o;
    CIntelligence *info;
    s32 fl;
    f32 *bB;
    f32 *bA;

    mask = gEngineApp_m_Settings_difficulty.mask;
    w->m_nRegistered = 0;
    w->m_nKind1 = 0;
    w->m_nFlagged = 0;
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
    CPlayerPool__RegisterActors(&gEngineApp_m_Game.m_PlayerList, w);
#else
    CPlayerPool__RegisterActors((u8 *)&gEngineApp_m_Settings_difficulty - 0x1270, w);
#endif
    view = NULL;
    if (gEngineApp_m_Settings_difficulty.unk11 == 0) {
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
        view = gEngineApp_m_Game.m_CameraPool->m_ActiveList.m_pHead;
#else
        view = *(u8 **)((u8 *)&gEngineApp_m_Settings_difficulty - 0x1210);
#endif
    }
    sflag = 0;
    if (Cinema_PathPlaying() == 0) {
        sflag = w->m_bStreamAll == 0;
    }
    i = w->m_nFirstActor;
    lim = w->m_nActors;
    actors = (CGameObjectInstance *)w->m_pActors;
    if (i < lim) {
        bB = &gCameraViewBox.m_vMax.x;
        bA = &gCameraVisibleBox.m_vMax.x;
        do {
            o = &actors[i];
            fl = o->m_dwFlags;
            if (fl & 0x100) {
                info = o->header.m_pIntelligence;
                s2 = 0;
                if (info->m_dwFlags & mask) {
                    if (sflag != 0 && !(fl & 2) && o->m_CurrentAnimation.m_bAnimActive == 0 && !(fl & 0x400) && info->m_Type != 3
                        && GameObject_IsSolid(o) == 0) {
                        if (bB[0] > o->m_vMin.x && bB[-3] < o->m_vMax.x && bB[2] > o->m_vMin.z
                            && bB[-1] < o->m_vMax.z && bB[1] > o->m_vMin.y && bB[-2] < o->m_vMax.y) {
                            if (view != NULL) {
#ifdef TARGET_PORT /* the CGame / CCamera fields by name (host layout) */
                                s2 = Frustum_TestBox(((CCamera *)view)->m_ViewVolume, &o->m_vMin.x);
#else
                                s2 = Frustum_TestBox(view + 0x2D8, &o->m_vMin.x);
#endif
                            } else {
                                goto set1;
                            }
                        }
                    } else {
                        s2 = 0;
                        if (bA[0] > o->m_vMin.x && bA[-3] < o->m_vMax.x && bA[2] > o->m_vMin.z
                            && bA[-1] < o->m_vMax.z && bA[1] > o->m_vMin.y && bA[-2] < o->m_vMax.y) {
                        set1:
                            s2 = 1;
                        }
                    }
                }
                if (Cinema_PathPlaying() != 0 && gCinemaHidesActors != 0) {
                    if (!(info->m_dwFlags & 0x200)) {
                        s2 = 0;
                    }
                } else if (!(info->m_dwFlags & 0x100) && info->m_Type != 0xB) {
                    s2 = 0;
                }
                if (s2 != 0) {
                    CGameObjectInstance__ClearDrawMatrices(o);
                    if (GameObject_IsSolid(o) != 0) {
                        t0 = w->m_nDrawActors;
                        if (t0 != 0x200) {
                            w->m_pDrawActors[t0] = (CGameObjectInstance *)o;
                            w->m_nDrawActors = t0 + 1;
                        }
                    }
                    if (GameObject_IsMover(o) != 0) {
                        t1 = w->m_nMovers;
                        if (t1 != 0x80) {
                            w->m_pMovers[t1] = (CGameObjectInstance *)o;
                            w->m_nMovers = t1 + 1;
                        }
                    }
                    t2 = w->m_nRegistered;
                    if (t2 < 0x80) {
                        w->m_pRegistered[t2] = (CGameObjectInstance *)o;
                        w->m_nRegistered = t2 + 1;
                    }
                    t3 = w->m_nKind1;
                    if (t3 < 0x40 && o->header.m_pIntelligence->m_Type == 1) {
                        w->m_pKind1[t3] = (CGameObjectInstance *)o;
                        {
                            s32 u = w->m_nKind1;
                            o->m_AI.m_PathFindCount = u;
                            w->m_nKind1 = u + 1;
                        }
                    }
                    t4 = w->m_nFlagged;
                    if (t4 < 0x20 && (o->m_dwFlags & 2)) {
                        w->m_pFlagged[t4] = (CGameObjectInstance *)o;
                        w->m_nFlagged = t4 + 1;
                    }
                    t5 = w->m_nKind5;
                    if (t5 < 0x10 && o->header.m_pIntelligence->m_Type == 5) {
                        w->m_pKind5[t5] = (CGameObjectInstance *)o;
                        w->m_nKind5 = t5 + 1;
                    }
                    t6 = w->m_nList6;
                    if (t6 < 0x20) {
                        if (o->m_nTypeFlag == 0x64F) {
                            w->m_pList6[t6] = (CGameObjectInstance *)o;
                            w->m_nList6 = t6 + 1;
                        }
                    }
                    if (o->m_nTypeFlag == 0x64D) {
                        t7 = w->m_nList7;
                        if (t7 < 4) {
                            w->m_pList7[t7] = (CGameObjectInstance *)o;
                            w->m_nList7 = t7 + 1;
                        }
                    }
                } else {
                    o->m_dwFlags &= ~0x200;
                }
            }
            i++;
        } while (i < lim);
    }
    CInstancePool__UpdateVisibility(&w->m_InstancePool, w);
    t = w->m_Kind1Cursor + 1;
    w->m_Kind1Cursor = t;
    if (t >= w->m_nKind1) {
        w->m_Kind1Cursor = 0;
    }
}

typedef struct GridCell {
    CRect rect;                  /* 0x00 */
    s32 empty;                  /* 0x10 */
} GridCell;                     /* 0x14 bytes */
typedef struct FloatList {
    s32 pad0;
    s32 count;                  /* 0x04 */
    f32 values[1];              /* 0x08 */
} FloatList;
typedef struct Viewer {
    u8 pad0[0x4];
    struct Viewer *next;        /* 0x004 */
    u8 pad8[0x350 - 0x8];
    f32 min[3];                 /* 0x350 */
    f32 max[3];                 /* 0x35C */
} Viewer;
#define V_NEXT(v) ((Viewer *) ((CCamera *) (v))->m_pNext)
#define V_MIN(v) (&((CCamera *) (v))->m_AnimMin.x)
#define V_MAX(v) (&((CCamera *) (v))->m_AnimMax.x)

/* Loads the grid sections inside the visible bounds that some viewer overlaps. */
void CScene__LoadVisibleGridSections(CScene *world) {
    s32 size;
    s32 nx;
    s32 x0;
    GridCell *cells;
    Viewer *viewers;
    Viewer *v;
    CSceneSector *region;
    GridCell *cell;
    void **handle;
    f32 *xMax;
    f32 *xMin;
    f32 *zMax;
    f32 *zMin;
    FloatList *list;
    s32 nz;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 x;
    s32 z;
    s32 idx;
    s32 found;
    s32 count;

    viewers = gEngineApp_m_Game_m_CameraPool.m_ActiveList.m_pHead;
    if (viewers == NULL) {
        return;
    }
    x0 = 0;
    count = 0;
    list = CISet__GetBlock(world->m_pCollision, 0);
    xMax = list->values;
    nx = list->count;
    xMin = ((FloatList *)CISet__GetBlock(world->m_pCollision, 1))->values;
    x1 = nx - 1;
    list = CISet__GetBlock(world->m_pCollision, 2);
    nz = list->count;
    zMax = list->values;
    zMin = ((FloatList *)CISet__GetBlock(world->m_pCollision, 3))->values;
    cells = (GridCell *)((u8 *)CISet__GetBlock(world->m_pCollision, 4) + 8);
    z1 = nz - 1;
    z0 = 0;
    while (x0 < nx && xMin[x0] < gCameraVisibleBox.m_vMin.x) {
        x0++;
    }
    while (x1 >= 0 && gCameraVisibleBox.m_vMax.x < xMax[x1]) {
        x1--;
    }
    while (z0 < nz && zMin[z0] < gCameraVisibleBox.m_vMin.z) {
        z0++;
    }
    while (z1 >= 0 && gCameraVisibleBox.m_vMax.z < zMax[z1]) {
        z1--;
    }
    region = world->m_Sectors;
    for (z = z0; z <= z1; z++) {
        for (x = x0; x <= x1; x++) {
            idx = x + z * nx;
            cell = &cells[idx];
            if (cell->empty != 0) {
                continue;
            }
            found = 0;
            for (v = viewers; v != NULL; v = V_NEXT(v)) {
                if (V_MIN(v)[0] < cell->rect.maxX && cell->rect.minX < V_MAX(v)[0]
                    && cell->rect.maxZ > V_MIN(v)[2] && cell->rect.minZ < V_MAX(v)[2]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                continue;
            }
            handle = CCache__Request(0, CISet__GetROMAddressAndSize(world->m_pLevelInfo, world->m_LevelInfoKey, idx, &size), size, 8, world,
                                   CScene__DecompressGridSection, sCSceneCacheDesc_GridSection, 0);
            if (handle == NULL) {
                continue;
            }
            if (*(s32 *)*handle != 0) {
                region->m_hData = handle;
                region->m_pBounds = cell;
                region++;
                if (++count == 128) {
                    goto done;
                }
            } else {
                CCache__Unlock(0, handle);
            }
        }
    }
done:
    world->m_nSectors = count;
}

typedef struct List {
    s32 pad0;
    s32 count;                  /* 0x04 */
    u8 items[1];                /* 0x08 */
} List;
void CStaticObject__TouchLitVertices(CStaticObject *inst);
void CLevelActor__Update(CLevelActor *actor);
void CLevelActorPool__CollectVisible(CLevelActorPool *arg0, CScene *world);

/* Collects the instances and actors of the loaded sections inside the visible bounds. */
void CScene__CollectSectorObjects(CScene *world) {
    CVector3 pos;
    s32 regionCount;
    void *data;
    CSceneSector *regions;
    List *list;
    CStaticObject *inst;
    CStaticObject *insts;
    CLevelActor *actors;
    CLevelActor *actor;
    s32 layers;
    s32 i;
    s32 j;
    s32 instCount;
    s32 actorCount;

    i = 0;
    regionCount = world->m_nSectors;
    layers = gEngineApp_m_Settings.difficulty;
    regions = world->m_Sectors;
    for (; i < regionCount; i++) {
        data = *regions[i].m_hData;
        j = 0;
        list = CISet__GetBlock(data, 0);
        instCount = list->count;
        insts = (CStaticObject *)list->items;
        for (; j < instCount; j++) {
            inst = &insts[j];
            if (!(inst->header.m_pIntelligence->m_dwFlags & layers)) {
                continue;
            }
            if (gCameraVisibleBox.m_vMax.x > inst->m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < inst->m_Bounds.m_vMax.x
                && gCameraVisibleBox.m_vMax.z > inst->m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < inst->m_Bounds.m_vMax.z
                && gCameraVisibleBox.m_vMax.y > inst->m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < inst->m_Bounds.m_vMax.y) {
                CStaticObject__TouchLitVertices(inst);
                if (GameObject_IsSolid(inst) && world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)inst;
                }
            }
        }
        list = CISet__GetBlock(data, 1);
        j = 0;
        actorCount = list->count;
        actors = (CLevelActor *)list->items;
        for (; j < actorCount; j++) {
            actor = &actors[j];
            if (!(actor->header.m_pIntelligence->m_dwFlags & layers)) {
                continue;
            }
            if (actor->m_wFlags & 0x41) {
                continue;
            }
            if (gLitVerticesKeyChanged != 0) {
                pos.x = actor->header.m_vPos.x;
                pos.y = actor->header.m_vPos.y + GameObject_GetCollisionHeight(actor) * 0.5f;
                pos.z = actor->header.m_vPos.z;
                CScene__GetLightsAt(world, &pos, &actor->m_Lights);
            }
            if (gCameraVisibleBox.m_vMax.x > actor->m_Bounds.m_vMin.x && gCameraVisibleBox.m_vMin.x < actor->m_Bounds.m_vMax.x
                && gCameraVisibleBox.m_vMax.z > actor->m_Bounds.m_vMin.z && gCameraVisibleBox.m_vMin.z < actor->m_Bounds.m_vMax.z
                && gCameraVisibleBox.m_vMax.y > actor->m_Bounds.m_vMin.y && gCameraVisibleBox.m_vMin.y < actor->m_Bounds.m_vMax.y) {
                CLevelActor__Update(actor);
                if (world->m_nDrawActors != 0x200) {
                    world->m_pDrawActors[world->m_nDrawActors++] = (CGameObjectInstance *)actor;
                }
            }
        }
    }
    CLevelActorPool__CollectVisible(&world->m_ActorPool, world);
}
void CScene__BuildGeometryDisplayList(CScene *w);
/* Builds (once per frame) the display list of the level geometry: for every
 * grid region seen by one of the views, every static object and every level
 * actor shown in this view and inside a view's frustum is drawn, or, when
 * translucent, queued on the world's sorted list (0x1064, drawn later by
 * CScene__DrawSortedList). The list is built in place behind a branch command that
 * jumps over it, and later views just call it (0xDE000000). */

/* gEngineApp_m_Settings (structs/game.h) seen from its difficulty, gSettingsDifficultyView_00221D3C: the
 * ROM bases the CGame and camera-pool reads below on this symbol, and with
 * gEngineApp_m_Settings / gEngineApp_m_Game the code no longer matches. */
typedef struct {
    s32 mask;               /* 0x00 */
} ViewState_00221D3C;
extern ViewState_00221D3C gSettingsDifficultyView_00221D3C __asm__("gEngineApp_m_Settings_difficulty"); /* local type: the field's real type (or the member path) changes the ROM's code here */

void CScene__BuildGeometryDisplayList(CScene *w) {
    s32 r;
    s32 n;
    u8 *objs;
    CCamera *views;
    s32 mask;
    s32 vmask;
    CCamera *v;
    CSceneSector *regions;
    CSceneSector *region;
    CRect *cell;
    void *grp;
    StrideTable *t;
    CStaticObject *o;
    CLevelActor *actors;
    CLevelActor *a;
    s32 k;
    s32 off;
    s32 cnt;
    s32 cnt2;
    s32 found;
    s32 c;

    mask = gSettingsDifficultyView_00221D3C.mask;
    vmask = gCameraVisBits;
    if (gSceneGeometryDL == NULL) {
#ifdef TARGET_PORT
        /* the N64 reads gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead 0x1210 bytes before
         * the difficulty; on the host the two are 0x1358 apart (round 18 g2, n64viewcheck gptr) */
        views = (CCamera *) gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
#else
        views = *(CCamera **)((u8 *)&gSettingsDifficultyView_00221D3C - 0x1210);
#endif
        if (views == NULL) {
            return;
        }
        gSceneGeometryDL = gDList;
        {
            Gfx *g = gDList++;
            g->w0 = 0xDE010000;
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE7000000;
            g->w1 = 0;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xE3000A01;
            g->w1 = 0x100000;
        }
        Render_ResetBatches();
        w->m_nDeferred = 0;
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040004;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04000C;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFE;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFE;
        }
        r = 0;
        n = w->m_nSectors;
        regions = w->m_Sectors;
        if (n > 0) {
            do {
                region = regions + r;
                v = views;
                found = 0;
                while (v != NULL) {
                    cell = region->m_pBounds;
                    if (cell->maxX > v->m_ViewMin.x && cell->minX < v->m_ViewMax.x
                        && cell->maxZ > v->m_ViewMin.z && cell->minZ < v->m_ViewMax.z) {
                        found = 1;
                        break;
                    }
                    v = v->m_pNext;
                }
                if (found) {
                    CCache__LockForDisplayList(0, region->m_hData);
                    k = 0;
                    grp = *region->m_hData;
                    t = CISet__GetBlock(grp, k);
                    cnt = t->count;
                    objs = t->entries;
                    if (cnt > 0) {
                        off = k;
                        do {
                            o = (CStaticObject *)(objs + off);
                            if (o->header.m_pIntelligence->m_dwFlags & mask) {
                                if (vmask & o->m_ViewMask) {
                                    v = views;
                                    found = 0;
                                    while (v != NULL) {
                                        if (v->m_ViewMax.x > o->m_Bounds.m_vMin.x && v->m_ViewMin.x < o->m_Bounds.m_vMax.x
                                            && v->m_ViewMax.z > o->m_Bounds.m_vMin.z && v->m_ViewMin.z < o->m_Bounds.m_vMax.z
                                            && v->m_ViewMax.y > o->m_Bounds.m_vMin.y && v->m_ViewMin.y < o->m_Bounds.m_vMax.y
                                            && Frustum_TestBox(v->m_ViewVolume, &o->m_Bounds)) {
                                            found = 1;
                                            break;
                                        }
                                        v = v->m_pNext;
                                    }
                                    if (found) {
                                        o->m_pOwner = region->m_hData;
                                        if (o->m_wFlags & 4) {
                                            c = w->m_nDeferred;
                                            if (c != 0x40) {
                                                w->m_pDeferred[c] = o;
                                                w->m_nDeferred = c + 1;
                                            }
                                        } else {
                                            CStaticObject__Draw(o, views);
                                        }
                                    }
                                }
                            }
                            k++;
                            off += sizeof(CStaticObject);
                        } while (k < cnt);
                    }
                    t = CISet__GetBlock(grp, 1);
                    k = 0;
                    cnt2 = t->count;
                    actors = (CLevelActor *)t->entries;
                    if (cnt2 > 0) {
                        do {
                            a = &actors[k];
                            if (!(a->header.m_pIntelligence->m_dwFlags & mask)) {
                                goto next;
                            }
                            if (a->m_wFlags & 1) {
                                goto next;
                            }
                            if (!(a->m_wFlags & 2)) {
                                goto next;
                            }
                            if (vmask & a->m_ViewMask) {
                                v = views;
                                found = 0;
                                while (v != NULL) {
                                    if (v->m_ViewMax.x > a->m_Bounds.m_vMin.x && v->m_ViewMin.x < a->m_Bounds.m_vMax.x
                                        && v->m_ViewMax.z > a->m_Bounds.m_vMin.z && v->m_ViewMin.z < a->m_Bounds.m_vMax.z
                                        && v->m_ViewMax.y > a->m_Bounds.m_vMin.y && v->m_ViewMin.y < a->m_Bounds.m_vMax.y
                                        && Frustum_TestBox(v->m_ViewVolume, &a->m_Bounds)) {
                                        found = 1;
                                        break;
                                    }
                                    v = v->m_pNext;
                                }
                                if (found) {
                                    if (a->m_wFlags & 0x20) {
                                        c = w->m_nDeferred;
                                        if (c != 0x40) {
                                            w->m_pDeferred[c] = a;
                                            w->m_nDeferred = c + 1;
                                        }
                                    } else {
                                        CLevelActor__Draw(a);
                                    }
                                }
                            }
                        next:
                            k++;
                        } while (k < cnt2);
                    }
                }
                r++;
            } while (r < n);
        }
        Render_FlushBatches();
        {
            Gfx *g = gDList++;
            g->w0 = 0xDF000000;
            g->w1 = 0;
        }
        {
            Gfx *g = gSceneGeometryDL++;
            g->w0 = 0xDE010000;
            g->w1 = GBI_ADDR(gDList);
        }
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xDE000000;
        g->w1 = GBI_ADDR(gSceneGeometryDL);
    }
}

CPlayer *CCamera__GetPlayer(CCamera *);
s32 CPlayer__GetPickupCountLow(CPlayer *, s32);

void CScene__DrawGeometry(CScene *w, CCamera *v_) {
    register CCamera *v asm("$18") = v_;
    u8 *base;
    void *grp;
    s32 i;
    s32 n;
    s32 mask;
    s32 vmask;
    s32 idx;
    s32 flag;
    CPlayer *player;
    IdTable *tbl;
    s32 cnt;
    s32 si;
    u16 *ip;
    StrideTable *t;
    CSceneSector *regions;
    CSceneSector *region;
    CRect *cell;
    CStaticObject *a;
    CLevelActor *p;
    CLevelActor *actors;
    s32 k;
    s32 cnt1;
    s32 cnt2;
    s32 off;
    s32 c;

    mask = gEngineApp_m_Settings.difficulty;
    vmask = v->m_ViewParams.m_VisBits;
    if (gEngineApp_m_Settings.multiplayer != 0) {
        CScene__BuildGeometryDisplayList(w);
        return;
    }
    player = CCamera__GetPlayer(v);
    si = 0;
    tbl = w->m_pObjectTypes;
    cnt = tbl->count;
    ip = tbl->ids;
    if (cnt > 0) {
        do {
            if (ip[si] == 0x33) {
                idx = si;
                goto found;
            }
            si++;
        } while (si < cnt);
    }
    idx = -1;
found:
    if (player != NULL && idx != -1 && CPlayer__GetPickupCountLow(player, 0x111F) != 0 && gEngineApp_m_Settings.multiplayer == 0) {
        flag = 1;
    } else {
        flag = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE7000000;
        g->w1 = 0;
    }
    {
        Gfx *g = gDList++;
        g->w0 = 0xE3000A01;
        g->w1 = 0x100000;
    }
    Render_ResetBatches();
    w->m_nDeferred = 0;
    if (player != NULL && (player->m_ModeFlags & 0x100)) {
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040004;
            g->w1 = 1;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04000C;
            g->w1 = 1;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFF;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFF;
        }
    } else {
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040004;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04000C;
            g->w1 = 2;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB040014;
            g->w1 = 0xFFFE;
        }
        {
            Gfx *g = gDList++;
            g->w0 = 0xDB04001C;
            g->w1 = 0xFFFE;
        }
    }
    do {
        i = 0;
        n = w->m_nSectors;
        regions = w->m_Sectors;
    } while (0);
    if (n > 0) {
        do {
            region = regions + i;
            cell = region->m_pBounds;
            if (cell->maxX > v->m_ViewMin.x && cell->minX < v->m_ViewMax.x
                && cell->maxZ > v->m_ViewMin.z && cell->minZ < v->m_ViewMax.z) {
                CCache__LockForDisplayList(0, region->m_hData);
                k = 0;
                grp = *region->m_hData;
                t = CISet__GetBlock(grp, k);
                cnt1 = t->count;
                base = t->entries;
                if (cnt1 > 0) {
                    off = k;
                    do {
                        a = (CStaticObject *)(base + off);
                        if (a->header.m_pIntelligence->m_dwFlags & mask) {
                            a->m_pOwner = region->m_hData;
                            if (gFrameCountGameplay == 0) {
                                CStaticObject__RequestLitVertices(a, v);
                            }
                            if ((vmask & a->m_ViewMask)
                                && v->m_ViewMax.x > a->m_Bounds.m_vMin.x && v->m_ViewMin.x < a->m_Bounds.m_vMax.x
                                && v->m_ViewMax.z > a->m_Bounds.m_vMin.z && v->m_ViewMin.z < a->m_Bounds.m_vMax.z
                                && v->m_ViewMax.y > a->m_Bounds.m_vMin.y && v->m_ViewMin.y < a->m_Bounds.m_vMax.y
                                && Frustum_TestBox(v->m_ViewVolume, &a->m_Bounds) != 0) {
                                if ((a->m_wFlags & 8) && (a->header.m_nObjType != idx || flag != 0)) {
                                    c = w->m_nListed;
                                    if (c != 0x40) {
                                        w->m_pListed[c] = (CGameObjectInstance *)a;
                                        w->m_nListed = c + 1;
                                    }
                                }
                                if (a->m_wFlags & 4) {
                                    c = w->m_nDeferred;
                                    if (c != 0x40) {
                                        w->m_pDeferred[c] = a;
                                        w->m_nDeferred = c + 1;
                                    }
                                } else {
                                    CStaticObject__Draw(a, v);
                                }
                            }
                        }
                        k++;
                        off += sizeof(CStaticObject);
                    } while (k < cnt1);
                }
                t = CISet__GetBlock(grp, 1);
                k = 0;
                cnt2 = t->count;
                actors = (CLevelActor *)t->entries;
                if (cnt2 > 0) {
                    do {
                        p = actors + k;
                        if (p->header.m_pIntelligence->m_dwFlags & mask) {
                            u16 fl = p->m_wFlags;

                            if (!(fl & 1) && (vmask & p->m_ViewMask)
                                && v->m_ViewMax.x > p->m_Bounds.m_vMin.x && v->m_ViewMin.x < p->m_Bounds.m_vMax.x
                                && v->m_ViewMax.z > p->m_Bounds.m_vMin.z && v->m_ViewMin.z < p->m_Bounds.m_vMax.z
                                && v->m_ViewMax.y > p->m_Bounds.m_vMin.y && v->m_ViewMin.y < p->m_Bounds.m_vMax.y
                                && (fl & 2) && Frustum_TestBox(v->m_ViewVolume, &p->m_Bounds) != 0) {
                                if (p->m_wFlags & 0x20) {
                                    c = w->m_nDeferred;
                                    if (c != 0x40) {
                                        w->m_pDeferred[c] = p;
                                        w->m_nDeferred = c + 1;
                                    }
                                } else {
                                    CLevelActor__Draw(p);
                                }
                            }
                        }
                        k++;
                    } while (k < cnt2);
                }
            }
            i++;
        } while (i < n);
    }
    Render_FlushBatches();
}

/* Collects up to 10 items of the region containing pos whose bounds hold it. */
void CScene__FindLightsAt(CScene *world, f32 *pos, CLightVolume **out, s32 *outCount) {
    CSceneSector *found;
    CLightVolumeTable *list;
    CLightVolume *item;
    CLightVolume *items;
    s32 i;
    s32 count;
    s32 n;
    s32 regionCount;
    CBox *b;
    CRect *r;
    f32 x, z;

    found = NULL;
    count = 0;
    x = pos[0];
    z = pos[2];
    regionCount = world->m_nSectors;
    for (i = 0; i < regionCount; i++) {
        r = world->m_Sectors[i].m_pBounds;
        if (r->maxX < x || x < r->minX || r->maxZ < z || z < r->minZ) {
            continue;
        }
        found = &world->m_Sectors[i];
        break;
    }
    if (found == NULL) {
        *outCount = 0;
        return;
    }
    list = CISet__GetBlock(*found->m_hData, 2);
    n = list->count;
    items = list->volumes;
    for (i = 0; i < n; i++) {
        item = &items[i];
        b = &item->m_Bounds;
        if (b->m_vMax.x > pos[0] && b->m_vMax.y > pos[1] && b->m_vMax.z > pos[2]
            && b->m_vMin.x <= pos[0] && b->m_vMin.y <= pos[1] && b->m_vMin.z <= pos[2]) {
            out[count++] = item;
            if (count == 10) {
                break;
            }
        }
    }
    *outCount = count;
}

/* gEngineApp_m_Settings (GameSettings in structs/game.h: cheatFlags, unkC). Kept as a local
 * array view: through it the compiler bases both loads on gEngineApp_m_Settings + 0xC, as
 * the ROM does; through the struct it bases them on gEngineApp_m_Settings. */
typedef struct B19bG8 { u32 flags; u8 pad4[0x8]; u32 fC; } B19bG8;
extern B19bG8 gSettingsView[] __asm__("gEngineApp_m_Settings"); /* local type: array view, see above */
void CMtxF__TransformPoint(void *, void *, void *);
#ifdef TARGET_PORT
/* The port: the point as three floats (the definition's host signature, func_002123D4.c). */
s32 Actors_GetLightVolumeFalloff(void *, f32, f32, f32, void *, f32 *);
#else
s32 Actors_GetLightVolumeFalloff(void *, B19bV3f, void *, f32 *);
#endif

void CVector3__Add(void *, void *, void *);
void CMtxF__TransformPointProjective(void *, void *, void *);
void CVector3__Subtract(void *out, void *a, void *b);
void CVector3__Normalize(void *);
f32 LightVolume_GetFlicker(void *);

void CScene__ComputeLights(B19bLObj **ents, s32 n, f32 *pos, u8 *out) {
    f32 w[128];
    B19bV3f dirs[128];
    B19bV3f t0, a, b, c, d, e, f, g, h, i, j;
    B19bLObj *last;
    B19bLObj *o;
    f32 x2, u0, u1, x, scale, s0, s1, s2;
    s32 k, v;
    f32 *wp, *w1, *wb, *pw;
    B19bV3f *dd;
    f32 zero;
    B19bV3f *db;
    B19bV3f *dp;
    B19bLObj **ep;
    f32 m1_0, m2_0;
    f32 m1_1, m2_1;
    f32 m1_2, m2_2;
    f32 m1_3, m2_3;
    f32 m1_4, m2_4;
    f32 m1_5, m2_5;
    scale = (f32)gSettingsView->fC * 0.003921569f;
    last = 0;
    if (gSettingsView->flags & 0x4000) {
        out[0] = out[4] = 0;
        out[1] = out[5] = 0;
        out[2] = out[6] = 0;
        out[8] = out[0xC] = 0;
        out[9] = out[0xD] = 0;
        out[0xA] = out[0xE] = 0;
        out[0x10] = 0x7F;
        out[0x11] = 0;
        out[0x12] = 0;
        return;
    }
    k = 0;
    a.x = a.y = a.z = 0.0f;
    c.x = c.y = c.z = 0.0f;
    d.x = d.y = d.z = 0.0f;
    e.x = e.y = e.z = 0.0f;
    if (n > 0) {
        wb = w;
        db = dirs;
        do {
            o = ents[k];
            CMtxF__TransformPoint((u8 *)o + 0x20, pos, &t0);
#ifdef TARGET_PORT
            if (Actors_GetLightVolumeFalloff(o, t0.x, t0.y, t0.z, &g, &wb[k]) != 0) {
#else
            if (Actors_GetLightVolumeFalloff(o, t0, &g, &wb[k]) != 0) {
#endif
                CVector3__Add(&h, &g, &t0);
                CMtxF__TransformPointProjective((u8 *)o + 0x60, &h, &g);
                CVector3__Subtract((&db[k]), pos, &g);
                CVector3__Normalize((&db[k]));
                b = o->pos;
                if (o->fBA != 0) {
                    i.z = i.y = i.x = LightVolume_GetFlicker(o);
                    CVector3__Scale(&f, &i, wb[k] * o->f4);
                    last = o;
                    if (o->fB8 != 0) CVector3__Scale(&f, &f, -1.0f);
                    CVector3__Add(&c, &c, &f);
                }
                u1 = o->f4 * 0.5f;
                u0 = 1.0f - u1;
                CVector3__Scale(&f, &b, wb[k] * u1);
                if (o->fB8 != 0) CVector3__Scale(&f, &f, -1.0f);
                CVector3__Add(&c, &c, &f);
                wb[k] *= u0;
                x = b.x;
                if (x < 0.0f) x = -x;
                if (b.y < 0.0f) x2 = x - b.y; else x2 = x + b.y;
                if (b.z < 0.0f) u0 = x2 - b.z; else u0 = x2 + b.z;
                CVector3__Scale(&f, (&db[k]), wb[k]);
                CVector3__Scale(&f, &f, u0);
                CVector3__Add(&a, &a, &f);
                if (o->fB8 != 0) wb[k] = -wb[k];
            } else {
                (&db[k])->x = 0.0f;
                (&db[k])->y = 0;
                (&db[k])->z = 0;
                wb[k] = 0.0f;
            }
                        k++;
        } while (k < n);
    }
    CVector3__Normalize(&a);
    do {
        k = 0;
    } while (0);
    if (n > 0) {
        wp = w;
        s1 = 1.0f;
        zero = 0.0f;
        do {
            o = ents[k];
            pw = &wp[k];
#ifdef TARGET_PORT
            /* dirs[k]: the ROM reaches it as w + 128 because its frame puts dirs right after w;
             * a host frame need not (macOS arm64 read other stack words there, so the actors'
             * light colours changed with unrelated code, e.g. T2_ACTOR_LOG) */
            s2 = a.x * dirs[k].x + a.y * dirs[k].y + a.z * dirs[k].z;
#else
            s2 = a.x * ((B19bV3f *)(wp + 128))[k].x + a.y * ((B19bV3f *)(wp + 128))[k].y + a.z * ((B19bV3f *)(wp + 128))[k].z;
#endif
            s0 = s1 - s2;
            if (s2 < zero) s0 = s1;
            s0 = s0 * 0.5f;
            b = o->pos;
            s2 = s1 - s0;
            if (o == last) {
                i.z = i.y = i.x = LightVolume_GetFlicker(o);
                CVector3__Scale(&j, &i, s2 * *pw);
                CVector3__Add(&e, &e, &j);
                CVector3__Scale(&j, &i, s0 * *pw);
                CVector3__Add(&d, &d, &j);
            }
            CVector3__Scale(&f, &b, s2 * *pw);
            CVector3__Add(&e, &e, &f);
            CVector3__Scale(&f, &b, s0 * *pw);
            CVector3__Add(&d, &d, &f);
            k++;
        } while (k < n);
    }
    CVector3__Scale(&d, &d, 0.5f);
    CVector3__Add(&c, &c, &d);
    if (c.x < 0.0f) e.x += c.x;
    if (c.y < 0.0f) e.y += c.y;
    if (c.z < 0.0f) e.z += c.z;
    m1_0 = c.x;
    if (!(m1_0 <= 255.0f)) m1_0 = 255.0f;
    m2_0 = m1_0;
    if (!(m2_0 >= 5.0f)) m2_0 = 5.0f;
    c.x = m2_0 * (1.0f - scale) + scale * 255.0f;
    m1_1 = c.y;
    if (!(m1_1 <= 255.0f)) m1_1 = 255.0f;
    m2_1 = m1_1;
    if (!(m2_1 >= 5.0f)) m2_1 = 5.0f;
    c.y = m2_1 * (1.0f - scale) + scale * 255.0f;
    m1_2 = c.z;
    if (!(m1_2 <= 255.0f)) m1_2 = 255.0f;
    m2_2 = m1_2;
    if (!(m2_2 >= 5.0f)) m2_2 = 5.0f;
    c.z = m2_2 * (1.0f - scale) + scale * 255.0f;
    m1_3 = e.x;
    if (!(m1_3 <= 255.0f)) m1_3 = 255.0f;
    m2_3 = m1_3;
    if (!(m2_3 >= 5.0f)) m2_3 = 5.0f;
    e.x = m2_3 * (1.0f - scale) + scale * 255.0f;
    m1_4 = e.y;
    if (!(m1_4 <= 255.0f)) m1_4 = 255.0f;
    m2_4 = m1_4;
    if (!(m2_4 >= 5.0f)) m2_4 = 5.0f;
    e.y = m2_4 * (1.0f - scale) + scale * 255.0f;
    m1_5 = e.z;
    if (!(m1_5 <= 255.0f)) m1_5 = 255.0f;
    m2_5 = m1_5;
    if (!(m2_5 >= 5.0f)) m2_5 = 5.0f;
    e.z = m2_5 * (1.0f - scale) + scale * 255.0f;
    v = (u32)c.x;
    out[0] = out[4] = v;
    v = (u32)c.y;
    out[1] = out[5] = v;
    v = (u32)c.z;
    out[2] = out[6] = v;
    v = (u32)e.x;
    out[8] = out[0xC] = v;
    v = (u32)e.y;
    out[9] = out[0xD] = v;
    v = (u32)e.z;
    out[0xA] = out[0xE] = v;
    out[0x10] = (s32)(a.x * 127.0f);
    out[0x11] = (s32)(a.y * 127.0f);
    out[0x12] = (s32)(a.z * 127.0f);
}

typedef struct Node {
    /* 0x00 */ s32 index;
    /* 0x04 */ void *prev;
    /* 0x08 */ void *next;
} Node;
s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);

/* Copies up to max placed objects of the given kind (-1: any) whose key matches into out; returns how many. */
VM_TEXT s32 CScene__FindWarpDests(void *world, s32 kind, s32 key, Placed *out, s32 max) {
    ListRec list;
    Node nodes[64];
    sptr resource;          /* the index (a pointer) */
    s32 first;
    s32 last;
    s32 found;
    s32 *keys;
    s32 *objectTable;
    Placed *objects;
    Node *node;
    s32 i;
    s32 match;
    s32 count;
    void **handle;
    void *table;
    s32 pool;

    found = 0;
    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(found, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    handle = CCache__LoadPersistantSubBlock_v(found, resource, 8, pool, world, 0, "warp dests");
    table = *handle;
    count = (keys = CISet__GetBlock(table, 0))[1];
    keys += 2;
    objectTable = CISet__GetBlock(table, 1);
    objects = (Placed *)(objectTable + 2);
    if (BinaryRange(keys, count, key, &first, &last) != 0) {
        CList__Construct(&list, T2_OFFSETOF(Node, prev), T2_OFFSETOF(Node, next));
        node = nodes;
        for (i = first; i <= last; i++) {
            match = 1;
            if (kind != -1) {
                match = objects[i].kind == kind;
            }
            if (match) {
                node->index = i;
                CList__AddHead(&list, node);
                node++;
            }
        }
        for (max--; max != -1; max--) {
            if (list.count == 0) {
                break;
            }
            node = CList__GetRandom(&list);
            out[found++] = objects[node->index];
            CList__Remove(&list, node);
        }
    }
    CCache__Deallocate(0, handle);
    return found;
}

#define T2_AI(e) ((u8 *) &(e)->m_AI) /* the AI by member (N64 0x140) */

f32 GameObject_GetCollisionYOffset(CGameObjectInstance *e);
s32 Action_CheckRequiredItem(CGameObjectInstance *e, u8 *p, CGameObjectInstance *me);

void CScene__CheckAutoActivation(CScene *lv, CGameObjectInstance *me, CVector3 *p0, CVector3 *p1) {
    s32 isB;
    s32 is1;
    f32 myBot;
    f32 myTop;
    s32 i;
    CGameObjectInstance *e;
    f32 eBot;
    f32 eTop;
    s32 kind;
    s32 s1;
    s32 s2;
    s32 s3;
    s32 s4;
    s32 a0;
    s32 v1;
    s32 a2;
    s32 a1;
    s32 hit;
    s32 hit2;
    f32 r1;
    register f32 r2 asm("$f20");
    f32 dx;
    f32 dz;
    f32 d0;
    f32 d1;
    f32 ex;
    f32 ez;
    s32 ok;
    u32 fl;
    if (p0->x != p1->x || p0->y != p1->y || p0->z != p1->z) {
        kind = me->header.m_pIntelligence->m_Type;
        isB = kind == 0xB;
        is1 = kind == 1;
        myBot = me->header.m_vPos.y + GameObject_GetCollisionYOffset(me);
        myTop = myBot + GameObject_GetCollisionHeight(me);
        for (i = 0; i < lv->m_nFlagged; i++) {
            e = (CGameObjectInstance *)lv->m_pFlagged[i];
            if (e != me) {
                s4 = 0;
                s1 = 0;
                eBot = e->header.m_vPos.y + GameObject_GetCollisionYOffset(e);
                eTop = eBot + GameObject_GetCollisionHeight(e);
                switch (e->header.m_pIntelligence->m_Type) {
                case 6:
                    s2 = 0;
                    s3 = s4 = 1;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Door.m_AutoOpenRadius;
                    r2 = e->header.m_pIntelligence->m_Door.m_AutoCloseRadius;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    a2 = fl & 0x10;
                    a1 = fl & 0x20;
                    break;
                case 10:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Action.m_AutoGoRadius;
                    r2 = 0.0f;
                    a0 = fl & 4;
                    v1 = fl & 8;
                    break;
                case 7:
                    s2 = 1;
                    s3 = 0;
                    a2 = s3;
                    a1 = s3;
                    s1 = s2;
                    fl = e->header.m_pIntelligence->m_dwTypeFlags;
                    r1 = e->header.m_pIntelligence->m_Destructible.m_AutoGoRadius;
                    r2 = 0.0f;
                    a0 = fl & 8;
                    v1 = fl & 0x10;
                    break;
                default:
                    s2 = 0;
                    r1 = 0.0f;
                    a0 = 0;
                    v1 = 0;
                    s3 = 0;
                    a2 = 0;
                    a1 = 0;
                    r2 = 0.0f;
                    break;
                }
                if (s1 != 0 && (myTop < eBot || eTop < myBot)) {
                    hit2 = hit = 0;
                } else {
                    hit = 0;
                    if (isB != 0 && a0 != 0) {
                        hit = 1;
                    } else if (is1 != 0 && v1 != 0) {
                        hit = 1;
                    }
                    hit2 = 0;
                    if ((isB != 0 && a2 != 0) || (is1 != 0 && a1 != 0)) {
                        hit2 = 1;
                    }
                }
                if (hit != 0 || hit2 != 0) {
                    d0 = (p0->x - e->header.m_vPos.x) * (p0->x - e->header.m_vPos.x) + (p0->z - e->header.m_vPos.z) * (p0->z - e->header.m_vPos.z);
                    d1 = (p1->x - e->header.m_vPos.x) * (p1->x - e->header.m_vPos.x) + (p1->z - e->header.m_vPos.z) * (p1->z - e->header.m_vPos.z);
                    if (hit != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (r1 * r1 < d0) {
                                if (d1 <= r1 * r1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(d1 <= r1 * r1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            if (e->header.m_pIntelligence->m_Type == 10 && s2 == 1) {
                                ok = Action_CheckRequiredItem(e, T2_AI(e), me);
                            }
                            if (ok != 0) {
                                CAI__SetMode(e, T2_AI(e), s2);
                            }
                        }
                    }
                    if (hit2 != 0) {
                        if (s4 != 0) {
                            ok = 0;
                            if (d0 <= r2 * r2) {
                                if (r2 * r2 < d1) {
                                    ok = 1;
                                }
                            }
                        } else {
                            ok = 1;
                            if (!(r2 * r2 < d1)) {
                                ok = 0;
                            }
                        }
                        if (ok != 0) {
                            CAI__SetMode(e, T2_AI(e), s3);
                        }
                    }
                }
                __asm__ volatile("" : : "f"(r2));
            }
        }
    }
}

extern CGameFromCameraPool gCameraPoolGame_002237C4 __asm__("gEngineApp_m_Game_m_CameraPool"); /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
void Cinema_Reset(CWarpFadeColor *entry);
void Cinema_StopAtEnd(void);
void Cinema_RequestById(s32 arg0);
s32 Demo_GetPlayerCount(void);
void CCameraPool__AddTimedText(CGameFromCameraPool *game, void *camera, u8 *text, f32 time);

static __inline__ s32 transitionId(CScene *scene) {
    if (scene->m_bWarpSpawn != 0) {
        return scene->m_WarpSpawn.m_nLevel;
    }
    return scene->m_WarpID;
}

/* Runs the fade of a level transition: state 1 fades in to the entry length and then warps
 * (CScene__FindWarpDests, or straight to ~id for a negative id), state 2 fades back out. */
void CScene__UpdateWarp(CScene *scene) {
    CWarpFadeColor *entry = &gWarpFadeColors[scene->m_WarpFade];
    s32 id;
    GameSettings *settings;

    switch (scene->m_WarpState) {
        case 0:
            break;
        case 1:
            if (scene->m_WarpTime == entry->length) {
                Cinema_Reset(entry);
                if (scene->m_WarpID == 999) {
                    Cinema_StopAtEnd();
                    Cinema_RequestById(400);
                }
                if (scene->m_WarpFade == 0) {
                    scene->m_WarpState = 0;
                } else {
                    scene->m_WarpState = 2;
                }
                if (scene->m_WarpID < 0) {
                    scene->m_WarpID = ~scene->m_WarpID;
                    scene->m_bWarpSpawn = 0;
                } else {
                    scene->m_bWarpSpawn = CScene__FindWarpDests(scene, -1, scene->m_WarpID, &scene->m_WarpSpawn, 1);
                }
                scene->m_LevelID = scene->m_WarpID;
                id = transitionId(scene);
                CScene__LoadLevel(scene, id, scene->m_bWarpSpawn);
                if (scene->m_bWarpSpawn != 0) {
                    settings = &gCameraPoolGame_002237C4.m_Settings;
                    if (scene->m_PreviewOptions != settings->unlockedCheats && !Demo_GetPlayerCount()) {
#ifdef TARGET_PORT
                        CCameraPool__AddTimedText(&gCameraPoolGame_002237C4, &gCameraPoolGame_002237C4.m_CameraPool[0].m_MainCamera, sCheatMessageText_NewCheatEnabled, 4.0f);
#else
                        CCameraPool__AddTimedText(&gCameraPoolGame_002237C4, gCameraPoolGame_002237C4.m_CameraPool + 0x40, sCheatMessageText_NewCheatEnabled, 4.0f);
#endif
                        scene->m_PreviewOptions = settings->unlockedCheats;
                    }
                }
            } else {
                scene->m_WarpTime += gFrameIncrement;
                if (entry->length < scene->m_WarpTime) {
                    scene->m_WarpTime = entry->length;
                }
            }
            break;
        case 2:
            if (scene->m_WarpTime == 0.0f) {
                scene->m_WarpState = 0;
            } else {
                scene->m_WarpTime -= gFrameIncrement;
                if (scene->m_WarpTime < 0.0f) {
                    scene->m_WarpTime = 0.0f;
                }
            }
            break;
    }
}

void *func_0029E460(void *, void *, s32);           /* memcpy */

/* Copies the level's name (sub-block 8 of its level file, a handle: data then size) into
 * arg2, at most arg3 bytes. The level file stays loaded when it is the current level. */
/* The one object's literals on the N64: "level index" is CScene__LoadLevel's (pooled), "level name" is output
 * here, between func_0042C194's and func_00223A94's .rodata (0xA6BD0); the port reads the generated copies. */
#ifdef TARGET_PORT
#define SCENE_DESC_LEVEL_INDEX ((sptr) &sCSceneCacheDesc_LevelIndex)
#define SCENE_DESC_LEVEL_NAME ((sptr) &sCSceneCacheDesc_LevelName)
#else
#define SCENE_DESC_LEVEL_INDEX ((sptr) "level index")
#define SCENE_DESC_LEVEL_NAME ((sptr) "level name")
#endif
DEFERRED void CScene__GetLevelName(struct CScene *arg0, s32 arg1, void *arg2, s32 arg3) {
    sptr sp20;
    sptr temp_s2;
    s32 temp_v0;
    s32 var_a2;
    CMemEntry *temp_v0_2;

    temp_v0 = CISet__GetROMAddress((sptr) arg0->m_pLevels, arg0->m_LevelsKey, arg1);
    temp_s2 = CCache__LoadPersistantIndex(0, (sptr) &sp20, temp_v0, SCENE_DESC_LEVEL_INDEX);
    temp_v0_2 = (CMemEntry *) CCache__LoadPersistantSubBlock_v(0, sp20, 8, temp_v0, (sptr) arg0, 0, SCENE_DESC_LEVEL_NAME);
    var_a2 = temp_v0_2->size;
    if (arg3 < var_a2) {
        var_a2 = arg3;
    }
    func_0029E460(arg2, temp_v0_2->data, var_a2);
    CCache__Deallocate(0, (sptr) temp_v0_2);
    if (arg0->m_nLevel != arg1) {
        CCache__Deallocate(0, temp_s2);
    }
}

typedef struct Entity_00223A94 Entity_00223A94;
#ifdef TARGET_PORT
struct Entity_00223A94 {
/* the view at the host offsets of CListedInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Entity(m) __builtin_offsetof(CListedInstance, m)
    union {
        struct { u8 _hp_kind[T2_HOFF_Entity(m_Instance.header.m_Type)]; u8 kind; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_Entity(m_Instance.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_Entity(m_Instance.header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_typeId[T2_HOFF_Entity(m_Instance.m_nTypeFlag)]; u16 typeId; }; /* N64 0xB8 */
        struct { u8 _hp_ai[T2_HOFF_Entity(m_Instance.m_AI.m_dwFlags)]; u8 ai[4]; }; /* N64 0x140 */
        struct { u8 _hp_health[T2_HOFF_Entity(m_Instance.m_AI.m_Health)]; s32 health; }; /* N64 0x144 */
        struct { u8 _hp_state[T2_HOFF_Entity(m_Instance.m_AI.m_Mode)]; s8 state; }; /* N64 0x174 */
        struct { u8 _hp_next[T2_HOFF_Entity(m_pNext)]; Entity_00223A94 *next; }; /* N64 0x28C */
    };
};
#else
struct Entity_00223A94 {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[3];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0xB8 - 0x18];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0x140 - 0xBA];
    /* 0x140 */ u8 ai[4];
    /* 0x144 */ s32 health;
    /* 0x148 */ u8 pad148[0x174 - 0x148];
    /* 0x174 */ s8 state;
    /* 0x175 */ u8 pad175[0x28C - 0x175];
    /* 0x28C */ Entity_00223A94 *next;
};
#endif

void DamageEvent_Set(void *event, sptr source, s32 damage, f32 radius, s32 flags, sptr frame);
void CAI__CallHurt(void *obj, void *ai, void *event);
s32 CAI__SetMode(void *obj, void *ai, s32 mode);

/* Blast_GetStrength inlined: the strength (0 or 0xFF) a blast at `target` hits `e` with. */
static __inline__ s32 blastStrength(Entity_00223A94 *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    if (e->kind == 1 && e->def->m_Type == 7) {
        CVector3__Subtract(&d, &e->pos, target);
        dist = d.x * d.x + d.z * d.z;
        d.y = 0;
        if (262144.0f < dist) {
            return 0;
        }
        return 0xFF;
    }
    if (e->def->m_Type == 1) {
        t = e->typeId;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0xFF;
        }
    }
    return 0;
}

/* Blast_IsLivingEnemyInRange inlined: a living enemy within 2560 units of `target`. */
static __inline__ s32 inBlastRange(Entity_00223A94 *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;

    CVector3__Subtract(&d, &e->pos, target);
    dist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (e->def->m_Type == 1) {
        t = e->typeId;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0;
        }
        if (e->health != 0) {
            if (e->state != 0x3D) {
                if (dist <= 6553600.0f) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* A blast at `target`: damages every entity of the world and of the actor list, then damages
 * the current player when within 512 units. */
void CScene__Blast(CScene *world, CVector3 *target) {
    DamageEvent event;
    CVector3 d;
    Entity_00223A94 *e;
    CListedInstance *node;
    Entity_00223A94 *actor;
    Entity_00223A94 *player;
    s32 count;
    s32 i;
    s32 strength;

    count = world->m_nKind1;
    for (i = 0; i < count; i++) {
        e = (Entity_00223A94 *)world->m_pKind1[i];
        strength = blastStrength(e, target);
        if (strength != 0) {
            DamageEvent_Set(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            CAI__CallHurt(e, e->ai, &event);
        }
        if (inBlastRange(e, target)) {
            CAI__SetMode(e, e->ai, 0x3D);
        }
    }
    for (node = gScene.m_InstancePool.m_ActiveList.m_pHead; node != NULL; node = node->m_pNext) {
        actor = (Entity_00223A94 *)&node->m_Instance;
        strength = blastStrength(actor, target);
        if (strength != 0) {
            DamageEvent_Set(&event, 0, strength << 8, 102.399994f, 0x400000, 0);
            CAI__CallHurt(actor, actor->ai, &event);
        }
        if (inBlastRange(actor, target)) {
            CAI__SetMode(actor, actor->ai, 0x3D);
        }
    }
    player = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    CVector3__Subtract(&d, &player->pos, target);
    if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
        DamageEvent_Set(&event, 0, 0x1900, 102.399994f, 0x400000, 0);
        CAI__CallHurt(player, player->ai, &event);
    }
}

#ifdef TARGET_PORT
void *CScene__GetSwoosh(struct CScene *, s32); /* host: returns the block (docs/port/ABI.md) */
#else
void CScene__GetSwoosh(struct CScene *);
#endif
#ifdef TARGET_PORT
#include "protos/iset.h" /* CISet__GetBlock returns a pointer: implicit int truncated it (--high-memory) */
/* ABI (docs/port/ABI.md, abicheck void_used + missing_args): the caller (Trail_AddNode) passes the
 * swoosh type in a1 and uses the block address; on MIPS a1 passes straight through to
 * CISet__GetBlock and its v0 is returned. */
void *CScene__GetSwoosh(CScene *arg0, s32 index) {
    return CISet__GetBlock(arg0->m_pSwooshes, index);
}
#else
void *CISet__GetBlock(void *, s32);

void CScene__GetSwoosh(CScene *arg0) {
    ((void (*)(sptr)) CISet__GetBlock)((sptr)arg0->m_pSwooshes); /* the ROM passes one (a1 goes through) */
}
#endif

void *CScene__GetLight(CScene *arg0, s32 arg1);

/* Element i of a table whose first word is the element size, data at +8. */
void *CScene__GetLight(CScene *arg0, s32 arg1) {
    s32 *t = arg0->m_pLights;
    return (s8 *)t + (arg1 * *t + 8);
}
void CScene__DrawRegisteredInstances(CScene *lv, sptr arg);
void Render_ResetBatches();

void CScene__DrawRegisteredInstances(CScene *lv, sptr arg) {
    Gfx *g;
    s32 i;
    s32 n;
    s32 c;
    CGameObjectInstance **arr;
    Render_ResetBatches();
    n = lv->m_nRegistered;
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 5;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 5;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFB;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFB;
    arr = (CGameObjectInstance **)lv->m_pRegistered;
    for (i = 0; i < n; i++) {
        CGameObjectInstance *e = arr[i];
        if (e->m_dwFlags & 4) {
            c = lv->m_nDeferred;
            if (c != 0x40) {
                lv->m_pDeferred[c] = e;
                lv->m_nDeferred = c + 1;
            }
        } else {
            CGameObjectInstance__Draw(e, arg);
        }
    }
    Render_FlushBatches();
}
void CScene__DrawParticles(u8 *arg0, sptr arg1);
#ifdef TARGET_PORT
#include "structs/world.h"
#endif

void CParticleSystem__Draw(void *, sptr);

void CScene__DrawParticles(u8 *arg0, sptr arg1) {
    Gfx *g;

    /* gSPClipRatio(FRUSTRATIO_1) */
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 1;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 1;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFF;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFF;
    Render_ResetBatches();
#ifdef TARGET_PORT
    CParticleSystem__Draw(&((CScene *) arg0)->m_ParticleSystem, arg1); /* N64 0x18B0 */
#else
    CParticleSystem__Draw(arg0 + 0x18B0, arg1);
#endif
    Render_FlushBatches();
}

s32 CScene__FindObjectType(CScene *lv, s32 id) {
    IdTable *t = lv->m_pObjectTypes;
    s32 n = t->count;
    u16 *p = t->ids;
    s32 i = 0;
    if (n > 0) {
        do {
            if (*p == (u16)id) {
                return i;
            }
            i++;
            p++;
        } while (i < n);
    }
    return -1;
}
u16 CScene__GetObjectTypeID(CScene *arg0, s32 arg1);

u16 CScene__GetObjectTypeID(CScene *arg0, s32 arg1) {
    return arg0->m_pObjectTypes->ids[arg1];
}
void CScene__SetInstanceObjectType(CScene *scene, CGameObjectInstance *obj, s32 type, sptr anim);
extern void CGameObjectInstance__SetupAnimation(CGameObjectInstance *, s32 *, s32, sptr);

/* CScene__FindObjectType, inlined: the index of id in the level's object type table. */
static __inline__ s32 find(IdTable *table, u16 id) {
    s32 index = 0, count;
    u16 *entry;
    entry = table->ids;
    count = table->count;
    if (count > 0) {
        do {
            if (*entry == id) return index;
            index++;
            entry++;
        } while (index < count);
    }
    return -1;
}

void CScene__SetInstanceObjectType(CScene *scene, CGameObjectInstance *obj, s32 type, sptr anim) {
    s32 index = 0;
    if (obj->m_nTypeFlag != type) {
        index = find(scene->m_pObjectTypes, type);
        if (index != -1) {
            obj->header.m_nObjType = index;
            obj->m_nTypeFlag = type;
            CGameObjectInstance__SetupAnimation(obj, scene->m_pObjects, scene->m_ObjectsKey, anim);
        }
    }
}
void *CScene__GetRegionInfo(CScene *arg0, u16 *arg1);
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));

void *CScene__GetRegionInfo(CScene *arg0, u16 *arg1) {
    sptr base;
    s32 idx;
    if (arg1 == 0) {
        return 0;
    }
    base = ((char *)CISet__GetBlock(arg0->m_pRegionData, 0)) + 8;
    idx = *arg1;
    return (void *)(base + idx * 0x64);
}

sptr CScene__GetRegion(struct CScene *, s32);
u32 CScene__GetRegionIndex(CScene *arg0, sptr arg1);
void CScene__SetLevelBit(void *, s32, s32);

u32 CScene__GetRegionIndex(CScene *arg0, sptr arg1) {
    sptr offset;
    sptr result;
    if (arg1 == 0) {
        return -1;
    }
    offset = ((char *)CISet__GetBlock(arg0->m_pRegionData, 2)) + 8;
    result = arg1 - offset;
    /* the host CGameRegion stride (sizeof(CGameRegion), three pointer pairs), as
     * CScene__GetRegion indexes it, not the N64's 0x20 bytes (>> 5). Round 9 fixed it twice:
     * x1: the index came out too big, Automap_RevealRegion's CScene__SetLevelBit set map bits
     * past the level's bit section into the next level's block of gGamePersistentData (the
     * warp 55 -> 54 crashed in CScene__DecompressCollision); x2: a cinema that saved the
     * player's region (Cinema_Start) restored him into another region or past the table (the
     * camera's region read floats and crashed after the Mother's phase-2 cinema). Unsigned,
     * like the N64's shift. */
    return (u32) ((uptr) result / sizeof(CGameRegion));
}

#ifdef TARGET_PORT
/* The port: the same through CScene (structs/world.h) and the host CGameRegion stride. */
#include "structs/region.h"
sptr CScene__GetRegion(CScene *arg0, s32 arg1) {
    return (sptr)((CGameRegion *)((u8 *)CISet__GetBlock(arg0->m_pRegionData, 2) + 8) + arg1);
}
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ s32 table;
} Arg0;


/* Entry arg1 (0x20 bytes each) of section 2 of the table at 0x58. */
#define arg0 ((Arg0 *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
sptr CScene__GetRegion(CScene *scene, s32 arg1) {
    { s32 base = CISet__GetBlock(arg0->table, 2) + 8; return base + (arg1 << 5); }
}
#undef arg0
#endif

void CScene__SetLevelBitInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void CScene__SetLevelBitInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
  s32 temp_a0;
  sptr temp_a1;
  sptr temp_v0;
  s32 var_a2;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  sptr new_var;
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x70)), 0), arg1), 0);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg2 & 7);
  if (arg3 != 0)
  {
    var_v1 = arg2;
    if (arg2 < 0)
    {
      var_v1 = arg2 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a2 = arg2;
  if (var_a2 < 0)
  {
    var_a2 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a2 >> 3);
  *temp_v0_2 &= ~temp_a0;
}

void func_00224414(void) {

}

#ifdef TARGET_PORT
/* The port: the level index and the game bit sections by member of CScene (the N64 offsets
 * 0x18FA8 / 0x70 read other members on the host). */
#include "structs/world.h"
void CScene__SetLevelBit(void *scene, s32 arg1, s32 arg2) {
    CScene *arg0 = scene;
    sptr bits;
    u8 *p;

    bits = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->m_pBitSections2, 0), arg0->m_nLevel), 0);
    p = (u8 *) CISet__GetBlock(bits, 1) + arg1 / 8;
    if (arg2 != 0) {
        *p |= 1 << (arg1 & 7);
    } else {
        *p &= ~(1 << (arg1 & 7));
    }
}
#else
void CScene__SetLevelBit(void *arg0, s32 arg1, s32 arg2)
{
  s32 temp_a0;
  sptr new_var;
  sptr temp_a1;
  s32 temp_s0;
  sptr temp_v0;
  s32 var_a1;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  temp_s0 = *((s32 *) (((s8 *) arg0) + 0x18FA8));
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x70)), 0), temp_s0), 0);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg1 & 7);
  if (arg2 != 0)
  {
    var_v1 = arg1;
    if (arg1 < 0)
    {
      var_v1 = arg1 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a1 = arg1;
  if (var_a1 < 0)
  {
    var_a1 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a1 >> 3);
  *temp_v0_2 &= ~temp_a0;
}
#endif

/* Tests a bit of the current level's second saved bitmap. */
s32 CScene__GetLevelBit(CScene *arg0, s32 arg1) {
    s32 *temp_v0;
    s32 temp_s0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg1;
    temp_s0 = arg0->m_nLevel;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->m_pBitSections2, 0), temp_s0), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}
void CScene__SetInstanceSavedBitInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3);

/* Turok 2 level actor: 0x288 bytes (0x2E8 in Rage Wars). */


void CScene__SetInstanceSavedBitInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *entry;
    CIntelligence *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 mask;
    s32 idx;
    s32 idx2;
    if (((CScene *)arg0)->m_Mode == 2) {
        return;
    }
    if (((CScene *)arg0)->m_nLevel == arg1) {
        entry = (char *)((CScene *)arg0)->m_pActors + (s32) arg2 * (s32)sizeof(CGameObjectInstance);
        cond = ((CGameObjectInstance *)entry)->header.m_pIntelligence;
        if ((u32) (cond->m_Type - 9) < 2) {
            if (cond->m_dwFlags & 0x200) {
                return;
            }
        }
    }
    field80 = ((CScene *)arg0)->m_pBitSections;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(field80, 0), (s32) arg1), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    base1 = (u8 *) CISet__GetBlock(temp_v0, 1);
    base2 = base1;
    mask = 1 << (arg2 & 7);
    if (arg3 != 0) {
        idx = arg2;
        if ((s32) arg2 < 0) {
            idx = arg2 + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = arg2;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}
void CScene__SetLevelActorKilledInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3);

void CScene__SetLevelActorKilledInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
  sptr new_var;
  s32 temp_a0;
  sptr temp_a1;
  sptr temp_v0;
  s32 var_a2;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
#ifdef TARGET_PORT
  /* the port: the scene's bit sections at their host offset (CScene.m_pBitSections) */
  temp_v0 = (sptr) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(((CScene *) arg0)->m_pBitSections, 0), arg1), 1);
#else
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x6C)), 0), arg1), 1);
#endif
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg2 & 7);
  if (arg3 != 0)
  {
    var_v1 = arg2;
    if (arg2 < 0)
    {
      var_v1 = arg2 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a2 = arg2;
  if (var_a2 < 0)
  {
    var_a2 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a2 >> 3);
  *temp_v0_2 &= ~temp_a0;
}

#ifdef TARGET_PORT
/* The port: the view read the scene's bit sections and level at the N64 offsets 0x6C/0x18FA8 and the
 * actor's killed index at 0x16E; typed on CScene and CLevelActor at their host offsets. Collecting a
 * level pickup (CPlayer__TouchPickup) crashed here. */
#include "common.h"
#include "structs/world.h"
#include "structs/level_actor.h"
#include "protos/iset.h"
void CScene__SetLevelActorKilled(CScene *o, CLevelActor *e, s32 set) {
    s32 bit = e->m_nKilledIndex;
    s32 idx = o->m_nLevel;
    sptr n = (sptr) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(o->m_pBitSections, 0), idx), 1);
    u8 *bits;
    s32 m;

    CISet__GetBlock((void *) n, 0);
    CISet__GetBlockSize((s32 *) n, 1);
    bits = (u8 *) CISet__GetBlock((void *) n, 1);
    m = 1 << (bit & 7);
    if (set != 0) {
        bits[bit / 8] |= m;
    } else {
        bits[bit / 8] &= ~m;
    }
}
#else
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B874.c, func_0028B874.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "protos/iset.h"
typedef struct { u8 pad0[0x16E]; u16 f16E; } P4EntB;
typedef struct { u8 pad0[0x6C]; s32 f6C; } P4OwnB;
void CScene__SetLevelActorKilled(P4OwnB *o, P4EntB *e, s32 set) {
    s32 bit = e->f16E;
    s32 idx = *(s32 *)((u8 *)o + 0x18FA8);
    sptr n = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(o->f6C, 0), idx), 1);
    CISet__GetBlock(n, 0);
    CISet__GetBlockSize(n, 1);
    {
        u8 *bits = (u8 *)CISet__GetBlock(n, 1);
        u8 *p2 = bits;
        s32 m = 1 << (bit & 7);
        if (set != 0) {
            bits[bit / 8] |= m;
        } else {
            p2[bit / 8] &= ~m;
        }
    }
}
#endif
#ifdef TARGET_PORT
/* the port: the actor's killed index at its host offset (CLevelActor.m_nKilledIndex) */
#include "structs/level_actor.h"
typedef struct { u8 pad0[__builtin_offsetof(CLevelActor, m_nKilledIndex)]; u16 unk16E; } Src;
#else
typedef struct { u8 pad0[0x16E]; u16 unk16E; } Src;
#endif

s32 CScene__IsLevelActorKilled(CScene *lv, Src *s) {
    s32 bit = s->unk16E;
    s32 idx = lv->m_nLevel;
    s32 *t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(lv->m_pBitSections, 0), idx), 1);
    u8 *bits;
    s32 m;
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

void CScene__SetEventFiredInLevel(void *, s32, s32, s32);
#ifdef TARGET_PORT
/* The port (round 13 b2): the bit sections by member of CScene (the N64 offset 0x6C is another
 * member of the host CScene). */
#include "structs/world.h"
void CScene__SetEventFiredInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    CScene *w = arg0;
    u8 *p = (u8 *) CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(w->m_pBitSections, 0), arg1), 2), 1) + arg2 / 8;
    if (arg3 != 0) {
        *p |= 1 << (arg2 & 7);
    } else {
        *p &= ~(1 << (arg2 & 7));
    }
}
#else
void CScene__SetEventFiredInLevel(void *arg0, s32 arg1, s32 arg2, s32 arg3)
{
  s32 temp_a0;
  sptr temp_a1;
  sptr temp_v0;
  sptr new_var;
  s32 var_a2;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*((s32 *) (((s8 *) arg0) + 0x6C)), 0), arg1), 2);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg2 & 7);
  if (arg3 != 0)
  {
    var_v1 = arg2;
    if (arg2 < 0)
    {
      var_v1 = arg2 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  var_a2 = arg2;
  if (var_a2 < 0)
  {
    var_a2 += 7;
  }
  temp_v0_2 = temp_a1 + (var_a2 >> 3);
  *temp_v0_2 &= ~temp_a0;
}
#endif

typedef struct { u8 pad0[0x13]; u8 unk13; } Src_00224A68;

void CScene__SetEventFired(CScene *lv, Src_00224A68 *s, s32 set) {
    s32 bit = s->unk13;
    s32 idx = lv->m_nLevel;
    s32 *t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(lv->m_pBitSections, 0), idx), 2);
    u8 *bits;
    u8 *b2;
    s32 m;
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    b2 = bits;
    m = 1 << (bit & 7);
    if (set != 0) {
        bits[bit / 8] |= m;
    } else {
        b2[bit / 8] &= ~m;
    }
}

typedef struct { u8 pad0[0x13]; u8 unk13; } Src_00224B58;

s32 CScene__IsEventFired(CScene *lv, Src_00224B58 *s) {
    s32 bit = s->unk13;
    s32 idx = lv->m_nLevel;
    s32 *t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(lv->m_pBitSections, 0), idx), 2);
    u8 *bits;
    s32 m;
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}
s32 CScene__GetLinkGroupCount(CScene *lv, s32 key);

s32 CScene__GetLinkGroupCount(CScene *lv, s32 key) {
    s32 *t = lv->m_pLinkGroupTypes;
    s32 n = t[1];
    s32 *ents = t + 2;
    s32 i;
    for (i = 0; i < n; i++) {
        if (ents[i] == key) {
            s32 *u = CISet__GetBlock(lv->m_pBitSections, 1);
            CISet__GetBlock(u, 0);
            return *((u8 *)CISet__GetBlock(u, 1) + i);
        }
    }
    return -1;
}
void CScene__DecLinkGroupCount(CScene *arg0, s32 arg1);

void CScene__DecLinkGroupCount(CScene *arg0, s32 arg1) {
    sptr temp_v0;
    u8 *temp_v0_2;

    temp_v0 = CISet__GetBlock(arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    temp_v0_2 = CISet__GetBlock(temp_v0, 1) + arg1;
    *temp_v0_2 -= 1;
}
s32 CScene__IsLinkGroupDone(void *arg0, s32 arg1);
struct Field_void_6C;
struct Field_void_6C {
    char pad[0x6C];
    void * value;
};
typedef struct Field_void_6C Field_void_6C;

s32 CScene__IsLinkGroupDone(void *arg0, s32 arg1) {
    void *temp_v0;
    u8 *result;
#ifdef TARGET_PORT /* m_pBitSections by member (host CScene layout) */
    temp_v0 = CISet__GetBlock(((CScene *) arg0)->m_pBitSections, 1);
#else
    temp_v0 = CISet__GetBlock((((struct Field_void_6C *) ((s8 *) arg0))->value), 1);
#endif
    CISet__GetBlock(temp_v0, 0);
    result = (u8 *)((char *)CISet__GetBlock(temp_v0, 1)) + arg1;
    return *result == 0;
}

CPooledLevelActor *CLevelActorPool__Spawn(CLevelActorPool *pool, s32 *refCount, s32 a, CVector3 z, CVector3 v, sptr b, f32 f);

DEFERRED void CScene__SpawnPoolActor(CScene *lv, s32 a1, CVector3 v, s32 x, f32 f) {
    CVector3 z;
    z.x = 0;
    z.y = 0;
    z.z = 0;
    CLevelActorPool__Spawn(&lv->m_ActorPool, NULL, x, z, v, a1, f);
}

DEFERRED void CScene__GetTextureSet(CScene *arg0, s32 arg1, s32 *arg2, sptr arg3) {
    *arg2 = CISet__GetROMAddressAndSize(arg0->m_pTextureSets, arg0->m_TextureSetsKey, arg1, (s32 *)arg3);
}

DEFERRED s32 CScene__GetObject(CScene *arg0, s32 arg1) {
    s32 *table;

    table = arg0->m_pObjects;
    if (arg1 < *table) {
        return CISet__GetROMAddress((sptr)table, arg0->m_ObjectsKey, arg1);
    }
    return 0;
}

DEFERRED void CScene__GetRegionEventRecords(CScene *lv, u16 *idp, CSceneRecord **outRecs, s32 *outCount) {
    s32 hi;
    s32 lo;
    s32 r; sptr a; s32 b; s32 id;
    id = *idp; a = (sptr) lv->m_pAltRecordIndex; b = lv->m_nAltRecords;
    r = BinaryRange(a, b, id, &lo, &hi);
    if (r != 0) {
        *outRecs = lv->m_pAltRecords + lo;
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}

/* Index of actor in the world's actor array, or -1 (same as CScene__GetInstanceIndex). */
static __inline__ s32 World_ActorIndex(CScene *world, CGameObjectInstance *actor) {
    CGameObjectInstance *base;
    if (actor->m_dwFlags & 0x80000) {
        return -1;
    }
    base = (CGameObjectInstance *)world->m_pActors;
    if (actor >= base &&
        (char *)actor <= (char *)base + ((u32)world->m_nActors * sizeof(CGameObjectInstance) - sizeof(CGameObjectInstance))) {
        return ((uptr)actor - (uptr)base) / sizeof(CGameObjectInstance);
    }
    return -1;
}

/* Look up the run of records belonging to actor arg1: *arg2 = first record,
 * *arg3 = record count (0 if none). */
DEFERRED void CScene__GetInstanceEventRecords(CScene *arg0, CGameObjectInstance *arg1, CSceneRecord **arg2, s32 *arg3) {
    s32 first;
    s32 last;
    s32 *recordIndex;
    s32 recordCount;
    s32 index = World_ActorIndex(arg0, arg1);
    if (index == -1) {
        *arg3 = 0;
        return;
    }
    recordIndex = arg0->m_pRecordIndex;
    recordCount = arg0->m_nRecords;
    if (BinaryRange(recordIndex, recordCount, index, &first, &last) != 0) {
        *arg2 = &arg0->m_pRecords[first];
        *arg3 = (last - first) + 1;
    } else {
        *arg2 = 0;
        *arg3 = 0;
    }
}

s32 LevelEvent_Trigger(sptr);

DEFERRED void CScene__FireEventsByKey(void *arg0, s32 arg1)
{
  s32 var_s0;
  s32 var_s3;
  s32 var_s4;
  void *var_s2;
  void *var_s1;
  var_s2 = arg0;
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s0 = ((CScene *) var_s2)->m_nRecords;
#else
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x1168));
#endif
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s1 = ((CScene *) var_s2)->m_pRecords;
#else
  var_s1 = *((void **) (((s8 *) var_s2) + 0x1178));
#endif
  var_s0 -= 1;
  var_s3 = arg1;
  if (var_s0 != (-1))
  {
    var_s4 = -1;
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        LevelEvent_Trigger((sptr) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s0 = ((CScene *) var_s2)->m_nAltRecords;
#else
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x116C));
#endif
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s1 = ((CScene *) var_s2)->m_pAltRecords;
#else
  var_s1 = *((void **) (((s8 *) var_s2) + 0x117C));
#endif
  var_s0 -= 1;
  if (var_s0 != (-1))
  {
    var_s2 = (void *) (-1);
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        LevelEvent_Trigger((sptr) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
}

DEFERRED s32 CScene__AreEventsDone(char *object, s32 type) {
    s32 index;
    CSceneRecord *record;
    u8 *flags;
    index = ((CScene *)object)->m_nRecords;
    record = ((CScene *)object)->m_pRecords;
    index--;
    if (index != -1) {
        flags = &record->m_Flags;
        do {
            if (flags[1] == type &&
                (*flags & 0x40) == 0) {
                return 0;
            }
            index--;
            flags += sizeof(CSceneRecord);
        } while (index != -1);
    }
    index = ((CScene *)object)->m_nAltRecords;
    record = ((CScene *)object)->m_pAltRecords;
    index--;
    if (index != -1) {
        do {
            if (record->m_Key == type &&
                (record->m_Flags & 0x40) == 0) {
                return 0;
            }
            index--;
            record++;
        } while (index != -1);
    }
    return 1;
}

s32 LevelEvent_MarkFired(sptr);

DEFERRED void CScene__FireEventsByKeyOnce(void *arg0, s32 arg1)
{
  s32 var_s0;
  s32 var_s3;
  s32 var_s4;
  int new_var;
  void *var_s1;
  void *var_s2;
  var_s2 = arg0;
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s0 = ((CScene *) var_s2)->m_nRecords;
#else
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x1168));
#endif
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s1 = ((CScene *) var_s2)->m_pRecords;
#else
  var_s1 = *((void **) (((s8 *) var_s2) + 0x1178));
#endif
  var_s0 -= 1;
  var_s3 = arg1;
  if (var_s0 != (-1))
  {
    var_s4 = -1;
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        LevelEvent_MarkFired((sptr) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s0 = ((CScene *) var_s2)->m_nAltRecords;
#else
  var_s0 = *((s32 *) (((s8 *) var_s2) + 0x116C));
#endif
#ifdef TARGET_PORT /* the record tables by member (host CScene layout) */
  var_s1 = ((CScene *) var_s2)->m_pAltRecords;
#else
  var_s1 = *((void **) (((s8 *) var_s2) + 0x117C));
#endif
  var_s0 -= 1;
  new_var = -1;
  var_s2 = (void *) (-1);
  if (var_s0 != new_var)
  {
    do
    {
      if ((*((u8 *) (((s8 *) var_s1) + 0xF))) == var_s3)
      {
        LevelEvent_MarkFired((sptr) var_s1);
      }
      var_s0 -= 1;
      var_s1 += 0x14;
    }
    while (var_s0 != (-1));
  }
}

#ifdef TARGET_PORT
/* The port: the scene's delayed events typed (structs/world.h); the views below have the N64
 * offsets. */
#include "structs/world.h"

void LevelEvent_Fire(void *event);
f32 HalfToFloat(u16);
DEFERRED void CScene__QueueDelayedEvent(CScene *scene, LevelEvent *event) {
    CDelayedEvent *d = scene->m_FreeDelayedEvents.m_pHead;
    f32 t;

    if (d != NULL) {
        CList__Remove(&scene->m_FreeDelayedEvents, d);
        CList__AddHead(&scene->m_DelayedEvents, d);
        d->m_pEvent = event;
    } else {
        d = scene->m_DelayedEvents.m_pTail;
        LevelEvent_Fire((sptr)d->m_pEvent);
        d->m_pEvent = event;
    }
    t = HalfToFloat(event->delay);
    d->m_Time = t;
    if (event->flags & 2) {
        d->m_Time = t * 15.0f;
    }
}
#else
typedef struct {
    /* 0x00 */ u8 links[8];
    /* 0x08 */ s32 def;
    /* 0x0C */ f32 length;
} Voice_00225354;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1180];
    /* 0x1180 */ CList activeVoices;
    /* 0x1194 */ CList freeVoices;
} VoiceOwner;

void LevelEvent_Fire(void *event);

f32 HalfToFloat(u16);                             /* extern */

#define arg0 ((VoiceOwner *) scene)
/* the definition takes the port body's types (one prototype for the merged object); the view reads them */
DEFERRED void CScene__QueueDelayedEvent(CScene *scene, LevelEvent *arg1) {
    f32 temp_f0;
    Voice_00225354 *var_s0;

    var_s0 = arg0->freeVoices.m_pHead;
    if (var_s0 != 0) {
        CList__Remove(&arg0->freeVoices, var_s0);
        CList__AddHead(&arg0->activeVoices, var_s0);
        var_s0->def = (sptr) arg1;
    } else {
        var_s0 = arg0->activeVoices.m_pTail;
        LevelEvent_Fire(var_s0->def);
        var_s0->def = (sptr) arg1;
    }
    temp_f0 = HalfToFloat(arg1->delay);
    var_s0->length = temp_f0;
    if (arg1->flags & 2) {
        var_s0->length = (f32) (temp_f0 * 15.0f);
    }
}
#undef arg0
#endif
#ifdef TARGET_PORT
/* The port: the scene's delayed events typed (structs/world.h). */
#include "structs/event.h"

void LevelEvent_Fire(void *event);
DEFERRED void CScene__UpdateDelayedEvents(CScene *scene) {
    CDelayedEvent *d = scene->m_DelayedEvents.m_pHead;
    CDelayedEvent *next;
    s32 fire;

    while (d != NULL) {
        LevelEvent *event = d->m_pEvent;
        next = d->m_pNext;
        fire = 0;
        if (event->flags & 2) {
            d->m_Time -= gFrameIncrement;
            if (d->m_Time <= 0.0f) {
                fire = 1;
            }
        }
        if (fire) {
            LevelEvent_Fire((sptr)event);
            CList__Remove(&scene->m_DelayedEvents, d);
            CList__AddHead(&scene->m_FreeDelayedEvents, d);
        }
        d = next;
    }
}
#else
void LevelEvent_Fire(void *event);


/* A timed list entry: counts down and is freed at zero. */
typedef struct TimedNode {
    /* 0x00 */ struct TimedNode *prev;
    /* 0x04 */ struct TimedNode *next;
    /* 0x08 */ u8 *item;               /* item[0xE] & 2: timed */
    /* 0x0C */ f32 timer;
} TimedNode;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1180];
    /* 0x1180 */ TimedNode *activeHead;  /* CList at 0x1180 */
    /* 0x1184 */ u8 pad1184[0x1194 - 0x1184];
    /* 0x1194 */ TimedNode *freeHead;    /* CList at 0x1194 */
} TimedPool;

#define arg0 ((TimedPool *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the view reads it */
DEFERRED void CScene__UpdateDelayedEvents(CScene *scene) {
    f32 temp_f1;
    s32 var_s1;
    s32 var_v1;
    u8 *temp_a0;
    TimedNode *temp_s2;
    TimedNode *var_s0;

    var_s0 = arg0->activeHead;
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            temp_a0 = var_s0->item;
            temp_s2 = var_s0->next;
            var_v1 = var_s1;
            if (temp_a0[0xE] & 2) {
                temp_f1 = var_s0->timer - gFrameIncrement;
                var_s0->timer = temp_f1;
                if (temp_f1 <= 0.0f) {
                    var_v1 = 1;
                    var_s1 = 1;
                }
            }
            if (var_v1 != 0) {
                LevelEvent_Fire((sptr) temp_a0);
            }
            if (var_s1 != 0) {
                CList__Remove(&arg0->activeHead, var_s0);
                CList__AddHead(&arg0->freeHead, var_s0);
            }
            var_s0 = temp_s2;
            var_s1 = 0;
        } while (var_s0 != NULL);
    }
}
#undef arg0
#endif

/* A separate function in Rage Wars; inlined into this one in Turok 2. */
static inline s32 hasTbl(CScene *lv) {
    s32 i;
    for (i = 0; i < lv->m_nSectors; i++) {
        if (((s32 *) CISet__GetBlock(*lv->m_Sectors[i].m_hData, 2))[1] != 0) {
            return 1;
        }
    }
    return 0;
}

DEFERRED void CScene__GetLightsAt(CScene *lv, CVector3 *a1, CLights1 *out) {
    u8 buf[10 * sizeof(void *)]; /* CScene__FindLightsAt stores up to 10 record pointers:
                                  * 0x28 bytes overran into the caller's frame on LP64 */
    s32 n;
    if (hasTbl(lv) == 0 || gLevelLightsEnabled == 0) {
        *out = gSceneLights;
        return;
    }
    CScene__FindLightsAt(lv, a1, buf, &n);
    CScene__ComputeLights(buf, n, a1, out);
}

/* Files a new actor into every actor list of the world it belongs to. */
DEFERRED void CScene__FileInstance(CScene *arg0, CGameObjectInstance *arg1) {
    s32 temp_a0;
    s32 temp_a0_2;
    s32 temp_a0_3;
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 temp_v1_4;
    s32 temp_v1_5;

    CGameObjectInstance__ClearDrawMatrices((sptr) arg1);
    if (GameObject_IsSolid((sptr) arg1) != 0) {
        temp_v1 = arg0->m_nDrawActors;
        if (temp_v1 != 0x200) {
            arg0->m_pDrawActors[temp_v1] = (CGameObjectInstance *)arg1;
            arg0->m_nDrawActors = (s32) (temp_v1 + 1);
        }
    }
    if (GameObject_IsMover((sptr) arg1) != 0) {
        temp_v1_2 = arg0->m_nMovers;
        if (temp_v1_2 != 0x80) {
            arg0->m_pMovers[temp_v1_2] = (CGameObjectInstance *)arg1;
            arg0->m_nMovers = (s32) (temp_v1_2 + 1);
        }
    }
    temp_v1_3 = arg0->m_nRegistered;
    if (temp_v1_3 < 0x80) {
        arg0->m_pRegistered[temp_v1_3] = (CGameObjectInstance *)arg1;
        arg0->m_nRegistered = (s32) (temp_v1_3 + 1);
    }
    temp_a0 = arg0->m_nKind1;
    if ((temp_a0 < 0x40) && (arg1->header.m_pIntelligence->m_Type == 1)) {
        arg0->m_pKind1[temp_a0] = (CGameObjectInstance *)arg1;
        temp_v0 = arg0->m_nKind1;
        arg1->m_AI.m_PathFindCount = (s8) temp_v0;
        arg0->m_nKind1 = (s32) (temp_v0 + 1);
    }
    temp_v1_4 = arg0->m_nFlagged;
    if ((temp_v1_4 < 0x20) && (arg1->m_dwFlags & 2)) {
        arg0->m_pFlagged[temp_v1_4] = (CGameObjectInstance *)arg1;
        arg0->m_nFlagged = (s32) (temp_v1_4 + 1);
    }
    temp_a0_2 = arg0->m_nKind5;
    if ((temp_a0_2 < 0x10) && (arg1->header.m_pIntelligence->m_Type == 5)) {
        arg0->m_pKind5[temp_a0_2] = (CGameObjectInstance *)arg1;
        arg0->m_nKind5 = (s32) (temp_a0_2 + 1);
    }
    temp_a0_3 = arg0->m_nList6;
    if (temp_a0_3 < 0x20) {
        if (arg1->m_nTypeFlag == 0x64F) {
            arg0->m_pList6[temp_a0_3] = (CGameObjectInstance *)arg1;
            arg0->m_nList6 = (s32) (temp_a0_3 + 1);
        }
    }
    if (arg1->m_nTypeFlag == 0x64D) {
        temp_v1_5 = arg0->m_nList7;
        if (temp_v1_5 < 4) {
            arg0->m_pList7[temp_v1_5] = (CGameObjectInstance *)arg1;
            arg0->m_nList7 = (s32) (temp_v1_5 + 1);
        }
    }
}

/* Turok 2 LevelActor_00225778: 0x288 bytes (Rage Wars 0x2E8). */

/* Nearest other actor matching team/kind (-1 = any), optionally alive only. */
DEFERRED CGameObjectInstance *CScene__FindNearestActor(CScene *world, CGameObjectInstance *self, s32 team, s32 kind, s32 activeOnly) {
    s32 i;
    CGameObjectInstance *obj;
    CGameObjectInstance *best;
    f32 min;
    f32 dx, dy, dz, distSq;
    s32 count;
    i = 0;
    min = 3.4028235e38f;
    count = world->m_nActors;
    best = 0;
    for (; i < count; i++) {
        obj = (CGameObjectInstance *)world->m_pActors + i;
        if (obj == self) {
            continue;
        }
        if (team != -1 && obj->m_nTypeFlag != team) {
            continue;
        }
        if (kind != -1 && obj->header.m_pIntelligence->m_Type != kind) {
            continue;
        }
        if (activeOnly && obj->m_AI.m_Health == 0) {
            continue;
        }
        dx = self->header.m_vPos.x - obj->header.m_vPos.x;
        dy = self->header.m_vPos.y - obj->header.m_vPos.y;
        dz = self->header.m_vPos.z - obj->header.m_vPos.z;
        distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < min) {
            min = distSq;
            best = obj;
        }
    }
    return best;
}

s32 Throwable_GetModeAnim(CGameObjectInstance *actor, void *ai);

/* The actor of kind 5 in the world list nearest to target (squared distance) for which
 * Throwable_GetModeAnim returns 0, or NULL. */
DEFERRED CGameObjectInstance *CScene__FindNearestIdleThrowable(CScene *world, CGameObjectInstance *target) {
    CGameObjectInstance **actors;
    CGameObjectInstance *actor;
    CGameObjectInstance *nearest;
    f32 best;
    f32 dx, dy, dz, dist;
    s32 count;
    s32 i;

    i = 0;
    nearest = NULL;
    count = world->m_nKind5;
    best = 3.4028235e38f;
    actors = (CGameObjectInstance **)world->m_pKind5;
    for (; i < count; i++) {
        actor = actors[i];
        if (actor->header.m_pIntelligence->m_Type != 5) {
            continue;
        }
        if (Throwable_GetModeAnim(actor, ((u8 *)&actor->m_AI)) != 0) {
            continue;
        }
        dx = target->header.m_vPos.x - actor->header.m_vPos.x;
        dy = target->header.m_vPos.y - actor->header.m_vPos.y;
        dz = target->header.m_vPos.z - actor->header.m_vPos.z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < best) {
            best = dist;
            nearest = actor;
        }
    }
    return nearest;
}

#ifdef TARGET_PORT
/* host body (round 10 y2): the ROM reads gScene's m_pList6/m_nList6 at their N64 offsets (0xFCC,
 * 0x104C) with 4-byte slots; on the host the list sits elsewhere and holds 8-byte pointers: the
 * Blind One's group actors (cover flags) took garbage as a cover target and crashed in
 * AI_FindBestTarget. The nearest actor of the list by squared distance, as the ROM does. */
#include "common.h"
#include "structs/game_object.h"
#include "structs/world.h"

DEFERRED void *CScene__FindNearestCover(void *arg0, void *arg1) {
    CScene *scene = arg0;
    CGameObjectInstance *self = arg1;
    CGameObjectInstance *best = NULL;
    f32 bestD = 3.4028235e38f;
    s32 i;

    for (i = 0; i < scene->m_nList6; i++) {
        CGameObjectInstance *a = scene->m_pList6[i];
        f32 dx = self->header.m_vPos.x - a->header.m_vPos.x;
        f32 dy = self->header.m_vPos.y - a->header.m_vPos.y;
        f32 dz = self->header.m_vPos.z - a->header.m_vPos.z;
        f32 d = dx * dx + dy * dy + dz * dz;
        if (d < bestD) {
            bestD = d;
            best = a;
        }
    }
    return best;
}
#else
#include "common.h"
DEFERRED void *CScene__FindNearestCover(void *arg0, void *arg1)
{
  f32 temp_f0;
  f32 temp_f0_2;
  f32 temp_f1;
  f32 temp_f2;
  f32 var_f3;
  s32 temp_a3;
  s32 var_v1;
  void **var_a0;
  void *temp_v0;
  void *var_a2;
 do { var_v1 = 0; var_a2 = 0; } while (0);
  temp_a3 = *((s32 *) (((s8 *) arg0) + 0x104C));
  var_f3 = 3.4028235e38f;
  var_a0 = arg0 + 0xFCC;
  if (temp_a3 > 0)
  {
    do
    {
      temp_v0 = *var_a0;
      temp_f0 = (*((f32 *) (((s8 *) arg1) + 4))) - (*((f32 *) (((s8 *) temp_v0) + 4)));
      temp_f2 = (*((f32 *) (((s8 *) arg1) + 8))) - (*((f32 *) (((s8 *) temp_v0) + 8)));
      temp_f1 = (*((f32 *) (((s8 *) arg1) + 0xC))) - (*((f32 *) (((s8 *) temp_v0) + 0xC)));
      temp_f0_2 = ((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) + (temp_f1 * temp_f1);
      var_v1 += 1;
      if (temp_f0_2 < var_f3)
      {
        var_f3 = temp_f0_2;
        var_a2 = temp_v0;
      }
      var_a0 = (typeof(var_a0)) ((s8 *) ((typeof(var_a0)) (((s8 *) var_a0) + 4)));
    }
    while (var_v1 < temp_a3);
  }
  return var_a2;
}
#endif

/* The actor of kind 5 in the second world list (0x1050) nearest to target (squared distance) for which
 * Throwable_GetModeAnim returns 0, or NULL. */
DEFERRED CGameObjectInstance *CScene__FindNearestIdleThrowableB(CScene *world, CGameObjectInstance *target) {
    CGameObjectInstance **actors;
    CGameObjectInstance *actor;
    CGameObjectInstance *nearest;
    f32 best;
    f32 dx, dy, dz, dist;
    s32 count;
    s32 i;

    i = 0;
    nearest = NULL;
    count = world->m_nList7;
    best = 3.4028235e38f;
    actors = (CGameObjectInstance **)world->m_pList7;
    for (; i < count; i++) {
        actor = actors[i];
        if (actor->header.m_pIntelligence->m_Type != 5) {
            continue;
        }
        if (Throwable_GetModeAnim(actor, ((u8 *)&actor->m_AI)) != 0) {
            continue;
        }
        dx = target->header.m_vPos.x - actor->header.m_vPos.x;
        dy = target->header.m_vPos.y - actor->header.m_vPos.y;
        dz = target->header.m_vPos.z - actor->header.m_vPos.z;
        dist = dx * dx + dy * dy + dz * dz;
        if (dist < best) {
            best = dist;
            nearest = actor;
        }
    }
    return nearest;
}

/* Detaches the first actor linked to `id` and relinks every other actor
 * linked to `id` onto it; returns the detached actor. */
DEFERRED ActorRec *CScene__ReplaceLeader(ActorList *obj, ActorRec *id) {
    int i;
    ActorRec *found;
    int count;
    ActorRec **actors;

    i = 0;
    found = 0;
    count = obj->count;
    actors = obj->actors;
    for (; i < count; i++) {
        if (actors[i]->link == id) {
            found = actors[i];
            found->link = 0;
            break;
        }
    }
    for (i = 0; i < count; i++) {
        if (actors[i]->link == id) {
            actors[i]->link = found;
        }
    }
    return found;
}

/* Clears m_AI.m_pParent on every actor whose parent is arg1. */
DEFERRED void CScene__DetachChildren(ActorTable *arg0, s32 arg1) {
    s32 i;
    s32 n;
    CGameObjectInstance **p;

    i = 0;
    n = arg0->count;
    p = arg0->actors;
    if (n > 0) {
        do {
            if ((sptr) (*p)->m_AI.m_pParent == arg1) {
                (*p)->m_AI.m_pParent = NULL;
            }
            i++;
            p++;
        } while (i < n);
    }
}

struct LevelActor_00225B7C;
struct Box;
typedef struct Box Box;
struct Box {
    CVector3 min;
    CVector3 max;
};
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
struct Shield;
struct Shield {
    char pad0[0x68];
    f32 factor;
};
typedef struct Shield Shield;
typedef struct LevelActor_00225B7C LevelActor_00225B7C;
struct LevelActor_00225B7C {
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    s16 modelIndex;
    u16 pad6;
    CVector3 position0;
    s32 model;
    CIntelligence *definition;
    CVector3 position1;
    CVector3 motionAngles;
    void *attached;
    s32 motionState;
    s32 motionIndex;
    f32 floorHeight;
    CVector3 floorDirection;
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    Box bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    /* no lighting[2] in Turok 2: behaviorFlags is at 0x140 */
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};
typedef struct Shared_ReactionActor Shared_ReactionActor;
typedef struct Shared_ReactionPlayer Shared_ReactionPlayer;
typedef struct Shared_ReactionObjectKind Shared_ReactionObjectKind;
typedef struct Shared_ReactionStatistics Shared_ReactionStatistics;
typedef struct Shared_ReactionPlayerRecord Shared_ReactionPlayerRecord;
typedef struct Shared_DamageEvent {
    Shared_ReactionActor *target;
    s32 damage;
    f32 radius;
    s32 flags;
    Shield *shield;
} Shared_DamageEvent;
struct Shared_ReactionObjectKind {
    u8 unk0;
    char pad0[0x3];
    u16 unk4;
};
struct Shared_ReactionActor {
    union {
        struct {
            u8 category;
            char pad1[3];
            u16 type;
            char pad6[2];
        };
        Shared_ReactionObjectKind objectKind;
    };
    CVector3 position;
    char pad14[0x58];
    f32 unk6C;
    f32 height;
    char pad74[0x70];
    u16 team;
    char padE6[0x1A];
    s32 flags;
    char pad104[0x28];
    Shared_ReactionActor *owner;
    char pad130[0x40];
    s32 unk170;
    s32 unk174;
    char pad178[0x60];
    Shared_ReactionPlayer *unk1D8;
};
struct Shared_ReactionPlayer {
    u8 unk0;
    char pad0[0x7];
    union {
        CVector3 position;
        struct { f32 unk8, unkC, unk10; };
    };
    u16* unk14;
    char pad14[0xCC];
    u16 unkE4;
    char padE4[0x1A];
    s32 unk100;
    char pad104[0x6C];
    s32 unk170;
    s32 unk174;
    char pad178[0x60];
    Shared_ReactionPlayer *unk1D8;
    char pad1DC[0x10C];
    Shared_ReactionActor actor2E8;
    char pad4C4[0x110];
    s32 unk5D4;
    Shared_ReactionStatistics * unk5D8;
    void *owner;
    char pad5E0[0x4];
    s32 unk5E4;
    char pad5E8[0x46];
    s16 unk62E;
    char pad630[0x20];
    s16 unk650;
    char pad652[0xB86];
    f32 unk11D8;
    char pad11DC[4];
    f32 unk11E0;
    char pad11E4[0x34];
    s32 unk1218;
    char pad121C[0x10];
    s32 unk122C;
    char pad1230[0x90];
    f32 reserve;
    Shared_ReactionPlayer *unk12C4;
    Shared_ReactionPlayer *unk12C8;
    Shared_ReactionPlayer *unk12CC[8];
    Shared_ReactionPlayer *unk12EC;
    s32 unk12F0;
    s32 unk12F4[8];
    char pad1314[0x24];
    s32 unk1338;
    char pad1338[0x88];
    s8 unk13C4;
    char pad13C5[0x23];
    s32 messageNext;
    char messages[4][25];
    s32 unk1450;
    Shared_ReactionPlayerRecord * unk1454;
};
struct Shared_ReactionStatistics {
    u16 deaths_self;
    u16 deaths_other;
    u16 score;
    u16 special_kills;
    u16 pad8;
    u16 special_hits;
    u16 kills_special[8];
    u16 deaths_special[8];
    u16 kills_normal[8];
    u16 deaths_normal[8];
    char pad4C[0x38];
    char name[9];
    u8 counted;
    char pad8E[0x1];
    u8 unk8F;
    char pad90[0x2];
    u8 unk92;
};
struct Shared_ReactionPlayerRecord {
    char pad0[0x4];
    s32 unk4;
    char pad8[0x31C];
    s32 unk324;
    Shared_ReactionPlayer *unk328;
};

DEFERRED void CScene__DamageAllEnemies(CScene *world, s32 damage, s32 flags) {
    Shared_DamageEvent event;
    LevelActor_00225B7C **actors;
    s32 count;
    s32 i;
    DamageEvent_Set(&event, 0, damage, 102.399994f, flags, 0);
    count = world->m_nKind1;
    actors = (LevelActor_00225B7C **)world->m_pKind1;
    for (i = 0; i < count; i++) {
        CAI__CallHurt(actors[i], &actors[i]->behaviorFlags, &event);
    }
}

void AI_HearNoise(u8 *actor, CAI *ai, CVector3 *pos, s32 kind);

/* Every kind-1 actor (CScene::m_pKind1, count m_nKind1) hears a noise of the given kind at pos. */
DEFERRED void CScene__BroadcastNoise(CScene *scene, s32 kind, CVector3 *pos) {
    s32 n = scene->m_nKind1;
    s32 i;

    for (i = 0; i < n; i++) {
        CGameObjectInstance *actor = scene->m_pKind1[i];
        AI_HearNoise((u8 *) actor, &actor->m_AI, pos, kind);
    }
}

#ifdef TARGET_PORT
#include "structs/world.h"

/* Host body (the Blind One fight reaches it): the actor table and its count at the N64 scene
 * offsets 0xE0/0xE8, the definition at actor + 0x14 and the N64 stride 0x288; the same walk
 * as CScene__UnmarkGeneratorsSaved (CScene__UnmarkGeneratorsSaved) on host members: every type-3 actor
 * (a generator) gets its saved bit. */
DEFERRED void CScene__MarkGeneratorsSaved(void *arg0)
{
  CScene *scene = (CScene *) arg0;
  s32 i;

  for (i = 0; i < scene->m_nActors; i++)
  {
    CGameObjectInstance *rec = &scene->m_pActors[i];

    if (*(s32 *) rec->header.m_pIntelligence == 3)
    {
      CScene__SetInstanceSavedBit(arg0, rec, 1);
    }
  }
}
#else
DEFERRED void CScene__MarkGeneratorsSaved(void *arg0)
{
  s32 temp_s2;
  int new_var;
  s32 var_s0;
  s32 var_s1;
  void *temp_a1;
  temp_s2 = *((s32 *) (((s8 *) arg0) + 0xE8));
  var_s0 = 0;
  var_s1 = 0;
  if (temp_s2 > 0)
  {
    new_var = 3;
    var_s1 = 0;
    do
    {
      temp_a1 = (*((s32 *) (((s8 *) arg0) + 0xE0))) + var_s1;
      if ((*(*((s32 **) (((s8 *) temp_a1) + 0x14)))) == new_var)
      {
        CScene__SetInstanceSavedBit(arg0, temp_a1, 1);
      }
      var_s0 += 1;
      var_s1 += 0x288;
    }
    while (var_s0 < temp_s2);
  }
}
#endif

struct LevelActor_00225D0C;
struct Def;
struct Def {
    s32 type;
    s32 flags;
    char pad8[0x20];
    s16 id;
};
typedef struct LevelActor_00225D0C LevelActor_00225D0C;
struct LevelActor_00225D0C {
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    s16 modelIndex;
    u16 pad6;
    CVector3 position0;
    CIntelligence *definition; /* 0x14 in Turok 2 */
    s32 model;
    CVector3 position1;
    CVector3 motionAngles;
    void *attached;
    s32 motionState;
    s32 motionIndex;
    f32 floorHeight;
    CVector3 floorDirection;
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    Box bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    Shared_ParticleLighting lighting[2];
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};
typedef struct Def Def;

DEFERRED void CScene__UnmarkGeneratorsSaved(void *arg0) {
    CScene *object;
    s32 count;
    s32 i;
    s32 offset;
    LevelActor_00225D0C *entry;
    s32 field;
    s32 three;
    object = arg0;
    count = object->m_nActors;
    i = 0;
    if (count > 0) {
        three = 3;
        offset = 0;
        do {
            object = arg0;
            entry = (void *)((char *)object->m_pActors + offset);
            field = ((struct Def *)entry->definition)->type;
            if (field != three) {
                i += 1;
            } else {
                CScene__SetInstanceSavedBit(arg0, entry, 0);
                i += 1;
            }
            offset += (s32)sizeof(CGameObjectInstance); /* sizeof(LevelActor_00225D0C) in Turok 2 */
        } while (i < count);
    }
}

/* Curve i: the table starts with the element size, the data at +8. */
DEFERRED void *CScene__GetCurve(CScene *arg0, s32 arg1) {
    s32 *t = arg0->m_pCurves;
    return (s8 *)t + (arg1 * *t + 8);
}

void Rumble_Start(sptr a, u8 *p, CVector3 v, f32 f, f32 g, sptr h);

DEFERRED void CScene__StartRumbles(CScene *lv, sptr a1, s32 id, CVector3 v, f32 f6, f32 f7) {
    s32 lo;
    s32 hi;
    s32 i;
    s32 *t = lv->m_pRumbleTypes;
    s32 n = t[1];
    if (BinaryRange(t + 2, n, id, &lo, &hi) != 0) {
        i = lo;
        if (hi >= i) {
            do {
                s32 *tbl = lv->m_pRumbles;
                Rumble_Start(a1, (u8 *)tbl + (i * tbl[0] + 8), v, f6, f7, 0);
                i++;
            } while (hi >= i);
        }
    }
}

#ifdef TARGET_PORT /* the scene (CScene, structs/world.h) at its host layout: m_pIntelligence */
#define RESOURCE_FILE_AT __builtin_offsetof(CScene, m_pIntelligence)
#else
#define RESOURCE_FILE_AT 0x64
#endif

/* (one conditional block of its own: tools/merge_module.py reads the definition below as a function) */
typedef struct {
    /* 0x00 */ u8 pad0[RESOURCE_FILE_AT];
    void *file;
} Resource;

/* the definition takes the callers' type (one prototype for the merged object); the view reads it */
DEFERRED sptr CScene__GetIntelligence(CScene *scene, s32 arg1) {
#define arg0 ((Resource *) scene)
    if (arg1 == -1) {
        return 0;
    }
    return CISet__GetBlock(arg0->file, arg1);
#undef arg0
}

DEFERRED Ent *CScene__FindIntelligence(CScene *lv, s32 a, s32 b) {
    s32 n = lv->m_pIntelligence[0];
    s32 i = 0;
    Ent *e;
    if (n > 0) {
        do {
            e = CISet__GetBlock(lv->m_pIntelligence, i);
            if (a == -1 || e->key == a) {
                if (b == -1 || e->unkC == b) {
                    return e;
                }
            }
            i++;
        } while (i < n);
    }
    return 0;
}

struct Container;
typedef struct Container Container;
struct RecordRecAtE4;
typedef struct RecordRecAtE4 RecordRecAtE4;
struct Sub18;
struct Sub18 {
    s32 unk0;
    char pad4[8];
    s16 unkC;
};
/* Turok 2 LevelActor_00225F6C: def 0x14, type id 0xB8, size 0x288
 * (Rage Wars 0x18 / 0xE4 / 0x2E8). */
struct RecordRecAtE4 {
    char pad0[0x14];
    struct Sub18 *unk14;
    char pad18[0xA0];
    u16 unkB8;
    char padBA[0x288 - 0xBA];
};
/* Turok 2: actor table at 0xE0 (Rage Wars 0x138). */
struct Container {
    char pad0[0xE0];
    struct RecordRecAtE4 *unkE0;
    s32 unkE4;
    s32 unkE8;
};
typedef struct Sub18 Sub18;
#ifdef TARGET_PORT
#include "common.h"
#include "structs/world.h"

/* The port: the same on CScene/CGameObjectInstance (the N64 views put the actor table at 0xE0,
 * the instance's definition at 0x14 and its type id at 0xB8, with the N64 stride 0x288); the
 * definition (asset data) keeps its words: the type at 0, the id (s16) at 0xC. */
DEFERRED s32 CScene__FindInstances(void *arg0, s32 key0, s32 key1, s32 key2, CGameObjectInstance **out, s32 max) {
    CScene *scene = (CScene *) arg0;
    s32 i = scene->m_nFirstActor;
    s32 end = scene->m_nActors;
    CGameObjectInstance *base = scene->m_pActors;
    s32 count = 0;
    CGameObjectInstance *rec;

    for (; i < end; i++) {
        rec = &base[i];
        if (count >= max) {
            return count;
        }
        if ((key0 == -1 || rec->m_nTypeFlag == key0) && (key1 == -1 || rec->header.m_pIntelligence->m_Type == key1)
            && (key2 == -1 || rec->header.m_pIntelligence->m_Id == key2)) {
            *out++ = rec;
            count++;
        }
    }
    return count;
}
#else
#define arg0 ((Container *) scene)
#define out ((RecordRecAtE4 **) outArg)
/* the definition takes the port body's types (one prototype for the merged object); the view reads them */
DEFERRED s32 CScene__FindInstances(void *scene, s32 key0, s32 key1, s32 key2, struct CGameObjectInstance **outArg, s32 max) {
    s32 i = arg0->unkE4;
    s32 end = arg0->unkE8;
    RecordRecAtE4 *base = arg0->unkE0;
    s32 count = 0;
    RecordRecAtE4 *rec;
    for (; i < end; i++) {
        rec = &base[i];
        if (count >= max) {
            return count;
        }
        if ((key0 == -1 || rec->unkB8 == key0) && (key1 == -1 || rec->unk14->unk0 == key1)
            && (key2 == -1 || rec->unk14->unkC == key2)) {
            *out++ = rec;
            count++;
        }
    }
    return count;
}
#undef arg0
#undef out
#endif

void CEngineApp__DrawFilledRect(u8 *a, u8 *col, s32 c, s32 d, s32 e, s32 f);

DEFERRED void CScene__DrawWarpFade(CScene *lv) {
    u8 col[4];
    f32 t;
    CWarpFadeColor *p;
    s32 a = lv->m_WarpState;
    if (a != 0) {
      if (a >= 0) {
       if (a < 3) {
        p = &gWarpFadeColors[lv->m_WarpFade];
        if (p->length != 0.0f) {
            col[0] = p->r;
            col[1] = p->g;
            col[2] = p->b;
            t = lv->m_WarpTime * 255.0f / p->length;
            col[3] = (u32)t;
            CEngineApp__DrawFilledRect(&gEngineApp, col, 0, 0, gScreenWidth, gScreenHeight);
        }
       }
      }
    }
}

/* ABI (docs/port/ABI.md, abicheck void_used): the callers use the result, on MIPS v0 of
 * the last call; the host returns it. */
DEFERRED void * CScene__GetPathHeader(CScene *arg0, s32 arg1) {
    return CISet__GetBlock(CISet__GetBlock(arg0->unk_68, arg1), 0);
}

/* Entry arg2 of table arg1: a word giving the entry size, then the entries
 * from +8. */
DEFERRED void *CScene__GetPathPoint(CScene *arg0, s32 arg1, s32 arg2) {
    s32 *t = (s32 *) CISet__GetBlock(CISet__GetBlock(arg0->unk_68, arg1), 1);
    return (s8 *)t + (arg2 * *t + 8);
}

extern f32 CVector3__Distance(void *a, void *b);

/* Finds the point of the given type nearest to position; copies it to out and returns its value (0 when none). */
VM_TEXT DEFERRED s32 CScene__FindNearestWarpDest(void *world, void *position, s32 type, SearchPoint *out) {
    s32 i;
    s32 best;
    SearchValues *values;
    SearchPoints *points;
    s32 *vals;
    SearchPoint *pts;
    s32 count;
    f32 distance;
    f32 minimum;
    sptr resource;          /* the index (a pointer) */
    s32 pool;
    void **handle;
    void *table;

    pool = gEngineApp.m_CartridgeIndex;
    CCache__LoadPersistantIndex(0, &resource, pool, sCSceneCacheDesc_CartridgeIndex);
    i = 0;
    best = 0;
    handle = CCache__LoadPersistantSubBlock_v(0, resource, 8, pool, world, 0, sCSceneCacheDesc_WarpDests);
    table = *handle;
    values = CISet__GetBlock(table, 0);
    count = values->count;
    vals = values->values;
    points = CISet__GetBlock(table, 1);
    minimum = 3.4028235e38f;
    pts = points->points;
    for (; i < count; i++) {
        if (pts[i].type == type) {
            distance = CVector3__Distance(&pts[i], position);
            if (distance < minimum) {
                minimum = distance;
                best = i;
            }
        }
    }
    if (best != 0) {
        *out = pts[best];
    }
    CCache__Deallocate(0, handle);
    if (best != 0) {
        return vals[best];
    }
    return 0;
}

s32 Sound__Play2D(s32);

VM_TEXT DEFERRED void CScene__RequestWarp(CScene *arg0, s32 arg1, s32 arg2) {
    arg0->m_WarpState = 1;
    arg0->m_WarpFade = arg2;
    arg0->m_WarpID = arg1;
    if ((arg2 == 2) && (arg1 != 0x3E7)) {
        Sound__Play2D(0xBE0);
    }
}

VM_TEXT DEFERRED void CScene__SetupLighting(CScene *menu) {
    CScene__LoadLightSettings(menu);
    gFogEnabled = (menu->m_pLevelHeader->m_Flags >> 2) & 1;
}

void CScene__RestoreFiredEvent(void *, void *);
struct State_func_8044D5C4_de;
struct State_func_8044D5C4_de {
    char p[0x1168];
    s32 unk1168;
    s32 unk116C;
    char q[8];
    s32 unk1178;
    s32 unk117C;
};
typedef struct State_func_8044D5C4_de State_func_8044D5C4_de;
void Cinema_Enable();
void Cinema_Disable();
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
VM_TEXT DEFERRED void CScene__RestoreFiredEvents(CScene *w) {
    s32 i, n;
    Cinema_Disable();
    n = w->m_nRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pRecords[i]);
    n = w->m_nAltRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pAltRecords[i]);
    Cinema_Enable();
}
#else
#define arg0 ((State_func_8044D5C4_de *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
VM_TEXT DEFERRED void CScene__RestoreFiredEvents(CScene *scene) {
 s32 i,n;
 Cinema_Disable();
 n=arg0->unk1168;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk1178+i*20);
 n=arg0->unk116C;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk117C+i*20);
 Cinema_Enable();
}
#undef arg0
#endif

struct Item_func_8044D668_de;
typedef struct Item_func_8044D668_de Item_func_8044D668_de;
struct Item_func_8044D668_de {
    char pad0[0x11];
    u8 kind;
    char pad12[2];
};
struct Level;
struct Level {
    char pad0[0x1168];  /* record tables 0x58 earlier than in Rage Wars */
    s32 counts[2];
    char pad1170[8];
    Item_func_8044D668_de *items[2];
};
typedef struct Level Level;
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
VM_TEXT DEFERRED void CScene__ReapplyFiredEvents(CScene *w) {
    s32 i, k, n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    for (k = 0; k < 2; k++) {
        n = k == 0 ? w->m_nRecords : w->m_nAltRecords;
        for (i = 0; i < n; i++) {
            item = (Item_func_8044D668_de *) &(k == 0 ? w->m_pRecords : w->m_pAltRecords)[i];
            if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
                CScene__RestoreFiredEvent(w, item);
            }
        }
    }
    Cinema_Enable();
}
#else
#define level ((Level *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
VM_TEXT DEFERRED void CScene__ReapplyFiredEvents(CScene *scene) {
    s32 i;
    s32 n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    n = level->counts[0];
    for (i = 0; i < n; i++) {
        item = &level->items[0][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    n = level->counts[1];
    for (i = 0; i < n; i++) {
        item = &level->items[1][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    Cinema_Enable();
}
#undef level
#endif
#ifdef TARGET_PORT
/* The port: the scene's delayed events typed (structs/world.h): fire them all and free them. */
#include "common.h"
#include "structs/world.h"

void LevelEvent_Fire(void *event);
VM_TEXT DEFERRED void CScene__FlushDelayedEvents(CScene *scene) {
    CDelayedEvent *d = scene->m_DelayedEvents.m_pHead;
    CDelayedEvent *next;

    while (d != NULL) {
        next = d->m_pNext;
        LevelEvent_Fire((sptr)d->m_pEvent); /* full width (soak S12, --high-memory) */
        CList__Remove(&scene->m_DelayedEvents, d);
        CList__AddHead(&scene->m_FreeDelayedEvents, d);
        d = next;
    }
}
#else
struct Entry_func_8044D794_de;
typedef struct Entry_func_8044D794_de Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
typedef struct Owner_func_8044D794_de Owner_func_8044D794_de;
struct Entry_func_8044D794_de;
struct Entry_func_8044D794_de {
    char pad0[4];
    struct Entry_func_8044D794_de *next;
    s32 handle;
};
struct Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
struct Owner_func_8044D794_de {
    char pad0[0x6C];
    void *unk6C;
    char pad70[0xC];
    void *unk7C;
    char pad80[0x1100];
    struct Entry_func_8044D794_de *active;
    char pad1184[0x10]; /* active at 0x1180, spare at 0x1194 */
    struct Entry_func_8044D794_de *spare;
};
void LevelEvent_Fire(void *event);
#define arg0 ((Owner_func_8044D794_de *) scene)
/* the definition takes the port body's types (one prototype for the merged object); the view reads them */
VM_TEXT DEFERRED void CScene__FlushDelayedEvents(CScene *scene) {
    Entry_func_8044D794_de *entry;
    Entry_func_8044D794_de *next;
    entry = arg0->active;
    while (entry != 0) {
        next = entry->next;
        LevelEvent_Fire(entry->handle);
        CList__Remove(&arg0->active, entry);
        CList__AddHead(&arg0->spare, entry);
        entry = next;
    }
}
#undef arg0
#endif

/* Tears the level down: releases its resources' handles. */
VM_TEXT DEFERRED void CScene__UnloadLevel(LevelState *level) {
    void *p;

    p = CPlayerPool__GetFirstActive(&gEngineApp_m_Game.m_PlayerList);
    if (p != NULL) {
        CPlayer__StopLoopingSounds(p);
    }
    LoopSounds_StopAll(LoopSounds_Get());
    Sound__DetachAll();
    Sound__StopFlags(0x1000);
    CCache__Deallocate(0, level->handle98);
    CCache__Deallocate(0, level->handle9C);
    CCache__Deallocate(0, level->handleA0);
    CCache__Deallocate(0, level->handleA4);
    CCache__Deallocate(0, level->handleA8);
    CCache__Deallocate(0, level->handleB0);
    CCache__Deallocate(0, level->handleB4);
    CCache__Deallocate(0, level->handleCC);
    CCache__Deallocate(0, level->handleAC);
    CCache__Deallocate(0, level->handleC8);
}

/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__GetObject_call CScene__GetObject
#else
s32 CScene__GetObject_call(struct CScene *, s32) __asm__("CScene__GetObject");
#endif
#ifdef TARGET_PORT
void T2Port_ParticlesBuild(void *scene, void *data); /* port/game/t2particles.c */
#endif

VM_TEXT DEFERRED void CScene__DecompressParticles(sptr arg0, void ***arg1) {
    void *obj;
    GroupCode8044E2B8 *group;
    Piece *piece;
    sptr base;
    s32 count;
    s32 i;
    if (ProPackDecompress(arg1, 0) != 0) {
        obj = **arg1;
#ifdef TARGET_PORT
        /* the host copies the templates with full-width pointers (port/game/t2particles.c);
         * block 2 keeps its offsets */
        T2Port_ParticlesBuild((void *) (sptr) arg0, obj);
        return;
#endif
        group = CISet__GetBlock(obj, 2);
        count = group->count;
        piece = group->pieces;
        i = 0;
        base = (sptr) CISet__GetBlock(obj, i);
        if (count > 0) {
            do {
                if (piece[i].tag == -1) {
                    piece[i].tag = 0;
                } else {
                    piece[i].tag = CScene__GetObject_call(arg0, piece[i].tag);
                }
                piece[i].unk18 = base + piece[i].unk18;
                piece[i].unk1C = base + piece[i].unk1C;
                piece[i].unk20 = base + piece[i].unk20;
                piece[i].unk24 = base + piece[i].unk24;
                piece[i].unk28 = base + piece[i].unk28;
                piece[i].unk2C = base + piece[i].unk2C;
                piece[i].unk30 = base + piece[i].unk30;
                piece[i].unk34 = base + piece[i].unk34;
                piece[i].unk38 = base + piece[i].unk38;
                i++;
            } while (i < count);
        }
    }
}

DEFERRED s32 CScene__FindNearestPathPoint(CScene *lv, s32 unused, CVector3 *pt) {
    CVector3 d;
    f32 bestIdx = -1.0f;
    f32 bestD = 0.0f;
    s32 i = 0;
    s32 *t;
    s32 n;
    u8 *p;
    /* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS the inner CISet__GetBlock gets this
     * function's second argument in $a1 untouched (the path index, passed down the same way by
     * CPathTrack__StartAtNearestPoint); the host passes it. */
    t = CISet__GetBlock(CISet__GetBlock((s32 *) lv->unk_68, unused), 1);
    n = t[1];
    p = (u8 *)(t + 2);
    if (n > 0) {
        do {
            f32 dd;
            CVector3__Subtract(&d, pt, p);
            dd = d.x * d.x + d.y * d.y + d.z * d.z;
            if (bestIdx < 0.0f || dd < bestD) {
                bestIdx = (f32)i;
                bestD = dd;
            }
            i++;
            p += 0x1C; /* 0x38 in Rage Wars */
        } while (i < n);
    }
    return (s32)bestIdx;
}

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad4[0x24];
    /* 0x28 */ f32 x;
    /* 0x2C */ f32 y;
    /* 0x30 */ f32 z30;
    /* 0x34 */ f32 z34;
    /* 0x38 */ u8 pad38[0x14];
    /* 0x4C */ f32 x2;
    /* 0x50 */ f32 y2;
    /* 0x54 */ f32 z2;
} Element;
#ifdef TARGET_PORT
int T2Port_LegacyIntel9(void); /* port/game/t2debug.c */
#endif

/* VM_TEXT on the definition (the merged object drops a prototype of a function it defines) */
DEFERRED VM_TEXT void CScene__DecompressIntelligence(sptr unused, void ***resource) {
    f32 scale = 1.0f;
    s32 *table;
    s32 count;
    s32 i;
    Element *e;

    if (ProPackDecompress(resource, 0)) {
        table = (s32 *) **resource;
        count = *table;
        for (i = 0; i < count; i++) {
            e = CISet__GetBlock(table, i);
            switch (e->type) {
                case 1:
                    e->x *= scale;
                    e->z30 *= scale;
                    e->z34 *= scale;
                    e->x2 *= scale;
                    e->y2 *= scale;
                    e->z2 *= scale;
                    break;
                case 11:
                    e->x *= scale;
                    e->y *= scale;
                    break;
                case 0:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                    break;
            }
#ifdef TARGET_PORT
            /* The port (round 17 f3, tools/port/n64cmp.py): the archive's swap plan has no struct for
             * the type-9 (interactive anim) body (tools/assets/swap_spec.json, "no struct yet"), so its
             * three words (the type flags, the start animation id, 0) stayed big-endian: on a
             * little-endian host InteractiveAnim_Advance took 0xFD530000 for animation 0x53FD and the
             * flag tests (m_dwTypeFlags & 1, ...) read the wrong bits. Swapped here, once per load.
             * T2_LEGACY_INTEL9=1 (DEBUG-ONLY) keeps the old bytes. */
            if (e->type == 9 && !T2Port_LegacyIntel9()) {
                u32 *w = (u32 *) ((u8 *) e + 0x14);
                w[0] = __builtin_bswap32(w[0]);
                w[1] = __builtin_bswap32(w[1]);
                w[2] = __builtin_bswap32(w[2]);
            }
#endif
        }
    }
}

/* The counts this clears, scattered over a large block. */

DEFERRED void CScene__ClearListCounts(CScene *arg0) {
    arg0->m_nDrawActors = 0;
    arg0->m_nMovers = 0;
    arg0->m_nListed = 0;
    arg0->m_nRegistered = 0;
    arg0->m_nKind1 = 0;
    arg0->m_nFlagged = 0;
    arg0->m_nKind5 = 0;
    arg0->m_nList6 = 0;
    arg0->m_nList7 = 0;
    arg0->m_nDeferred = 0;
    arg0->m_nSectors = 0;
}

s32 CStaticObject__SpawnEvents(sptr, s32);
#ifdef TARGET_PORT
/* Host body (round 10 y3, docs/port/FIDELITY.md 5): the same by member. The N64 view read the
 * timer (0x18EB8), the count (0xBF4) and the s32 list (0xAF4) at N64 offsets of CScene, so on
 * the host no listed static object ever fired its events: demo 1's ambient particle (type 103,
 * kind 8 at gf 201, every second after) never spawned. */
#include "structs/world.h"
DEFERRED void CScene__AdvanceListedInstances(void *arg0) {
    CScene *world = (CScene *) arg0;
    f32 *timer = (f32 *) &world->unk_18EB8; /* a float: the second's fraction */
    f32 t;
    s32 all;
    s32 i;

    t = *timer + gFrameIncrement;
    *timer = t;
    all = t > 0.0f;
    if (all) {
        *timer = t - (f32) ((s32) t + 1);
    }
    for (i = 0; i < world->m_nListed; i++) {
        CStaticObject__SpawnEvents((sptr) world->m_pListed[i], all);
    }
}
#else
DEFERRED void CScene__AdvanceListedInstances(void *arg0)
{
  f32 temp_f1;
  int new_var;
  s32 temp_a0;
  s32 var_s0;
  s32 var_s3;
  void *var_s1;
  temp_f1 = (*((f32 *) (((s8 *) arg0) + 0x18EB8))) + gFrameIncrement;
  *((f32 *) (((s8 *) arg0) + 0x18EB8)) = temp_f1;
  var_s3 = 1;
  if (!(temp_f1 > 0.0f))
  {
    var_s3 = 0;
  }
  if (var_s3 != 0)
  {
    *((f32 *) (((s8 *) arg0) + 0x18EB8)) = (f32) (temp_f1 - ((f32) (((s32) temp_f1) + 1)));
  }
  var_s0 = 0;
  new_var = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0xBF4))) > new_var)
  {
    var_s1 = arg0;
    do
    {
      temp_a0 = *((s32 *) (((s8 *) var_s1) + 0xAF4));
      var_s1 += 4;
      CStaticObject__SpawnEvents(temp_a0, var_s3);
      var_s0 += 1;
    }
    while (var_s0 < (*((s32 *) (((s8 *) arg0) + 0xBF4))));
  }
}
#endif

void CPlayerPool__Update(CPlayerPool *list);
s32 CPlayerPool__CheckAllReady(CPlayerPool *list);
void CEngineApp__QuitToLegal(u8 *);
void LoopSounds_Update(sptr);

/* Draws one actor, keeping the frame time step. */
static __inline__ void drawActor(CGameObjectInstance *actor) {
    f32 step = gFrameIncrement;

    CGameObjectInstance__UpdateAI(actor);
    if (actor->m_dwFlags & 0x2000) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    gFrameIncrement = step;
}

/* Draws the leading run of type-2 actors, then the mode extras, then the rest.
 * One actor variable for both loops: its pseudo outlives the first loop, so
 * the first loop keeps the kind-test load ($a0) apart from it ($s0). */
DEFERRED void CScene__AdvanceInstances(CScene *world) {
    CGameObjectInstance *actor;
    s32 i = 0;
    s32 count = world->m_nRegistered;
    CGameObjectInstance **actors = world->m_pRegistered;

    while (i < count) {
        if (actors[i]->header.m_pIntelligence->m_Type != 2) {
            break;
        }
        actor = actors[i++];
        drawActor(actor);
    }
    if (gEngineApp_m_Game.m_Settings.mode == 13) {
        CPlayerPool__Update(&gEngineApp_m_Game.m_PlayerList);
    }
    if (CPlayerPool__CheckAllReady(&gEngineApp_m_Game.m_PlayerList)) {
        CEngineApp__QuitToLegal(&gEngineApp);
    }
    while (i < count) {
        actor = actors[i++];
        drawActor(actor);
    }
    LoopSounds_Update(LoopSounds_Get());
}

/* Turok 2 layouts (Rage Wars offsets in comments). */

typedef struct Container_002265E8 {
    u8 pad0[0xE0];
    CGameObjectInstance *actors;        /* 0xE0 (0x138) */
    s32 unkE4;
    s32 actorCount;            /* 0xE8 (0x140) */
} Container_002265E8;

/* Index of arg1 in the scene's actor array, or -1. */
DEFERRED s32 CScene__GetInstanceIndex(void *arg0, void *arg1) {
    Container_002265E8 *scene = arg0;
    CGameObjectInstance *actor = arg1;
    void *base;
    if (actor->m_dwFlags & 0x80000) {
        return -1;
    }
    base = scene->actors;
    if (arg1 >= base &&
        (char *)arg1 <= (char *)base + ((u32)scene->actorCount * sizeof(CGameObjectInstance) - sizeof(CGameObjectInstance))) {
        return ((uptr)arg1 - (uptr)base) / sizeof(CGameObjectInstance);
    }
    return -1;
}

DEFERRED void CScene__GetEventRecords(CScene *lv, s32 which, s32 id, CSceneRecord **outRecs, s32 *outCount) {
    s32 lo;
    s32 hi;
    sptr a;
    s32 b;
    s32 off;
    if (which != 0) {
        a = (sptr) lv->m_pAltRecordIndex;
        b = lv->m_nAltRecords;
    } else {
        a = (sptr) lv->m_pRecordIndex;
        b = lv->m_nRecords;
    }
    if (BinaryRange(a, b, id, &lo, &hi) != 0) {
        off = lo * 0x14;
        if (which != 0) {
            *outRecs = (CSceneRecord *)((u8 *)lv->m_pAltRecords + off);
        } else {
            *outRecs = (CSceneRecord *)((u8 *)lv->m_pRecords + off);
        }
        *outCount = hi - lo + 1;
    } else {
        *outRecs = 0;
        *outCount = 0;
    }
}

#ifdef TARGET_PORT

/* Host body on CScene (the view below has the N64 offsets and a 4-byte sector handle). */
DEFERRED void CScene__ReleaseSectors(CScene *scene) {
    s32 i;
    s32 count = scene->m_nSectors;

    for (i = 0; i < count; i++) {
        CCache__Unlock(0, (struct CMemEntry *)scene->m_Sectors[i].m_hData);
    }
    scene->m_nDrawActors = 0;
    scene->m_nListed = 0;
    scene->m_nDeferred = 0;
    scene->m_nSectors = 0;
}
#else
typedef struct Entry8 {
    s32 handle;
    s32 unk4;
} Entry8;

typedef struct Cache {
    u8 pad0[0x8EC];
    s32 unk8EC;           /* 0x8EC */
    u8 pad8F0[0xBF4 - 0x8F0];
    s32 unkBF4;           /* 0xBF4 */
    u8 padBF8[0x1064 - 0xBF8];
    s32 unk1064;          /* 0x1064 */
    u8 pad1068[0x14A8 - 0x1068];
    s32 count;            /* 0x14A8 */
    Entry8 entries[1];    /* 0x14AC */
} Cache;


#define cache ((Cache *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the view reads it */
DEFERRED void CScene__ReleaseSectors(CScene *scene) {
    s32 i;
    s32 count;
    Entry8 *entries;

    count = cache->count;
    entries = cache->entries;
    for (i = 0; i < count; i++) {
        CCache__Unlock(0, entries[i].handle);
    }
    cache->unk8EC = 0;
    cache->unkBF4 = 0;
    cache->unk1064 = 0;
    cache->count = 0;
}
#undef cache
#endif

DEFERRED void func_002267A0(CScene *arg0, s32 arg1, s32 arg2, s32 arg3) {
    sptr temp_v0;
    sptr var_a0;

    if (arg1 != 0) {
        var_a0 = (sptr) arg0->m_pBitSections;
    } else {
        var_a0 = (sptr) arg0->m_pBitSections2;
    }
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(var_a0, 0), arg3), arg2);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    CISet__GetBlock(temp_v0, 1);
}

DEFERRED void CScene__SetSavedBit(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5)
{
  s32 temp_a0;
  sptr new_var;
  sptr temp_a1;
  sptr temp_v0;
  sptr var_a0;
  s32 var_v1;
  u8 *temp_v0_2;
  u8 *temp_v1;
  if (arg1 != 0)
  {
    var_a0 = *((s32 *) (((s8 *) arg0) + 0x6C));
  }
  else
  {
    var_a0 = *((s32 *) (((s8 *) arg0) + 0x70));
  }
  temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(var_a0, 0), arg3), arg2);
  CISet__GetBlock(temp_v0, 0);
  CISet__GetBlockSize(temp_v0, 1);
  new_var = CISet__GetBlock(temp_v0, 1);
  temp_a1 = new_var;
  temp_a0 = 1 << (arg4 & 7);
  if (arg5 != 0)
  {
    var_v1 = arg4;
    if (arg4 < 0)
    {
      var_v1 = arg4 + 7;
    }
    temp_v1 = temp_a1 + (var_v1 >> 3);
    *temp_v1 |= temp_a0;
    return;
  }
  temp_v0_2 = temp_a1 + (arg4 / 8);
  *temp_v0_2 &= ~temp_a0;
}

DEFERRED s32 CScene__GetSavedBit(CScene *lv, s32 sel, s32 c, s32 b, s32 bit) {
    s32 *t;
    s32 *base;
    u8 *bits;
    s32 m;
    if (sel != 0) {
        base = lv->m_pBitSections;
    } else {
        base = lv->m_pBitSections2;
    }
    t = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(base, 0), b), c);
    CISet__GetBlock(t, 0);
    CISet__GetBlockSize(t, 1);
    bits = (u8 *)CISet__GetBlock(t, 1);
    m = 1 << (bit & 7);
    return (bits[bit / 8] & m) != 0;
}

DEFERRED s32 CScene__GetLevelBitInLevel(BitTableOwner *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->unk70, 0), arg1), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}

DEFERRED s32 CScene__GetInstanceSavedBitInLevel(CScene *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->m_pBitSections, 0), arg1), 0);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}

DEFERRED s32 CScene__IsLevelActorKilledInLevel(Unk00226B34 *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->data, 0), arg1), 1);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}

/* Tests a bit of the given level's saved-actor bitmap. */
DEFERRED s32 CScene__IsEventFiredInLevel(CScene *arg0, s32 arg1, s32 arg2) {
    s32 *temp_v0;
    s32 temp_v1;
    s32 var_s1;
    s8 *temp_a0;

    var_s1 = arg2;
    temp_v0 = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(arg0->m_pBitSections, 0), arg1), 2);
    CISet__GetBlock(temp_v0, 0);
    CISet__GetBlockSize(temp_v0, 1);
    temp_a0 = CISet__GetBlock(temp_v0, 1);
    temp_v1 = 1 << (var_s1 & 7);
    if (var_s1 < 0) {
        var_s1 += 7;
    }
    return ((u8) temp_a0[var_s1 >> 3] & temp_v1) != 0;
}

DEFERRED sptr CScene__GetLinkGroupCountPtr(CScene *arg0, s32 arg1) {
    sptr temp_v0;

    temp_v0 = CISet__GetBlock(arg0->m_pBitSections, 1);
    CISet__GetBlock(temp_v0, 0);
    return CISet__GetBlock(temp_v0, 1) + arg1;
}

DEFERRED void SortRecord_Swap(RenderSortRecord *left, RenderSortRecord *right) {
    RenderSortRecord temporary = *left;
    *left = *right;
    *right = temporary;
}

/* Sort callback: -1 when arg0's key is the lower, else 1. */
DEFERRED s32 SortRecord_CompareDescending(SortEntry_00226D0C *arg0, SortEntry_00226D0C *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}

/* qsort-style comparator on the f32 key at 0x4. */
DEFERRED s32 SortRecord_Compare(SortItem *arg0, SortItem *arg1) {
    s32 r = 1;

    if (arg0->key > arg1->key) {
        r = -1;
    }
    return r;
}

DEFERRED s32 func_00226D54(void *arg0)
{
  s32 var_s0;
  s8 *new_var;
  void *var_s1;
  s32 new_var2;
  int new_var3;
  var_s0 = 0;
  new_var3 = 0;
  if ((*((s32 *) (((s8 *) arg0) + 0x14A8))) > new_var3)
  {
    var_s1 = arg0;
    loop_2:
    var_s0 += 1;

    if ((new_var2 = *((s32 *) (((s8 *) CISet__GetBlock(*(*((s32 **) (((s8 *) var_s1) + 0x14AC))), 2)) + 4))) != 0)
    {
      return 1;
    }
    var_s1 += 8;
    new_var = ((s8 *) arg0) + 0x14A8;
    if (var_s0 >= (*((s32 *) new_var)))
    {
      goto block_5;
    }
    goto loop_2;
  }
  block_5:
  return 0;

}

/* Rumble i: the table starts with the element size, the data at +8. */
DEFERRED void *CScene__GetRumble(CScene *arg0, s32 arg1) {
    s32 *t = arg0->m_pRumbles;
    return (s8 *)t + (arg1 * *t + 8);
}
sptr CScene__GetPathRegion(s32 ref);
void CScene__ConstructMap(void *level, CSceneMapRecord *out, s32 key);

DEFERRED s32 Blast_IsLivingEnemyInRange(CGameObjectInstance *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;
    CVector3__Subtract(&d, &e->header.m_vPos, target);
    dist = d.x * d.x + d.y * d.y + d.z * d.z;
    if (e->header.m_pIntelligence->m_Type == 1) {
        t = e->m_nTypeFlag;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0;
        }
        if (e->m_AI.m_Health != 0) {
            if (e->m_AI.m_Mode != 0x3D) {
                if (dist <= 6553600.0f) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

DEFERRED s32 Blast_GetStrength(CGameObjectInstance *e, CVector3 *target) {
    CVector3 d;
    f32 dist;
    u32 t;
    if (*(u8 *)e == 1 && e->header.m_pIntelligence->m_Type == 7) {
        CVector3__Subtract(&d, &e->header.m_vPos, target);
        dist = d.x * d.x + d.z * d.z;
        d.y = 0;
        if (262144.0f < dist) {
            return 0;
        }
        return 0xFF;
    }
    if (e->header.m_pIntelligence->m_Type == 1) {
        t = e->m_nTypeFlag;
        if (t == 0x44E || t == 0x453 || t == 0x451 || t == 0x450 || t == 0x454 || t == 0x455 || t == 0x456) {
            return 0xFF;
        }
    }
    return 0;
}
void CScene__LoadBitSections2(CScene *menu);
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 done;
    /* 0x08 */ u8 pad8[0x28 - 0x8];
    /* 0x28 */ s32 unk28;
    /* 0x2C */ u8 pad2C[4];
    /* 0x30 */ s32 unk30;
} Arg0_0042C418;
void CScene__LoadBitSections(Arg0_0042C418 *arg0);

VM_TEXT void CScene__InitPersistentData(CScene *menu) {
    CScene__LoadBitSections(menu);
    CScene__LoadBitSections2(menu);
    menu->m_PreviewState = 0;
    menu->m_PreviewTimer = 34;
    menu->m_PreviewOptions = gEngineApp_m_Settings.unlockedCheats;
}

void CLoader__LoadSync(void *reader, sptr address, sptr length, void *dest);
s32 SavedPlayers_IsActive();                                /* extern */

VM_TEXT void CScene__LoadBitSections(Arg0_0042C418 *arg0) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, arg0->unk28, arg0->unk30, (sptr) &gLoadSavePersistentData);
        arg0->done = 1;
    }
}

extern s32 SavedPlayers_IsActive(void);

VM_TEXT void CScene__LoadBitSections2(CScene *menu) {
    if (SavedPlayers_IsActive() == 0) {
        CLoader__LoadSync(&gLoader, menu->unk_2C, menu->unk_34, gGamePersistentData);
    }
}

/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__RequestWarp_call CScene__RequestWarp
#else
void CScene__RequestWarp_call(struct CScene *, s32, s32) __asm__("CScene__RequestWarp");
#endif

void CScene__WarpToLevel(CScene *arg0, s32 arg1);

VM_TEXT void CScene__WarpToLevel(CScene *arg0, s32 arg1) {
    if (arg1 != arg0->m_nLevel) {
        arg0->m_bWarpSpawn = 0;
        CScene__RequestWarp_call(arg0, ~arg1, 0);
    }
}

VM_TEXT void CScene__ConstructWarp(CScene *arg0) {
    arg0->m_WarpFade = 2;
    arg0->m_WarpState = 0;
    arg0->m_WarpTime = 0;
    arg0->m_bWarpSpawn = 0;
    arg0->m_LevelID = 0;
    arg0->m_WarpID = 0;
    arg0->unk_18FD8 = 0;
    arg0->m_PendingWarpID = 0;
}

typedef struct Entry {
    u8 data[0x20];
} Entry;

/* Resolves the references of every item of every group of the loaded resource and keeps it in the
 * level (CScene.unk_68; the references point into block 2 of m_pRegionData). Typed on CScene: the
 * m2c view put them at their N64 offsets 0x58/0x68, which on the host overwrote m_pCollision.
 * The item's ref stays an s32 (the ROM record); on the host it stays the region index
 * (CScene__GetPathRegion below). */
VM_TEXT DEFERRED void CScene__DecompressPaths(CScene *level, s32 ***handle) {
    s32 *groups;
    s32 ngroups;
    s32 i;
    s32 j;
    s32 count;
    ItemList *list;
    s32 ref;
    ListItem *item;
    Entry *entries;

    if (ProPackDecompress(handle, 0) != 0) {
        groups = **handle;
        ngroups = groups[0];
        for (i = 0; i < ngroups; i++) {
            list = CISet__GetBlock(CISet__GetBlock(groups, i), 1);
            count = list->count;
            item = list->items;
            for (j = 0; j < count; j++) {
                ref = item->ref;
#ifndef TARGET_PORT
                if (ref != -1) {
                    entries = (Entry *)((u8 *)CISet__GetBlock(level->m_pRegionData, 2) + 8);
                    item->ref = (sptr)&entries[ref];
                } else {
                    item->ref = 0;
                }
#endif
                Angle_Wrap(item->data);
                item++;
            }
        }
        level->unk_68 = groups;
    }
}

#ifdef TARGET_PORT
#include "structs/region.h"
/* The port: a path point's region. The item keeps the ROM's region index (-1: none) in its 4-byte
 * slot, which cannot hold a host pointer (--high-memory, macOS), and the region table holds host
 * CGameRegions (0x40 bytes, not the N64's 0x20): the readers (AI_GetPathTargetInfo,
 * AI_UpdatePathHeading) resolve it here, as CScene__GetRegion does. */
VM_TEXT DEFERRED sptr CScene__GetPathRegion(s32 ref) {
    if (ref == -1) {
        return 0;
    }
    return (sptr) ((CGameRegion *) ((u8 *) CISet__GetBlock(gScene.m_pRegionData, 2) + 8) + ref);
}
#endif
/* the N64 symbol by another C name: the merged object defines it DEFERRED (inline), and a call by its own
 * name after that definition would expand it (the CGameObjectInstance__LoadGeometry_call pattern) */
#ifdef TARGET_PORT
#define CScene__FindObjectType_call CScene__FindObjectType
#else
s32 CScene__FindObjectType_call(void *level, s32 key) __asm__("CScene__FindObjectType");
#endif

/* Looks key up in the level and loads its record into out (out->valid = 0 when there is none). */
VM_TEXT DEFERRED void CScene__ConstructMap(void *level, CSceneMapRecord *out, s32 key) {
    s32 id;
    s32 **h1;
    s32 **h2;
    s32 size;
    s32 res;

    id = CScene__FindObjectType_call(level, key);
    if (id == -1) {
        out->valid = 0;
        return;
    }
    res = CScene__GetObject_call(level, id);
    h1 = CCache__Request(0, res, 0x18, 0, 0, 0, sCSceneCacheDesc_ObjectIndex, 1);
    if (h1 != NULL) {
        h2 = CCache__Request(0, CISet__GetROMAddressAndSize(*h1, res, 1, &size), size, 0, 0, Model_DecompressCallback, sCSceneCacheDesc_Geometry, 1);
        if (h2 != NULL) {
            *out = *(CSceneMapRecord *)CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(*h2, 2), 0), 0), 0);
            CCache__Deallocate(0, h2);
        }
        CCache__Deallocate(0, h1);
    }
}
