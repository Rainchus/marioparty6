# Native matching wave 57: Board culling and easy owner ports

Wave 57 completes source representation of `board/math.c` and adds eight
authenticated Player/Coin functions. All three owners remain `NonMatching`
and fallback-linked because their complete objects still diverge. No Matching
owner is claimed in this wave.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target on the left and source on the right. Strict
  mapped/exact counts use default relocation identity; data-value scores use
  `functionRelocDiffs=data_value` where stated.
- Compiler: the project-pinned compiler and per-owner flags from
  `configure.py`.
- Primary evidence: MP6 instructions, function boundaries, direct-call sites,
  relocations, callers, HSF/HU3D field offsets, initialized data, and BSS
  boundaries.
- Corroborating source: Matching Mario Party 4/5 source is used only where the
  MP6 target independently proves the retained behavior and layout.

No assembly, byte blobs, synthetic helpers, guessed fields, or fallback-backed
Matching claim is introduced.

## `board/math.c`: all 56 target functions represented

Five target functions and their real storage owners are recovered:

- `mbRandMod` uses the target's 31-bit `frand` value, extends its low bit into
  bit 31, and returns the high word of the unsigned product with the modulus.
- `ObjectBBoxUpdate` scans the real HSF vertex bank and updates the six
  `HSF_OBJECT::mesh.mesh` min/max members.
- `ObjectCullUpdate` builds the eight bounding-box corners, transforms them
  through the camera/shadow frustum matrix, and applies
  `HSF_MATERIAL_DISPOFF` only after the target's near/X/Y plane tests.
- `ObjectCullHook` reconstructs the object matrix through the existing
  rotation/scale helpers, concatenates it with the parent matrix, and invokes
  the culling owner.
- `mbObjCullInit` walks `HSF_DATA::object`, installs the real
  `HSF_CONSTDATA::hook`, and applies `HU3D_ATTR_NOCULL` when no eligible object
  exists.

The MP6 accesses close through existing real structures: `HSF_OBJECT 0x144`,
`HSF_DATA::object` at `0x4C`, `objectNum` at `0x6C`, mesh min/max at
`0x64/0x70`, vertex at `0x108`, cenv count at `0x134`, and const-data hook at
`0x44`. Matching [MP4 `hsfdraw.c`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/hsfdraw.c)
and [MP5 `hsfdraw.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/game/hsfdraw.c)
authenticate the camera/shadow, min/max, tangent, and object-hook policies.
Target function order plus the retained direct call from `mbObjCullInit` proves
that `ObjectBBoxUpdate` must not be auto-inlined; the owner-local
`dont_inline` contract restores target size for the caller.

The two eight-entry `HuVecF` culling banks make source/target `.bss` exact at
`0xC0`; source `.sbss 0x4` covers the target's named value, whose owner section
is `0x8` after alignment. Coverage rises from 51/56 to 56/56 represented
functions, with the existing `mbMathClose` still the sole strict-exact body.
Target/source `.text` are `0x2A44/0x2F00`; strict whole score rises from
35.577080% to 47.086136% and data-value score from 35.699078% to 47.239555%.
All prior 51 function scores are unchanged.

| New function | Target/source | Data-value objdiff |
| --- | ---: | ---: |
| `mbRandMod` | `0x44/0x40` | 91.941180% |
| `ObjectCullUpdate` | `0x704/0x6E0` | 56.109130% |
| `ObjectBBoxUpdate` | `0xCC/0xC4` | 14.725491% |
| `ObjectCullHook` | `0x2D8/0x428` | 0% |
| `mbObjCullInit` | `0xC4/0xC4` | 75.714290% |

The low scores are recorded rather than hidden: the hook inherits codegen
differences from the already-divergent matrix-helper family, and the culling
loop remains structurally different. These are real remaining C-matching
problems, not grounds for assembly or promotion.

## `board/player.c`: lifecycle, rotation, material, and names

Matching [MP5 `player.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/game/board/player.c)
authenticates the six source families, while MP6 independently proves
`MBPLAYERWORK::rotateObj` at `0x14`, `matCopy` at `0x4C`, HSF material count at
`0x5C`, the `0x3C` material stride, the 14-entry character-message table, and
the 11 retained names plus three null slots.

`mbPlayerClose`, both rotation completion checks, `mbPlayerMatClone`,
`mbPlayerNameMesGet`, and `mbPlayerNameGet` raise coverage from 86 mapped/66
strict-exact to 92/68. The two rotation checks are byte-exact. Target/source
`.text` are `0xBEB0/0x3704` at 27.636677% strict and 27.680515% data-value.
Source now materializes `.rodata 0xA4`, `.data 0x14`, and `.sdata 0x3B` for the
target-backed name owners; `.bss 0x140` remains target-exact. Every previously
represented function retains its prior score.

| New function | Target/source | Strict objdiff |
| --- | ---: | ---: |
| `mbPlayerClose` | `0xC4/0xBC` | 95.591835% |
| `mbPlayerRotateCheck` | `0x28/0x28` | 100% |
| `mbPlayerRotateCheckAll` | `0x54/0x54` | 100% |
| `mbPlayerMatClone` | `0xA0/0x94` | 86.475000% |
| `mbPlayerNameMesGet` | `0xA8/0xA8` | 98.095240% |
| `mbPlayerNameGet` | `0xA8/0xA8` | 98.095240% |

## `board/coin.c`: display-object teardown

Matching [MP5 `coin.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/game/board/coin.c)
authenticates the lifecycle shape. MP6 proves the initialized five-entry
`coinDispOMObj` owner, its valid indices 1-4, and `COINDISPWORK::killF` in the
OM work byte at `0x4C`.

`mbCoinDispKill` and `mbCoinDispKillCheck` raise coverage from 22 mapped/5
strict-exact to 24/5. Target/source `.text` are `0x4550/0x1038` at 17.027052%
strict and 17.036068% data-value. Source `.data 0x14` now owns the target-backed
pointer bank; all prior 22 function scores are unchanged.

| New function | Target/source | Strict objdiff |
| --- | ---: | ---: |
| `mbCoinDispKill` | `0x68/0x7C` | 71.807690% |
| `mbCoinDispKillCheck` | `0x78/0x78` | 99.800000% |

## Rejected bounded probes

- `dolphin/mic/m2s.c`: TU-wide `scheduling off` was disproved. It reduced
  source `.text` from `0xD04` to `0xCFC`, regressed data-value score from
  79.922540% to 69.859150%, and reduced strict-exact coverage from 1/14 to
  0/14. It was removed.
- `dolphin/mic/mic.c`: authenticated early-return/explicit-else spellings for
  six public wrappers compiled byte-neutral. Whole score remained 54.690662%,
  source `.text` remained `0x1F40`, and exact coverage remained 0/41. The
  no-op rewrite was discarded.
- `board/audio.c`: MP7 binary parity confirms the target's four-local stack
  rotation in `mbMusBoardFadeOut`, but no authenticated source expression
  recreates the compiler-generated first temporary. No synthetic helper or
  register-forcing change was admitted.

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
