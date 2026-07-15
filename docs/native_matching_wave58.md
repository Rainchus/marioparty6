# Native matching wave 58: Masu closure, Wipe lifecycle, and M2S exacts

Wave 58 completes source representation of all 119 target functions in
`board/masu.c`, recovers six target-backed special-wipe lifecycle paths, and
makes two more `dolphin/mic/m2s.c` functions byte-exact. All three owners
remain `NonMatching` and fallback-linked because their complete objects still
diverge. No Matching owner is claimed in this wave.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target on the left and source on the right. Strict
  mapped/exact counts use default relocation identity; data-value scores use
  `functionRelocDiffs=data_value` where stated.
- Compiler: the project-pinned compiler and per-owner flags from
  `configure.py`.
- Primary evidence: MP6 instructions, function boundaries, call targets,
  relocations, initialized data, BSS boundaries, and owner/caller field
  accesses.
- Corroborating source: Matching Mario Party 5 source is used only where the
  MP6 target independently proves the retained behavior and layout.

No assembly, byte blobs, synthetic helpers, guessed fields, or fallback-backed
Matching claim is introduced. Fallback-linked code is not counted as
decompiled source.

## `board/masu.c`: all 119 target functions represented

The seven formerly absent functions are recovered from the MP6 target:

- `MasuDraw` and `MasuNextDraw` reconstruct the base-space and capsule-face GX
  paths, including target fade, scale, texture, culling, and material state.
- `mbev_MasuMove`, `mbev_MasuCapStop`, and `mbev_MasuStop` close the party,
  story, tutorial, capsule, event, and space-stat transitions.
- `mbMasuPlayerColorSet` restores the target player-color table and status
  selection logic.
- `mbMasuPKinokoValueGet` restores the bounded route search, candidate
  selection, and `MBPLAYERWORK::masuNext` update.

The route search uses the target-proven `MASUFINDWORK`, `masuFindWork[MASU_MAX]`,
`masuFindResult`, and `masuFindVisit` owners. Its newly named result entry is
the two-byte `MASUPKINOKORESULT { u8 id; u8 step; }`. Matching
[MP5 `masu.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/game/board/masu.c)
authenticates the source families; MP6 instructions and data independently
prove every retained branch, table, offset, and bound.

Coverage rises from 112/119 represented functions to 119/119. The strict-exact
count remains 103/119; under data-value relocation comparison 110/119 are
exact. Target/source `.text` are `0x5DC8/0x5DD4`, with 96.772575% strict and
96.905030% data-value whole-owner scores. Target/source relocations are
1140/1142. The initialized and zero-initialized ownership is nearly closed:

- `.rodata 0x40/0x40`: 100%
- `.data 0x128/0x128`: 89.189186%
- `.bss 0x700/0x700`: 100%
- `.sdata 0x10/0x10`: 100%
- `.sbss 0x78/0x74`: 100% across the named overlap; target has an aligned tail
- `.sdata2 0x90/0xA0`: 94.736850%

| New function | Target/source | Data-value objdiff |
| --- | ---: | ---: |
| `MasuDraw` | `0xB54/0xBB4` | 92.086900% |
| `MasuNextDraw` | `0x3FC/0x3E8` | 96.662740% |
| `mbev_MasuMove` | `0x104/0xF8` | 87.553850% |
| `mbev_MasuCapStop` | `0x460/0x45C` | 86.425000% |
| `mbev_MasuStop` | `0x238/0x234` | 72.373240% |
| `mbMasuPlayerColorSet` | `0x260/0x258` | 92.559210% |
| `mbMasuPKinokoValueGet` | `0x7EC/0x7D0` | 95.621300% |

All previously represented functions retain their prior scores. The owner is
source-complete but still has sixteen strict-divergent functions, so it is not
flipped and remains real C-matching work.

## `board/wipe.c`: special-wipe lifecycle and ownership

Six target-backed lifecycle/state entry points are recovered:
`mbWipeSpecialInit`, `mbWipeSpecialClose`, `mbWipeSpecialKill`,
`mbWipeSpecialCreate`, `mbWipeSpecialFadeOutCreate`, and
`mbWipeSpecialFadeInCreate`. They prove the full semantic
`WIPE_SPECIAL_DATA` layout through offset `0x28` and size `0x2C`, the
three-entry `ANIMDATA *wipeImageAnim` bank, and two initialized three-entry
resource tables `{7, 8, 9}` and `{10, 12, 11}`.

Mapped coverage rises from 17 functions/15 strict-exact to 23/15. Target/source
`.text` are `0x3664/0xDA8`, at 18.934214% strict and 18.938524% data-value.
Target/source `.bss` are exact at `0x38`; source now materializes the proven
`.data 0x18`. All previously represented functions retain their prior scores.

| New function | Target/source | Data-value objdiff |
| --- | ---: | ---: |
| `mbWipeSpecialInit` | `0x1CC/0x224` | 64.121740% |
| `mbWipeSpecialClose` | `0xA4/0xA4` | 70.951220% |
| `mbWipeSpecialKill` | `0x74/0x94` | 34.137930% |
| `mbWipeSpecialCreate` | `0xC4/0xF4` | 43.428570% |
| `mbWipeSpecialFadeOutCreate` | `0xB4/0xCC` | 58.400000% |
| `mbWipeSpecialFadeInCreate` | `0xD4/0x104` | 47.886790% |

The special renderer/update closure remains absent: `WipeMasuMatHook`,
`WipeSpecialDraw`, the shape/grid/paper/image update paths, and the grid/paper/
image draw paths are not transcribed without proof. Typed declarations are
retained only where the recovered lifecycle calls those target functions.

## `dolphin/mic/m2s.c`: two exact public setters

MP6 and MP7 instruction identity proves that `M2SSetBuffer` and `M2SSetMode`
address `__M2SBlock[__M2SChannel].is_active` directly rather than retaining a
local control-block pointer. Both now compile byte-exact at target/source
`0x58/0x58`, including their five-instruction indexed address sequence.

Strict-exact coverage rises from 1/14 to 3/14. Target/source `.text` remain
`0xD50/0xD04`; the whole-owner strict score is 79.981224% and data-value score
is 79.992960%, up from 79.922540%. Target/source relocations remain 143/149,
and the other twelve functions do not regress. The complete owner still
diverges and remains fallback-linked.

## Gate

The three modified owners compile under their configured flags. The final
integrated build/container gate is:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- progress remains 9.17% code/31.33% data overall and 47.07% code/74.48% data
  in the DOL, with 301/895 and 292/396 Matching owners respectively

The final build leaves `config/GP6E01/symbols.txt` unchanged in both the
staged and unstaged diffs.
