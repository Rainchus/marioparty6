# Native matching wave 60: Board easy-port closure and MIC compiler recovery

Wave 60 batches the instruction-proven easy ports across three Board owners and
the two MIC owners. It adds 45 exact Capevent functions, represents the final
four Wipe target functions, advances Player by six mapped functions, and proves
the MIC library's original compiler as GC/1.2.5n. No owner is promoted: every
complete object still diverges and remains fallback-linked
`C-not-yet-matched` work.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target object on the left and source object on the
  right. Strict results retain relocation-symbol identity; the data-value pass
  ignores relocation-owner identity while retaining instruction operands and
  data values.
- Primary evidence: complete MP6 instructions and CFGs, function boundaries,
  calls, relocations, initialized bytes, BSS spans, and cross-function field
  use.
- `repos/mwcc-decomp` was inspected read-only. It does not contain the MIC or
  M2S owner source, so it was not treated as a donor.
- `repos/gc-mic/mic.c` corroborates MIC behavior, but MP6 instructions and
  compiler/object comparisons remain authoritative.

No assembly, byte blobs, fake padding, or forced instruction encodings are
introduced. Fallback-linked code is not counted as decompiled source.

## `board/capevent.c`: 45 strict-exact functions

Capevent rises from 38 mapped/28 strict-exact/38 data-value-exact functions to
83/73/83. Every Wave 60 addition is strict-exact. Target/source `.text` are
`0x14EFC/0x2068`, at 9.650637% strict and 9.673959% data-value.

The recovered groups are:

- event/process and player helpers: `mbev_CapCircuitCallKettou`,
  `mbev_CapRandomBonusCoin`, `mbev_CapBonusCoinCheck`, `mbev_CapNull`, the
  player-move object init/close/kill/check/setter family, `mbev_CapCoinAdd`,
  `mbev_CapPlayerSquishSet`, `mbev_CapCullPlayerCheck`,
  `mbev_CapPlayerMasuNumGet`, `mbev_CapPlayerNoSearch`,
  `mbev_CapPlayerComSelGet`, `mbev_CapChoiceSet`, `mbev_CapVecChase`, and
  `mbev_CapPlayerRotate`;
- exact effect-bank teardown/getters: explode, boost, snow, glow, ring, coin,
  coin-man, star-man, cap-lose, ray, and space-hit;
- four exact-stride active-entry counters for the `0x60` coin, `0x3C`
  coin-man, `0x3C` star-man, and `0x34` cap-lose records;
- `mbev_CapEffCoinMaxYSet`, `mbev_CapEffCoinGlowSet`,
  `mbev_CapEffColorSet`, `ev_CapBiriQMetalShock`, its destructor,
  `ev_CapEffOpenKill`, and `ev_CapBonusCoinKill`.

The instructions prove the `[8]` object banks, `[4]` player/process banks,
work offsets and record strides. The target `.data` bytes recover the seven
`GXColor` random-color entries exactly. Declaration order was retained only
where target register allocation proved it; unsupported Biri-Q flag naming was
rejected instead of being guessed. The other 154 target functions remain
absent.

## `board/wipe.c`: all target functions represented

The final four functions are recovered as real C:

| Function | Target/source size | Data-value result |
| --- | ---: | ---: |
| `WipePaperUpdate` | `0x5B4/0x5B4` | 100% |
| `WipeGridDraw` | `0x748/0x748` | 100% |
| `WipePaperDraw` | `0x888/0x888` | 100% |
| `WipeImageDraw` | `0x68C/0x68C` | 100% |

This recovers the actual two-position/two-UV `WIPE_PAPER_VTX`, paper start
directions, screen corners, light position/direction/colors, geometric edge
intersection and bend update, and the GX grid/paper/image render paths.
Target/source `.data` are exactly `0xA0/0xA0`; `.bss` is exactly `0x38/0x38`.

All 33 target functions now pair. Sixteen are strict-exact and 25 are
data-value exact. Target/source `.text` are `0x3664/0x3744`, at 95.119220%
strict and 95.441540% data-value. Eight represented functions remain genuinely
different after the data-value comparison, so source completeness does not
justify a Matching flip.

## `board/player.c`: turn, view, effects, names, and lifetimes

Coverage rises from 123 mapped/97 strict-exact/103 data-value-exact functions
to 129/105/116. Target/source `.text` are `0xBEB0/0x484C`, at 37.015240%
strict and 37.092266% data-value. No previously exact function regresses.

New source includes `mbTurnExec`, `PlayerViewSet`, `PlayerMetalKill`,
`PlayerBiriQKill`, `mbPlayerDiceTypeGet`, and `mbPlayerTagNameMesGet`. The
target data proves the 110-byte triangular tag-ID table. `mbPlayerClose`,
`mbPlayerModelIDGet`, `mbPlayerAttrSet`, and `mbPlayerAttrReset` are corrected
to strict exactness; the motion set/shift/idle paths are data-value exact.
`mbPlayerTagNameMesGet` and `mbPlayerDiceTypeGet` are data-value exact, while
the target-proven allocation-size lifetime improves `mbPlayerMatClone` to
94.100000% data-value.

Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`
authenticates `include/game/gamework.h`'s single `GWPlayerGet` inline and its
use in `src/game/board/player.c`. MP6 register allocation independently proves
the same shape in the recovered paths. Thirty-six target functions remain
unpaired.

## `dolphin/mic`: GC/1.2.5n recovered

The M2S baseline provides a decisive compiler fingerprint:

| MWCC version | Pre-fix M2S strict score |
| --- | ---: |
| GC/1.2.5 | 95.978874% |
| GC/1.2.5e | 91.988266% |
| **GC/1.2.5n** | **98.561035%** |
| GC/1.3 | 80.164320% |
| old project assignment, GC/2.6 | 80.004690% data-value |

GC/1.2.5n alone produces exact target/source M2S `.text 0xD50` and immediately
makes 12 of 14 functions exact. The assignment is therefore recorded on the
`sdk_mic` library rather than forced function by function.

With target-CFG source recovery, `mic.c` reaches 20/41 strict-exact functions.
The new exact set covers `MICProbeEx`, the ring/sample/button/device getters,
current/index/sample APIs, active/attached checks, unmount, callback, sync,
alarm/EXI handlers, and control-block release. Target/source `.text` are
`0x2020/0x1F10`, at 85.376945% strict and 85.413420% data-value, up from
54.678500%/54.690662% and 0/41 exact under GC/2.6. `.data 0x40` and
`.bss 0x150` compare at 100%; source `.sbss 0x4` covers the named target value
before its four-byte alignment tail.

M2S reaches 13/14 strict-exact functions and exact 143/143 relocations.
`M2SSetPrerecordSamples` is newly exact. `M2SAdvanceBuffer` is the only
residual at 95.037970%, with the target-proven bounded debug-buffer copy and
lifetimes retained. Target/source `.text` are both `0xD50`, at 99.539910% in
both comparison modes. Source `.bss 0x25C` differs from target `0x260` only by
the four-byte alignment tail.

## Gate

The final integrated gate is:

- `ninja -j1`: `137 files OK`;
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`;
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical;
- progress remains 9.17% code/31.33% data overall and 47.07% code/74.48% data
  in the DOL, with 301/895 and 292/396 Matching owners respectively.

The final build leaves `config/GP6E01/symbols.txt` unchanged. No Matching flag
is changed in this wave.
