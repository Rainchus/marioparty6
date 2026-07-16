# Native matching wave 74: Board lifecycle closures

Wave 74 represents the final `0x4424` target text bytes that were absent from
the Coin, Player, and Star owners. All 307 target functions across those three
owners now have compiled source counterparts. Four existing `mgcall` residues
also close exactly. Eight functions totaling `0x81C` target text bytes become
exact under objdiff's required `functionRelocDiffs=data_value` comparison.
Every incomplete owner remains `NonMatching`; fallback-linked owner bytes are
not counted as decompiled source.

## Authorities and recovery boundary

The MP6 target objects, instructions, relocations, symbols, data accesses, and
stack offsets are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Board
source family. Its bodies were used only where MP6 retained the same control
flow or data contract.

The local `mwcc-decomp` checkout at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was used to interpret remaining
inline, stack-slot, and register-allocation differences. No inline assembly,
byte body, match-only padding, volatile or register-forcing declaration, or
opaque work buffer was introduced. No compiler sweep was used to turn a
source-shape uncertainty into a score-only transcription.

## Coin award flows

The final eight Coin functions represent `0xDD4` target text bytes. They
recover the per-player award loop, display/no-display variants, 0..999 clamp,
sound and delay rules, team minimum calculation, four-player distribution,
and the vector and scalar public entry points. Target integer operations prove
that `mbStatTeamMinValGet` receives an `int` value; the former `s16` parameter
inserted a narrowing operation absent from MP6.

Three Coin functions totaling `0x26C` close exactly in this batch:

- `mbCoinDispKillCheck` (`0x78`), after restoring the target's
  `no >= GW_PLAYER_MAX + 1` boundary shape;
- `mbCoinAddAllProcExecV` (`0x198`);
- `mbCoinAddAllExec` (`0x5C`).

The six non-exact newly represented award bodies remain honest WIP. In
particular, `mbCoinAddExec` has the correct public behavior but does not yet
reproduce the target's inlined callee body, so its source is much smaller and
is not reported as recovered byte-identical code.

## Player initialization and turn loop

The final five Player functions represent `0x1E38` target text bytes. The
source recovers initialization, character model and motion tables, the
target-access-proven collision work fields, turn-state transitions, dice and
capsule dispatch, path movement, gate/branch events, and space-coin handling.
`MasuCoinExec` is exact (`0x42C`).

MP5 `src/game/board/player.c` authenticates the retained configuration-copy
shape, including the literal assignments back to `GwPlayerConf` and the
temporary used to copy the group value. These statements are sibling source,
not ghost operations invented from a register diff. MP6 separately proves a
party-path `_CheckFlag(FLAG_BOARD_TUTORIAL)` call at DOL
`0x801534D4..0x801534E8`: the branch rejoins without consuming `r3`. The call
is retained because the target executes it; it is not inferred from a desired
score.

The other four final Player bodies remain WIP at 89.704865%-98.315940%.
Their remaining differences are code-generation shape and lifetime residue;
all five target bodies now have real compiled C counterparts.

## Star and Ztar event loops

The final four Star functions represent `0x1818` target text bytes. They
recover Star and Ztar purchase/event flow, guide model and motion ownership,
map-view processing, message-base arithmetic, the event-space list, and the
model/effect transition sequence. Target stack offsets establish an
`s16 list[12]`; target consumers and relocation order establish the retained
guide globals and motion tables.

The Star/Ztar creation paths use their `HuVecF *pos` parameters directly, as
authenticated by MP5. Reinterpreting the address of that pointer as vector
data would improve stack shape while changing runtime behavior, so that probe
is rejected and is not present in the retained source.

No final Star body is exact yet. `mbStarMapViewProcExec` reaches 99.929080%
with only its frame-size prologue/epilogue residue; `ev_StarMasuRun` reaches
99.019270%. The two larger acquisition flows remain represented WIP rather
than being padded or transcribed.

## Minigame-call boundary closures

Four already represented `board/mgcall.c` functions totaling `0x184` become
exact:

- `mbMgCallInit` (`0xE8`): target relocation order places the 2-vs-2 status
  positions before the 1-vs-3 positions;
- `mbMgRouletteFocusKill` (`0x40`): MP5's public display setter authenticates
  the recovered `dispF` work field;
- `mbMgCallDataClose` (`0x34`): the target's unsigned close guard proves the
  directory handle is `u32`;
- `MgCallBattleMesGet` (`0x28`): the target branch proves the two-return
  `curTime` source shape.

All 12 currently represented `mgcall` functions are now exact (`0x3E4`), but
26 target functions remain absent. The owner therefore stays `NonMatching`.

## Object evidence

All four changed objects compile with the pinned MWCC configuration. Objdiff
CLI `3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/coin.c` | `0x4550/0x3F7C` | 80.933500% | 52/52 | 19 |
| `board/player.c` | `0xBEB0/0xBDC0` | 96.342760% | 165/165 | 134 |
| `board/star.c` | `0x5C00/0x5A08` | 93.603770% | 90/90 | 64 |
| `board/mgcall.c` | `0x75FC/0x3E4` | 3.297576% | 12/38 | 12 |

The final serialized full build and explicit DTK checksum both report
`137 files OK`; `cmp` confirms the rebuilt and original `main.dol` are
byte-identical. No REL changed. The final build leaves
`config/GP6E01/symbols.txt` unchanged.
