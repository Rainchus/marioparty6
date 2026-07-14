# Native matching wave 39

Wave 39 is a recovery-first board batch. It adds 87 source functions across
eight owners that previously had no source candidate. One batched compile and
one DTK/objdiff pass pair 86 of those functions with retained MP6 targets; 63
are byte-exact and cover `0x1A5C` target text bytes. The paired functions cover
`0x27C4` target text bytes in total.

No inline assembly, proof-only byte storage, register forcing, opaque padding,
or unknown/reserved structure members were added. Every incomplete owner stays
`NonMatching` and fallback-linked, so none of these owners is counted as fully
decompiled source.

## Recovery evidence

The primary evidence is the split MP6 target object: instruction widths,
loads/stores, relocations, indexed extents, call signatures, constants, and
section sizes. Two authenticated local sibling snapshots add source-shape or
binary-parity evidence where applicable:

- Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`:
  Matching `src/game/board/capsule.c` authenticates the capsule-object wrapper
  family; Matching `src/game/board/pauseoption.c` authenticates the complete
  framebuffer-copy draw shape; `src/game/board/scroll.c` authenticates the map
  lifecycle/camera shape; and `src/game/board/mgcircuit.c::GetHermiteSpeed`
  authenticates the Hermite-vector magnitude helper used by the opening owner.
- Mario Party 7 commit `f5ea780d07dab8188d3c02f7e9052360c591dc1b`:
  the cap-select target has the same small result/type/callback/map-object
  function shapes and provides the larger sibling array extents. MP6 target
  instructions remain authoritative for the four-player and 16-object bounds.

The recovered layouts are semantic, not placeholders:

- `CAPSULE_OBJ_COLOR` is `0x2C`: three `s16` state/model fields, a byte layer,
  and position/rotation/scale vectors; MP6 indexes 128 entries in the exact
  `0x1600` BSS owner.
- Capsule state includes `int capsuleUseEffMode[4]`, `HuVecF
  capsuleUseEffPos[4]`, `s16 capsuleNum[33][2]`, and the real throw-process
  pointer.
- Cap-select state uses four-entry result/model/extra arrays, four shrink
  process pointers, and a 16-entry `OMOBJ *` map-capsule table.
- `LAST5COINWORK` is the target-proven caller-work overlay `{ s16 delay; float
  velocity; }` on the public `MBCOINOBJ`; there is no synthetic tail.
- `MBSHOPWORK` is exactly two words, `playerNo` and `shopNo`.
- `MBSNPCSAVEWORK` is exactly three bytes: flags, space ID, and missed-effect
  count. The `0x298` runtime object remains intentionally incomplete.
- Scroll/map state uses `HSF_FACE *`, `HU3D_MODELID`, `ANIMDATA *`, `HuVecF`,
  and typed map/scroll/star-finder callbacks.

## Batched object results

All comparisons use objdiff v3.7.2 with target on the left, source on the
right, and `functionRelocDiffs=data_value`.

| Owner | Target/source `.text` | Mapped/exact functions | Evidence retained |
| --- | ---: | ---: | --- |
| `board/capselect.c` | `0x3460/0x32C` | 18/16 | Result set/get/reset, story/type state, capsule lookup/count, six authentic empty callbacks, and map-object lifecycle/display. `mbCapSelectShrinkCheck` is 68.333336%; `CapSelectCapsuleGet` is 97.558136%. |
| `board/capsule.c` | `0x146CC/0x1130` | 29/18 | Effect state, packed space/player helpers, capsule counters, and the capsule-color wrapper family. The compiler-retained sibling search helper has no named target pairing. |
| `board/config.c` | `0x63BC/0x560` | 5/4 | `mbPauseDispCopyCreate`, `mbPauseDispCopyKill`, `PauseDispCopyDraw`, and `mbConfigPadDisableSet` are exact; `GWStorySingleCheck` is 72.272730%. |
| `board/last5.c` | `0x2458/0x614` | 2/0 | `Last5PlayerOrderGet` is 99.266050%; `ev_Last5Coin40` is 89.601070%. Together they recover `0x658` target bytes with closed semantics. |
| `board/opening.c` | `0x2B14/0x3D4` | 14/12 | Curve, pad-delay, view/hook/guide accessors, and guide/camera restore paths are exact. The two process wrappers are 82.722220% and 94.500000%. |
| `board/scroll.c` | `0x3E4C/0x27C` | 9/7 | Close, star-space, map-view lifecycle, and hook setters are exact. `ScrollKill` is 80.208336%; `mbMapCameraSet` is 87.692310%. |
| `board/shopevent.c` | `0x2FDC/0x160` | 6/3 | Enable, object-hook, and back-create wrappers are exact; the remaining wrappers are 92.395350%-95.357140%. |
| `board/snpc.c` | `0x9594/0x44` | 3/3 | Init, space getter, and authentic empty star callback are exact. |

The objdiff artifacts are retained under `build/GP6E01/` as
`wave39_board_<owner>.json`.

## Deliberate boundaries

The wave stops at the first unresolved semantic dependency. In particular:

- `mbCapObjColorCreate` needs a still-unidentified capsule-data field at
  offset `0x1C`.
- `mbCapMasuObjCreate` needs the unresolved remainder of its `0x28` work.
- Last-5 roulette work contains unproven fields.
- Scroll collision construction and map rendering require the collision
  deduplication and sprite-state closures.
- The special-NPC runtime object and the remaining opening, shop, config,
  capsule-select, and capsule event state machines are not closed.

Those functions were omitted rather than represented with byte arrays,
padding, guessed fields, or assembly.

## Full gate

- `ninja -j1`: `137 files OK`.
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`: `137 files OK`.
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical.

DTK remains at 292/895 Matching owners overall and 283/396 in the DOL. The
eight recovered owners move from `NO-SOURCE` to `SRC-DIVERGES`; their
configuration remains `NonMatching` until each full owner passes the object,
relocation, linked-range, and container gates.
