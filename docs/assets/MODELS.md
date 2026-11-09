# Models and geometry (Phase 6, part a3)

Tool: `tools/assets/models.py` (Python 3, stdlib only).

```
python tools/assets/models.py baserom.us.z64 --out DIR [--ids 0,5,10-20] [--obj] [--gltf]
                              [--render [--textured | --uvcheck]] [--json] [--lod N]
```

`--json` writes `DIR/index.json`, one summary per object (ROM ranges, kind, LODs, parts, materials,
triangle counts, bone hierarchy). `--obj`/`--gltf` export a LOD as Wavefront OBJ+MTL or glTF 2.0
(`.gltf` + `.bin`, one primitive per part, POSITION/TEXCOORD_0/COLOR_0, material `extras` = the
material record). `--render` writes a 256x256 PNG from a small z-buffered software rasterizer
(`--textured` uses a preview texture decode, `--uvcheck` a checker per texture repeat).
The whole set (2705 objects, glTF + OBJ) exports in about 15 s.

Locating and decompressing go through `tools/assets/romfs.py` and `rnc.py`
(`fs.children(fs.object(i))[1]` + `fs.blob`; `Rom` is a thin wrapper over them). The stand-in
`rnc_min.py` of the first pass is gone (it matched the decompressor on every geometry block).
Texture previews use `textures.py`'s decoder. `--pose` places the vertices of animated models at
frame 0 of their animation 0 (`anim.parse_skeleton` / `parse_animation` / `pose` ->
`apply_pose`); `extract_all.py` exports them that way. Posed models are Z-up (ANIMATION.md), so `--render`
draws them with `up="z"` (upright); unposed scenery stays Y-up. The parser (`parse_geometry`) only sees
the decompressed bytes.

## Where the models are

All rendered geometry in the game is an object model. Static scenery ("stat geometry",
`CStaticObject__LoadGeometry`), simple actors ("simp geometry", `CLevelActor__LoadGeometry`),
animated actors ("anim geometry", `CGameObjectInstance__LoadGeometry`), batched models
(`Render_QueueModelByKey`) and the five scene geometry headers (`CScene__Construct`) all read the
same *objects* set. No CCache__Request with a geometry description reads anything else.

| what | ROM | how it is established |
|---|---|---|
| cartridge index | 0xFCBFB4 | `CEngineApp__Main` (func_00286674.c): `m_CartridgeIndex = 0xFCBFB4`; a CISet of 21 blocks covering 0xFCBFB4..0x1FF7AEC |
| objects index (cart block 0) | 0xFCC014..0xFCEA64 | `CScene__Construct`: "objects index", `m_ObjectsKey`; count 2705 + 2706 offsets |
| objects | 0xFCEA64..0x17618BC | contiguous, back to back, no gaps |
| object types (cart block 2) | 0x17655C4, 5424 B | `{s32 2, s32 2705, u16 id[2705]}`; `CScene__FindObjectType(id)` returns the object index |
| texture sets (cart block 3) | 0x1766AF4 | the material texture ids index it (`CScene__GetTextureSet`); 1839 entries (part a2) |

Coverage: objects index + objects = 0xFCC014..0x17618BC = 7,952,040 B = **25.0 %** of the asset
region 0x20F178..0x1FF7AEC (31,754,612 B), or 24.8 % if the region is taken from 0x1E0000 as in
ROMFS.md. The packed geometry blocks alone are 3,607,320 B (11.4 %).
Most of the rest is the animation sets of animated objects (object block 2, 4,122,312 B: part a5).

### An object (the key is its ROM address)

