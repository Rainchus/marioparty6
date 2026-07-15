# Native matching wave 59: Player systems, Wipe update closure, and math helpers

Wave 59 recovers 47 additional target functions across three Board owners and
retains one measured M2S compiler-shape improvement. `board/player.c` gains 31
functions, `board/wipe.c` gains six, and `board/capevent.c` gains ten. No owner
is promoted: all four complete objects still diverge and remain
fallback-linked `C-not-yet-matched` owners.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target on the left and source on the right. Strict
  results retain relocation-symbol identity; data-value results use
  `functionRelocDiffs=data_value`.
- Compiler: the project-pinned MWCC and the per-owner flags in `configure.py`.
- Primary evidence: MP6 function boundaries, complete instruction streams,
  call targets, relocation values, initialized bytes, BSS ranges, and field
  offsets used by callers and callees.

No assembly, byte blobs, synthetic instruction forcing, or Matching claim is
introduced. Fallback-linked owners are not counted as decompiled source.

## `board/player.c`: turn, movement, collision, and effects

The target-proven `MBPLAYERWORK` remains `0x50`. Wave 59 names `masuMoveF` at
`0x0A` and `moveProc` at `0x48`, and recovers the distinct pointer lifetimes
that MWCC assigns around movement calls. The exact four-entry owner remains
`.bss 0x140`.

The movement-number, collision, metal, and Biri-Q object work is recovered as
packed bitfields at the exact byte offsets used by MP6. The named owner globals
now include `turnIntrF`, both metal colors, `playerColSnapF`, and the two
target-byte default colors `{128,190,140,255}` and `{100,50,130,255}`. Source
`.sbss 0x1C` covers the target's named prefix before its four-byte alignment
tail at `0x20`.

Coverage rises from 92 mapped/68 strict-exact functions to 123/97. Target and
source `.text` are `0xBEB0/0x4310`, at 33.953457% strict and 33.997707%
data-value; 103 mapped functions are data-value exact.

Twenty-nine new functions are strict-exact:

- turn/movement: `mbSingleTurnExec`, `PlayerMoveCall`, `PlayerMoveDestroy`,
  `ev_PlayerStartTurn`, `ev_PlayerEndTurn`, `mbPlayerMasuMoveTo`,
  `mbPlayerMasuMove`, `mbPlayerMasuMovePos`, `mbPlayerMasuMoveSpeed`, and
  `mbPlayerDiceMotExec`;
- movement-number/collision: `mbMoveNumCreate`, `mbMoveNumKill`,
  `mbMoveNumDispSet`, `mbev_PlayerColMasuSet`, `mbPlayerColCheck`,
  `PlayerColKill`, `mbev_PlayerColReserve`, `mbPlayerColSnapSet`,
  `mbPlayerColSnapPlayerSet`, `mbPlayerColSnapGet`, `mbPlayerColRestSet`, and
  `mbPlayerColFirstSet`;
- effects/spaces: `ResetMetalColor`, `mbPlayerMetalColorSet`,
  `PlayerBiriQFlashSet`, `PlayerBiriQEffectSet`, `mbPlayerPlusMasuExec`,
  `mbPlayerCapCoinMasuExec`, and `mbPlayerMinusMasuExec`.

`mbPlayerMoveExec` is target/source `0x60/0x60`, data-value exact, and
99.791664% strict because objdiff pairs target `lbl_802C31A4` with the
compiler-local source constant at the same value. `mbPlayerEffectSet` is
`0x80/0x80` at 99.843750%; its sole reported difference is the destination
identity of the otherwise identical `beq` into the still-partial local owner.

## `board/wipe.c`: update and dispatch closure

Six target functions are recovered with target-backed owner types:

| Function | Target/source | Data-value objdiff |
| --- | ---: | ---: |
| `WipeMasuMatHook` | `0x254/0x254` | 100% |
| `WipeSpecialDraw` | `0x110/0x110` | 100% |
| `WipeShapeUpdate` | `0x234/0x234` | 100% |
| `WipePaperVtxUpdate` | `0x278/0x278` | 100% |
| `WipeGridUpdate` | `0x370/0x370` | 100% |
| `WipeImageUpdate` | `0x20C/0x20C` | 100% |

`WipeSpecialDraw` is also strict-exact. The other five retain only
same-valued local constant-label ownership differences in strict mode, scoring
99.522730%-99.618324%.

MP6 offsets prove `WIPE_GRID_ENTRY` at `0x3C`, `WIPE_GRID_WORK` at `0x14AC`,
`WIPE_IMAGE_WORK` at `0x18`, `WIPE_PAPER_VTX` at `0x34`, and
`WIPE_PAPER_WORK` at `0xD34`. The eight-entry `fadeFunc` table is recovered
from the target relocations. Single-precision target loads prove the float-pi
paper geometry, while the final `atan2` angle conversion remains double.

Wipe reaches 29 mapped/16 strict-exact and 21 data-value-exact functions.
Target/source `.text` are `0x3664/0x1B34`, at 43.738870% strict and 43.845158%
data-value. Target/source `.bss 0x38` remains exact; source `.data 0x38`
contains both three-entry resource tables and the fade-function table. Four
target functions remain absent: `WipePaperUpdate`, `WipeGridDraw`,
`WipePaperDraw`, and `WipeImageDraw`.

## `board/capevent.c`: ten exact-value numerical helpers

The angle-wrap/chase, Hermite scalar/vector, quadratic Bezier, slope, and
normalized-vector helpers recover exactly `0x1018` additional target text.
All ten functions have equal target/source sizes and are 100% under data-value
objdiff. In particular, the scalar Bezier targets prove the ABI
`(t, a, b, c)` from `f1` through `f4`; correcting that order also preserves
the already-identical inlined vector paths.

Capevent rises from 28 to 38 mapped functions. All 38 are data-value exact and
28 are strict-exact. The 100 remaining strict diff rows in the ten new helpers
are same-valued local literal relocation ownership, with no instruction or
register mismatch. Target/source `.text` are `0x14EFC/0x11E0`, at 5.312748%
strict and 5.336070% data-value. The other 199 target functions remain absent.

## `dolphin/mic/m2s.c`: retained measured improvement

Reusing the public `samples` parameter for the `MICGetSamples` result moves the
result lifetime from `r31` to the target's `r30`. `M2SGetSamples` remains
`0x118/0x118` and improves from 88.700000% to 88.842860%, reducing its diff
rows from 12 to 10. Whole-owner strict/data-value scores improve from
79.981224%/79.992960% to 79.992960%/80.004690%; target/source `.text` remain
`0xD50/0xD04`, strict-exact coverage remains 3/14, and relocations remain
143/149.

The walking-pointer `M2SClose` probe was rejected because it grew the source
from `0x94` to `0x98` and regressed 85.918920% to 83.216220%. Explicit narrow
shift assignments and the chained clear in `M2SGetSamplesLeft` were byte-
neutral and were not retained as progress.

## Gate

The final integrated gate is:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical;
- progress remains 9.17% code/31.33% data overall and 47.07% code/74.48% data
  in the DOL, with 301/895 and 292/396 Matching owners respectively.

The final build leaves `config/GP6E01/symbols.txt` unchanged in both staged
and unstaged diffs.
