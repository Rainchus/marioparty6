# Native matching wave 78: Board event and panel recovery

Wave 78 closes 22 represented Board functions totaling `0x2468` target text
bytes across Opening, Coin, Star, Single, Config, and Shop Event. It also
replaces Effect's placeholder animation structs with the real engine types and
corrects a disproven Last Five local without claiming either change as an
exact closure. No owner is promoted; every touched owner remains
fallback-linked and excluded from decompiled-source totals.

## Authorities and recovery boundary

The MP6 target objects, complete function ranges, instructions, relocations,
stack slots, symbol order, and data references are authoritative. Mario Party
5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared
Board source families. Same-game exact functions were used only where they
isolate an MWCC source shape under the same flags.

The local `mwcc-decomp` checkout at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was consulted for automatic-object
ordering, live-range coalescing, nested-inline behavior, and stack allocation.
No assembly, byte body, register forcing, volatile match trick, padding local,
invented symbol, or compiler sweep was introduced.

## Opening closures

`OpeningCoinExec` (`0x220`) and `PlayerDropExec` (`0x2E4`) recover the target's
loop exits, chained zero assignments, and automatic-array declaration order.
The MP5-authenticated party-opening family, reconciled with MP6 stack and
relocation evidence, closes `ev_OpeningParty` at `0x9B8`. Its exact source
shape includes the reverse player scan with a comma initializer, explicit
Boolean assignment, scoped `boardNo` lifetimes, `GWPlayerGet` and
`GWTeamFGet`, the chained player-number assignment, member-wise position copy,
and the authenticated literal far plane.

Opening reaches 21/22 data-value-exact functions. Target/source `.text` are
`0x2B14/0x2CAC`, scoring 89.108086%. `ev_OpeningSingle` remains the sole
non-exact function at target/source `0x1094/0x122C`, scoring 71.697460%.

## Coin and Star closures

The target return width and local use make `CoinInit`'s model ID an `int`,
closing the complete `0x1FC` function without changing the shared public API.
MP5 authenticates the team getter and block-local clamp value in
`CoinAddAllProc`; MP6's live range proves that the team-player index and later
active-count loop reuse one `playerNum` scalar. The complete `0x368` function
is exact.

The same authenticated all-player source family and MP6 live-range evidence
close `StarAddAllProc` at `0x368`. The target's `extsh` authenticates this
owner's legacy signed-16-bit caller declaration of `mbGuideModelGet`, while
the already Matching Guide owner retains its public `int` definition. That
source-local return view and the target's `int` model-ID local close
`StarPauseHook` at `0x6C`; no unsupported shared-header widening is retained.

Coin reaches 23/52 exact functions at target/source `.text`
`0x4550/0x3F78`, scoring 81.157800%. Star reaches 68/90 at
`0x5C00/0x5A04`, scoring 93.670350%.

## Single-mode state

`mbSingleSaveInit` (`0x160`) follows the MP5-authenticated save-initialization
sequence with MP6's target-proven `(teamChar, mgPack, storyComDif)` inputs,
turn limit, and flag writes. Target references recover the owner-local
`u8 masuTypeNum` and `u8 masuType[5]`, making `SingleMasuTypeReset` exact at
`0x34`. The target's step-state load closes `mbSingleStepGet` at `0x54`.

The caller-side `mbSaveStoryInit` declaration and parameter names now carry
the same proven argument ownership; this interface clarification is
binary-neutral. Single now represents 14 of 58 target functions, 9 exactly.
Target/source `.text` are `0x98F0/0x4EC`, scoring 2.481099%. The large
single-mode driver remains absent.

## Pause panel state

Target field accesses recover the complete `0x5C` `PAUSE_PANEL_WORK`, its
typed animation IDs and vectors, and the owner-local panel and guide-kill
globals. Eight accessors are exact: `mbPausePanelPosSet` (`0x3C`),
`mbPausePanelPosGet` (`0x3C`), `mbPausePanelRotSet` (`0x5C`),
`mbPausePanelScaleSet` (`0x30`), `mbPausePanelScaleGet` (`0x28`),
`mbPausePanelBankSet` (`0x80`), `mbPausePanelBatsuSet` (`0x28`), and
`mbPausePanelFreezeGet` (`0x50`).

