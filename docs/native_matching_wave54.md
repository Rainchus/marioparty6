# Native matching wave 54: Board opening/tutorial and MIC semantics

Wave 54 advances four fallback-linked owners without relaxing the Matching
gate. MP6 instructions, relocations, and initialized sections are the primary
evidence. Fully Matching Mario Party 5 source at commit `e246f9d` corroborates
the opening flow and the `GW_PLAYER::playerNo` field, but MP5 is not used in
place of MP6 target proof.

## `board/tutorial.c`

The owner grows from 10 mapped/exact functions to 40 mapped and 34 exact
functions. Target/source `.text` are `0x2C34/0x139C`. The 24 new exact
functions cover:

- call/wait/message/turn/guide/capsule dispatch;
- `mbTutorialInit`, camera-by-space setup, sprite create/display-in, and all
  sprite/group/model close loops;
- model and guide creation/cleanup;
- the window timing initializer/updater and public wait/close paths.

The source now owns the real `s16 tutorialSprId[16]`,
`s16 tutorialSprGrpId[16]`, `s16 tutorialMdlId[32]`, `0x14`
`TUTORIALCALLWORK`, and `0x80` `TUTORIALWINDATA[32]`. Those definitions account
for the complete source BSS `0x114`; the target's remaining `0x2C` belongs to
the unrecovered minigame-call state. Chained narrow/wide assignments reproduce
the target `extsb` and store order in `TutorialWinInit`, `TutorialWinUpdate`,
and `mbTutorialWinClose`; changing public window IDs from `s16` to `int`
recovers the target coercions at the underlying `s16` window API boundary.

Six represented functions remain non-exact and therefore do not justify an
owner flip:

| Function | Target/source | Objdiff |
| --- | ---: | ---: |
| `mbTutorialViewSet` | `0xA4/0xA4` | 97.073170% |
| `mbTutorialSprKill` | `0x90/0x84` | 90.638885% |
| `mbTutorialSprDispOff` | `0x110/0x104` | 95.279410% |
| `mbTutorialSprGrpSet` | `0x5C/0x58` | 95.478264% |
| `mbTutorialSprGrpKill` | `0x90/0x84` | 90.638885% |
| `mbTutorialModelKill` | `0x90/0x84` | 89.250000% |

The watch/create path, guide-message table, and minigame-call state machine
remain absent rather than being filled with guessed storage or control flow.

## `board/opening.c`

The source grows from 14 mapped/12 exact functions to 20 mapped/15 exact.
Target/source `.text` are `0x2B14/0x1838`. The newly represented closure is
the complete party event plus its coin, player-drop, and order helpers, and
the single-event destructor and particle hook. It recovers the `0x3C`
`OPENINGCOINWORK`, process/hook/guide/effect state, camera and guide tables,
and the target meaning of the two-bit `GW_PLAYER` field at offset `0x02`.

[Matching MP5 `gamework.h`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/include/game/gamework.h)
names the same field `playerNo`, and
[Matching MP5 `opening.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/game/board/opening.c)
uses it in the same dice-order handoff. MP6 stores and later consumes the field
the same way. This replaces the unsupported `circuitOrder` name.

Initialized-section proof is strong: `.rodata 0x38`, `.data 0x1C0`, and
`.sbss 0x38` match exactly. The two recovered `-28.0f` vector components are
verified by the only two prior `.data` byte differences. Source BSS is
`0x994`; the target's `0x998` is the same semantic state followed by its named
four-byte split tail. Five large bodies remain non-exact:

| Function | Target/source | Objdiff |
| --- | ---: | ---: |
| `ev_OpeningParty` | `0x9B8/0x9CC` | 95.736336% |
| `OpeningCoinExec` | `0x220/0x22C` | 96.852940% |
| `PlayerDropExec` | `0x2E4/0x2F0` | 96.708110% |
| `PlayerOrderSet` | `0x1E0/0x1E8` | 95.958336% |
| `OpeningSingleEffHook` | `0x390/0x390` | 97.197365% |

`mbev_OpeningSingle` and the `0x1094` `ev_OpeningSingle` remain together on
the fallback side. The event's adaptive curve-length and Newton camera
traversal has not yet passed a source-shape comparison, so no placeholder or
partial body is admitted.

## Dolphin MIC and EXI evidence

`dolphin/mic/mic.c` now types four byte/sample ring fields as signed because
the target uses `divw`, `srawi`, and `cmpw`, not the baseline `divwu`, `srwi`,
and `cmplw`. Arithmetic `>> 1` restores the target sample-index operation.
Target/source `.text` remain `0x2020/0x1F40`; the data-value objdiff score is
54.690662%, with 0/41 exact functions and 299/291 relocations. The changed
instruction in `__MICTxHandler` is target-exact even though whole-function
alignment chooses a worse pairing path. A signed timeout and `/ 2` expression
were compiled, measured, and reverted because they emitted extra signed
rounding/extension code.

`dolphin/mic/m2s.c` now copies exactly 44 signed calibration samples in both
directions: `44 * sizeof(s16) == 0x58`, matching both target immediates rather
than the `0xB0` backing-array sizes. Target/source `.text` remain
`0xD50/0xD0C`; the data-value score is 78.733570%, with 1/14 exact functions
and 143/149 relocations. A cached debug-index probe grew and regressed the
function and was removed.

EXIBios was also tested once with owner-local GC/2.6 because its target
metadata reports `2.4.7.1`. The emitted source `.text` expanded from `0x2100`
to `0x3BA0`, relocations rose from 219 to 369, eleven functions lost their
pairing, and no function became exact. Metadata parity is therefore not
compiler proof; the override was reverted.

## Gate

All object comparisons use objdiff v3.7.3 with target on the left, source on
the right, and `functionRelocDiffs=data_value`. DTK remains pinned to 0.9.2 at
`4d039140f2d2ed80572b1949b76a5ff9b3094e06`.

No owner is flipped in this wave. The final regression gate passes:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- progress remains 9.12% code/31.30% data overall and 46.76% code/74.41% data
  in the DOL, with 300/895 and 291/396 Matching owners respectively
