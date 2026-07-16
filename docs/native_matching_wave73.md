# Native matching wave 73: Board display and effect flows

Wave 73 represents `0x484C` additional target text bytes in recovered C across
26 formerly absent Coin, Player, and Star functions. Seventeen functions
totaling `0x1178` target text bytes become exact under objdiff's required
`functionRelocDiffs=data_value` comparison. Seven smaller target-proven
closures in CapSelect, Config, Last5, and Shop are included in the same batch.
Every incomplete owner remains `NonMatching`; fallback-linked bytes are not
counted as decompiled owners.

## Authorities and compiler method

The MP6 target objects, instructions, relocations, symbols, allocation sizes,
and data accesses are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Board
framework, but every sibling body was reconciled against MP6 before retention.

The local `mwcc-decomp` oracle at
`094b96ca1df4a035b5f93c351f773306c0241f3f` supplied three concrete compiler
facts used in this wave:

- `CInline.c` requires the callee's packed body to exist before auto-inlining,
  so target definition order is a code-generation input;
- `StackFrame.c` assigns spilled locals by increasing size class and then
  declaration-list order;
- `Coloring.c` chooses the lowest available volatile register from the
  interference graph, so register cycles are symptoms of an earlier AST or
  lifetime difference rather than a reason to force a register.

These facts close the Shop wrapper ordering and bound the rejected Coin and
Player experiments. No inline assembly, byte body, padding construct, volatile
or register-forcing declaration, opaque storage substitute, or invented
semantic name was added.

## Coin display family

Ten formerly absent functions now represent `0x156C` target text bytes. The
recovered source establishes the `0x1C` `COINDISPMODEL`, its five-entry
allocation, the display work modes, sign/digit ownership, camera-facing model
update, timing curves, sounds, and object teardown.

Seven functions totaling `0x78C` are exact:

- `mbCoinDispCreate` (`0x208`);
- `mbCoinDispMasuCreate` (`0x3C`);
- `mbCoinDispCapsuleCreate` (`0x38`);
- `CoinDispOMExec` (`0xFC`);
- `CoinDispOn` (`0x144`);
- `CoinDispMain` (`0x214`);
- `CoinDispObjKill` (`0xBC`).

`CoinDispCreate`, `CoinDispObjUpdate`, and `CoinDispOff` remain honest WIP at
33.661793%, 88.301770%, and 97.255810%. Their differences originate in the
still-divergent earlier `mbCoinObj*` helper source and expansion shapes. Two
inline redeclarations were byte-neutral and one inline-definition experiment
regressed; all three probes were reverted.

## Player Metal and Biri-Q family

Seven formerly absent functions represent `0x1B00` target text bytes. MP6
proves the Metal/Biri-Q work fields through offset `0x08`, the particle
`vertexNo` member at offset `0x20`, the public `mbPlayerMetalSet(int, BOOL)`
ABI, and the by-value `GXColor` ABI of `mbObjMetalColorSet`. It also proves the
2,500-byte vertex-bin allocation, board assets, particle counts, layers,
fade tables, and camera offsets.

`mbPlayerMetalSet` is exact (`0x244`). The six larger effect bodies range from
69.756580% to 99.035090%. The two bin builders use a paired-single vector
multiply shape for which neither MP5 nor the current headers authenticate the
source expression; no transcription or speculative helper was retained.

## Star add and display family

Nine formerly absent functions represent `0x17E0` target text bytes. The
recovered source establishes the exact `0x60` `STARDISPWORK`, number assets
`DATA_board` 11 through 20, sign assets 21 and 22, the tag-team clamp through
symbol-authoritative `mbStatTeamMinValGet`, and target translation-unit order.

Two functions totaling `0x220` are exact:

- `mbStarAddAllProcExecV` (`0x108`);
- `mbStarAddAllProcExec` (`0x118`).

The target's absence of narrowing masks proves `StarDispObjUpdate` takes an
`int modelNo`, not the initial `u16` reconstruction. That correction improves
the whole owner from 69.527510% to 69.696500% and is retained. The other seven
new functions remain represented WIP at 79.719010%-97.912840%.

## Seven exact local closures

CapSelect closes both previously divergent represented functions (`0xDC`
bytes total). `CapSelectCapsuleGet` returns `int`: MP6 loads the word-sized
extra-capsule entry directly and sign-extends only the `s8` player-capsule
callee result. `mbCapSelectShrinkCheck` retains the target's explicit branch
rather than the branchless equality lowering.

Config closes `GWStorySingleCheck` (`0x2C`) with the target's direct
`return !GWPartyGet()` inline shape.

Last5 closes `Last5PlayerOrderGet` (`0x368`) with the authenticated
`!GWTeamFGet()` helper shape. The 40-coin event also recovers the target's
floating-point `(float)(i * 30) / coinNum` delay calculation and improves from
89.601070% to 98.781910%. Its sole target-only four-byte local is written once
and never read; without a source authority for its meaning, it is not invented.

Shop closes `mbev_ShopInit` (`0x34`), `mbev_ShopExInit` (`0x38`), and
`mbev_Shop` (`0xAC`). Target symbol order places `mbev_ShopExObjHookSet` after
the wrappers, preventing their auto-inline. The entry function's two distinct
post-allocation moves prove an untyped allocation result followed by the typed
`MBSHOPWORK *` assignment.

## Object evidence

All changed objects compile with the pinned MWCC configuration. Objdiff CLI
`3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/capselect.c` | `0x3460/0x330` | 6.085919% | 18/32 | 18 |
| `board/coin.c` | `0x4550/0x32C0` | 63.219566% | 44/52 | 16 |
| `board/config.c` | `0x63BC/0x560` | 5.389316% | 6/51 | 6 |
| `board/last5.c` | `0x2458/0x650` | 17.356405% | 2/10 | 1 |
| `board/player.c` | `0xBEB0/0x9F98` | 81.179940% | 160/165 | 133 |
| `board/shopevent.c` | `0x2FDC/0x16C` | 2.970944% | 6/13 | 6 |
| `board/star.c` | `0x5C00/0x43B0` | 69.696500% | 86/90 | 64 |

The final serialized full build and explicit DTK checksum both report
`137 files OK`; `cmp` confirms the rebuilt and original `main.dol` are
byte-identical. The final build leaves `config/GP6E01/symbols.txt` unchanged.