All 14 represented Config functions are exact. Target/source `.text` are
`0x63BC/0x784`, scoring 7.535642%; 37 target functions remain absent.

## Shop Event construction and state

Target offsets recover the complete `0xB0` `MBSHOPOMWORK`, while two fields
without closed semantics remain honestly named `unk18` and `unk1C`. Target
data and relocation order recover the named capsule/player, light, sprite,
window, and capsule-position tables plus the shop object bank and globals used
by the represented functions.

Four added functions are exact: `mbev_ShopCreate` (`0x450`),
`ev_ShopOMExec` (`0x1A8`), `ev_ShopOpenSet` (`0x248`), and
`ev_ShopMesGet` (`0x28`). Shop Event now represents 11 of 13 target functions,
10 exactly. Target/source `.text` are `0x2FDC/0xC7C`, scoring 26.054848%.

`mbev_ShopBackMotCreate` is retained as real WIP at target/source
`0x2AC/0x2A8`, scoring 98.865490%. Mario Party 7 commit
`f5ea780d07dab8188d3c02f7e9052360c591dc1b` supplies only binary and
`config/GP7E01/symbols.txt` evidence for `MBShopBackMotObjCreate` at
`0x8021311C` (`0x2EC`) in `orig/GP7E01/sys/main.dol`, not authentic C. It
cannot authenticate MP6's unexplained `0x90` frame or register move, so no
guessed local was added. `ev_Shop` and `ev_ShopSelect` remain absent.

## Effect typing and Last Five correction

MP5 authenticates the loop shape for `mbParticleUnkTotalGet`; MP6 field
offsets align it with the established `ANIMDATA -> ANIMBANK -> ANIMFRAME`
layout. The former opaque placeholder structs and `void *` API are removed.
This is source-quality recovery, not an exact closure: Effect remains 1/35
exact at target/source `.text` `0x3050/0x3074`, scoring 59.852844%.

Wave 77 associated a target zero stack store with `activeNum = 0` in
`ev_Last5Coin40`. The association is disproven: target `+0x8` is write-only,
while the real `playerPos` and `coinObjId` arrays begin at `+0xC` and `+0x18`.
The initializer emits a live `li r27, 0`, changes downstream allocation, and
does not create the target store. It was removed. The function returns to
target/source `0x2F0/0x2E8` at 98.781910%; Last5 whole `.text` is
`0x2458/0x650` at 17.356405%, with one of two represented functions exact.

## Bounded rejected probes

- A direct-index Dice initialization made MWCC fully unroll the loop to
  `0xD0`; a mixed-induction form regressed the score. Neither was retained.
- Opening callback-wrapper ordering was byte-neutral, while removing the
  inline qualifier emitted an extra helper. Both were reverted.
- Config panel unlock/slide candidates compiled to `0x74` against target
  `0xCC`; an authenticated higher optimization setting regressed further.
  Neither candidate was retained.
- Shared Coin/Star prototype widening was rejected in favor of owner-local,
  target-proven types.
- Effect's typed loop remains divergent, and Shop background-motion frame
  ownership remains unexplained. Neither is counted as exact recovery.

## Object and container evidence

Objdiff CLI `3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Represented/target | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/opening.c` | `0x2B14/0x2CAC` | 89.108086% | 22/22 | 21 |
| `board/coin.c` | `0x4550/0x3F78` | 81.157800% | 52/52 | 23 |
| `board/star.c` | `0x5C00/0x5A04` | 93.670350% | 90/90 | 68 |
| `board/single.c` | `0x98F0/0x4EC` | 2.481099% | 14/58 | 9 |
| `board/config.c` | `0x63BC/0x784` | 7.535642% | 14/51 | 14 |
| `board/shopevent.c` | `0x2FDC/0xC7C` | 26.054848% | 11/13 | 10 |
| `board/effect.c` | `0x3050/0x3074` | 59.852844% | 35/35 | 1 |
| `board/last5.c` | `0x2458/0x650` | 17.356405% | 2/10 | 1 |

No previously exact function regressed. The final serialized `rtk ninja -j1`
build and the independent `dtk shasum -q -c config/GP6E01/build.sha1` command
both report `137 files OK`; `cmp` confirms original and rebuilt `main.dol` are
byte-identical. No REL changed, and the final build leaves
`config/GP6E01/symbols.txt` unchanged.