A CISet with 4 blocks. The game first reads 0x18 bytes at the key ("object index", "simp/stat/anim
object index"), then `CISet__GetROMAddressAndSize(index, key, n)` gives block n's ROM range.

| block | content | read by | sizes |
|---|---|---|---|
| 0 | object info: CISet {0: bounding box (2 x 3 f32, `CMtxF__TransformBox`), 1: u16 array, [3] = setId}. Empty `{0,0}` sets for simple objects | `CGameObjectInstance__SetupAnimation` ("anim object info") | 32 B (simple), 64..184 B (anim) |
| 1 | **geometry**, RNC method 1 (every one of the 2705 is packed) | Model_Decompress* | 3.6 MB in all |
| 2 | animations index (`unkA8` = `obj->animFile`) | CAnimationState__* (part a5) | 8 B empty set for simple objects |
| 3 | "static events" (a1's name): 8 B empty; for 37 objects a CUSet of 0x1C-byte records | ? | 8 / 40 / 64 B |

2705 objects: 2215 with a 3-block geometry ("simple": stat/simp), 490 with a 6-block geometry
("anim"; skeleton, hot points); 77 of them are placeholders with no vertices.

## Geometry (decompressed block 1)

Parser: `Model_Decompress` (src/us/code/func_00209AF4.c), reached through
`Model_DecompressCallback` (withExtra 0, withTerminator 0: "geometry", CStaticObject),
`Model_DecompressCallbackTerminated` (0, 1: "simp geometry", `Render_QueueModelByKey`) and
`Model_DecompressCallbackExtra` (1, 0: "anim geometry"). The same packed resource is valid for all
three; the flags only decide what the in-memory copy keeps.

Every level is a CISet (`s32 count; s32 offset[count + 1]`, offsets from the set's start,
`CISet__GetBlock`). The packed resource:

```
set {
  0: vertex lists     set { per LOD: CUSet {s32 width=16, s32 count, bytes transposed} }
  1: data block       copied as is (an empty set {0, 8} in 2647 models; 1-entry sets in the rest; unknown)
  2: LODs             set { per LOD: set { per part: set { 0: material record (0x18 B),
                                                         1: packed command stream } } }
  3: skeleton         (anim only) set { per bone: set { 0: children, 1: bone data } }
  4: hot points       (anim only) set { per slot: CUSet of 0x14-byte records }
  5: ?                (anim only) set { per slot: CUSet of 8-byte records, usually empty }
}
```

### Vertex lists (block 0)

One list per LOD (`vtxBase = GetBlock(frameTbl, lod) + 8`). Each is a `{s32 width, s32 count}`
header plus `width * count` bytes stored column-major: all byte 0s, then all byte 1s, and so on.
`CUSet__Transpose` turns it into `count` rows of 16 bytes, an F3DEX2 `Vtx`:

| off | type | field |
|---|---|---|
| 0x0 | s16[3] | x, y, z (model units; bone-local for anim geometry) |
| 0x6 | u16 | flag (0) |
| 0x8 | s16[2] | s, t (S10.5) |
| 0xC | u8[4] | r, g, b, a: colour, or a normal for lit models ("lit vertices", `Lighting_BuildLitVertices`) |

The vertices load sequentially: each vertex-load command takes the next `count` vertices of the
list (`vtxCounter`). Over all 2705 models every LOD's commands use up exactly its list.

### Material record (part block 0, 0x18 B; `Material_Unpack`, `Render_SetupMaterial`)

| off | type | field |
|---|---|---|
| 0x00 | s32 | texture id: index into the cartridge's texture sets (cart block 3); -1 = untextured |
| 0x04 | s32 | unk4 (runtime: `unk4 * 2 | (flags & 0x40 ? 1 : 0)`) |
| 0x08 | u32 | flags |
| 0x0C | u8[4] | prim colour (RGBA) |
| 0x10 | u8[4] | env colour |
| 0x14 | u16 | gSPTexture scale S (0.16 fixed) |
| 0x16 | u16 | gSPTexture scale T |

Flags (from `Render_SetupMaterial` / `Texture_Load`): 0x02 shade from vertex colour (combine
mode choice), 0x10 scale from the texture info instead (1 << (shift + 6)), 0x20 clamp S/T,
0x40 low bit of unk4, 0x80 render mode 6, 0x100 translucent (skipped when prim alpha 0),
0x200 no z-write/decal-style tail mode, 0x400 env colour used, 0x800 mirror, 0x1000 alternate
combiner, 0x2000 texture LOD (detail/mip, kind 4/5). The most common values are 0x48, 0x08, 0x49,
0x848, 0x808.

Texture coordinates: the RSP computes texel = s * scale / 65536 (S10.5), so the normalised
coordinate is `u = s * scale_s / 65536 / 32 / W`, with W x H = 2^info[2] x 2^info[3] from the
texture's info block (`Texture_Load`). The scale is a per-material choice (not tied to the
texture size: scale / 256 equals W in only about 15 % of the parts), so exporters need the texture size.
`attach_texture_sizes()` reads it; without it 32x32 is assumed.

### Packed command stream (part block 1; expanded by `Model_Decompress`)

Byte-oriented, big endian, ends with a 0 byte. `c` is the first byte:

| c | bytes | meaning | F3DEX2 command built |
|---|---|---|---|
| `1xxxxxxx` | 2 (+2) | triangle: w = 16 bits, indices (w>>10)&31, (w>>5)&31, w&31. If the next byte also has bit 7 set, a second triangle follows | `G_TRI1` 0x05 / `G_TRI2` 0x06 |
| `001xxxxx` | 2 | vertex load: w = 16 bits, first = (w>>5)&31, count = w&31 (0 = 32); takes the next `count` vertices of the LOD's list into cache slots `first..` | `G_VTX` 0x01 (segment 2 address) |
| `010xxxxx` | 2 | matrix: second byte n = bone; loads segment 1 + n*64 as MODELVIEW (load, no push) | `G_MTX` 0xDA380003 |
| `011vvvvv` | 5 | modify vertex: slot v gets texture coordinates s, t (two s16) | `G_MODIFYVTX` 0x02, where 0x14 = `G_MWO_POINT_ST` |

Each part becomes `{material (unpacked), display list}` ending in `G_ENDDL`. The 0x40 command only
appears in anim geometry (never in the 2215 simple models). 51,827 modify-vertex commands in all
re-use a cached vertex with other UVs. `build_mesh()` replays the stream through a 32-entry vertex
cache, which turns a modified slot into a new vertex.

Counts: 2705 models, 248,094 triangles at LOD 0, 47 models with 2..20 LODs, 1743 distinct texture
ids used (9..1837).

### Anim geometry extras

* **Skeleton (block 3)**, read by `CGameObjectInstance__BuildBoneMatrices` (func_0021CDAC.c:
  `MtxPool_Alloc(*block3)`, one matrix per bone) and `CGameObjectInstance__PoseBone`
  (func_0021B95C.c, recursive from bone 0). Bone b = set {0: children `{s32 4, s32 n, s32 id[n]}`,
  1: bone data, 112 B: 8 bounding-box corners (24 f32), f32 radius at +0x60, flags u8 at +0x65 (the
  pose code tests `& 0xC7` == 0xC2/0xC3/0xC6/0xC7 for the big-head style cheats)}. 1..115 bones.
  The bone's local transform (position + quaternion) is **not** in the geometry: it comes from the
  animation (track or the animation's default frame, `KeyLookup.frames`), so a rest pose needs part
  a5's decoder. `apply_pose(verts, matrices)` takes the PoseBone matrices (row vectors) and
  moves the bone-local vertices into model space. Until then the glTF of an anim model carries the
  bone-local positions plus a `_BONE` vertex attribute (the gSPMatrix index of each vertex).
* **Hot points (block 4)**: per slot (`count = number of LODs`, `obj->attachMask` bit i) a CUSet
  `{s32 0x14, s32 n, n x 20 B}`: 3 f32 (position) + 8 bytes with a bone index and flags,
  passed to `GameObject_EmitHotPoints` with the skeleton (muzzle/effect points).
* **Block 5**: per slot a CUSet of 8-byte records, almost always empty. Unknown.

## Verification

* Decoding: every one of the 2705 geometry blocks decompresses (CRC-checked by `rnc.py`) and parses; every
  LOD's vertex loads use up exactly its vertex list; every exported glTF's indices are in range.
* Rendered (textured with a preview decode of the CI4/CI8 textures, frame 0): object 1 is an egg-shaped orb with
  a glowing vein texture; 13 is a dark metallic piece; 19 is a yellow object with a black/white
  concentric-stripe disc; 1598 is a rock/cliff chunk with a grass top; 2222 is a round stone
  tower/arena with stairs, walls and grass patches. The textures sit where they should, the
  repeat and seams line up, so the UV formula above is right. Anim models (68) render as
  bone-local pieces, as expected without a pose.

## Open questions

* Geometry block 1 ("data", copied as is): empty in 2647 models, a 1-entry set in the rest. Who reads it?
* Object block 3 ("static events", 37 objects with 0x1C-byte records) and geometry block 5.
* `unk4` in the material (texture frame/palette selector? the runtime doubles it and ORs flag 0x40).
* Object type ids: only 246 distinct values over 2705 objects (id 7 x1249, 5 x367, 1 x266);
  `CScene__FindObjectType` uses them to find the special geometry headers (0xDAC -> object 65,
  0xDAD -> 64, 0xDB0 -> 63; 0xDAE and 0xDAF are absent), which copy the first material of LOD 0.
* The exact use of the colour bytes (colour vs normal) per material flag, which needs
  `Lighting_BuildLitVertices` and the combiner modes.

## For the other areas

* **a1 (romfs)**: the cartridge index is at 0xFCBFB4 (a direct ROM address, not an offset into the
  913-entry table); its 21 blocks cover 0xFCBFB4..0x1FF7AEC. Block 0 objects, 1 intelligence, 2 object types,
  3 texture sets, 4 particles, 5 levels, 6/7 persistent-data bit sections, 9 binaries, 0xA object
  types 2, 0xB swooshes, 0xC dynamic lights, 0xD curves, 0xE rumbles, 0xF rumble types,
  0x10 graphic sets, 0x11 graphic set types, 0x12 cinemas, 0x13 link group types (`CScene__Construct`).
* **a2 (textures)**: material texture id = cart block 3 index. Texture resource = set {0: info
  (u8 fmt: 0 CI8, 1 CI4, 2 RGBA32, 3 CI4 with IA16 palette, 4 RGBA16, 5 I16; [2]/[3] log2 W/H; [5]
  load mode), 1: frames, 2: palettes}. `preview_texture()` here decodes only CI4/CI8/RGBA16/32 for
  checking UVs; the real decoder is a2's.
* **a4 (levels)**: placed static objects and actors refer to objects by index
  (`CISet__GetROMAddress(objects, key, type)` in `CScene__DecompressGridSection`); level scenery
  is these object models.
* **a5 (animation)**: skeleton = geometry block 3 (above); an anim object's animation set is object
  block 2; vertex `_BONE` = matrix index = bone index in PoseBone order. `apply_pose()` takes your
  matrices.
