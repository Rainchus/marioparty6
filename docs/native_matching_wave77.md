# Native matching wave 77: Board source-shape closures

Wave 77 closes eight represented Board functions totaling `0x133C` target
text bytes. The exact closures recover control-flow form, inline ownership,
local-object lifetime, declaration order, expression kind, and explicit
return shape. No owner is promoted: all five owners retain represented or
absent work, and fallback-linked bytes remain excluded from decompiled-source
totals.

Wave 78 corrected the Last Five initialization claim originally recorded in
this note. The exact Wave 77 closures and their `0x133C` byte total are
unaffected; the non-exact Last Five candidate is corrected below.

## Authorities and recovery boundary

The MP6 target objects, complete function ranges, instructions, relocations,
stack slots, and control-flow edges are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Board
framework and getter source family. Same-game exact functions were used where
they isolate an MWCC AST or condition spelling under identical flags.

The local `mwcc-decomp` checkout at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was used for stack allocation,
graph coloring, compound-assignment lowering, and inline-expansion evidence.
No assembly, byte body, register forcing, volatile match trick, padding local,
invented symbol, or compiler sweep was introduced.

## Opening closures

`PlayerOrderSet` now uses the established `GWTeamFGet` and `mbPlayerGrpGet`
inline accessors. MP5 authenticates the getter family, while MP6 proves the
condition form and the temporary lifetime created by the group accessor. The
complete `0x1E0` function is exact.

The target enters `mbev_OpeningSingle`'s body before its first `endF` test and
checks the flag only at the back edge. Restoring the real `do ... while`
construct removes the source's sole extra branch and makes the target/source
range `0x27C/0x27C` exact.

`OpeningSingleEffHook` proves the automatic-object layout `colorTbl` before
`dir` and loads each `dir` component before the corresponding `guideCurPos`
component. Restoring the declaration order and the commutative expression
order removes every stack and operand difference. The function is exact at
`0x390`.

Opening reaches 18/22 data-value-exact functions. Target/source `.text` are
`0x2B14/0x2CD8`, scoring 87.929634%.

## Player closures

`mbMoveNumCreateColor` previously declared separate normalized and camera
vectors even though the target uses one `HuVecF` stack object for both
lifetimes. Reusing the established `posNorm` object restores the target slot
map and makes the complete `0x27C` function exact.

The teardown in `PlayerBiriQOMExec` uses a truthy data-pointer test and an
explicit return after `omDelObjEx`. Same-translation-unit controls distinguish
the truthy test from `!= NULL`, and the target branch proves the return edge.
The function is exact at `0x474`.

Player reaches 139/165 data-value-exact functions. Target/source `.text` remain
`0xBEB0/0xBDC0`, scoring 96.373980%.

## Coin closures

The target alpha update in `CoinEffHook` matches the exact same-TU
`DiceInEffHook` compound-assignment sequence. Restoring `color.a *= 0.8f`
recovers MWCC's compound-assignment AST and makes the `0x1AC` function exact.

`mbStatTeamMinValGet` proves distinct discovery and clamp loop counters but a
single player/swap lifetime. Removing the false swap temporary, retaining a
separate clamp counter, and reusing `player` restores the target allocation
without forcing registers. The complete `0x1B8` function is exact.

Coin reaches 21/52 data-value-exact functions. Target/source `.text` remain
`0x4550/0x3F7C`, scoring 81.022545%.

## Star material hook

`StarMatHook`'s remaining differences were entirely automatic-object slots.
The target proves the four-byte and twelve-byte object order; MWCC's stack
allocator explains the declaration-list traversal. Moving `colorNew` and
`lightDir` ahead of the output locals while preserving their initializer
execution order makes the complete `0xFC` function exact.

Star reaches 66/90 data-value-exact functions. Target/source `.text` remain
`0x5C00/0x5A08`, scoring 93.608350%.

## Last Five correction (Wave 78)

The zero word at target stack offset `+0x8` is write-only; the actual
`playerPos` and `coinObjId` arrays begin at `+0xC` and `+0x18`. Initializing
`activeNum` emits a live `li r27, 0`, changes the downstream register
allocation, and does not produce the target's write-only stack word. The
same-game Dice routine therefore authenticates a related source family, but
does not authenticate that local for this function.

The unsupported initialization was removed. `ev_Last5Coin40` returns to
target/source `0x2F0/0x2E8` at 98.781910%. Last5 target/source whole `.text`
are `0x2458/0x650`, scoring 17.356405%; one of two represented functions
remains exact. The unexplained write-only slot and downstream allocation
residue remain open rather than being assigned a guessed C local.

## Bounded rejected probes

The batch measured and rejected several tempting but unauthenticated or
regressing shapes:

- Restoring a fourth apparent getter expansion in `mbPlayerMoveMain` grew the
  source by four bytes and regressed 99.251440% to 98.109825%. It was reverted.
- Removing explicit `inline` from the two Star space helpers was byte-neutral;
  `ev_StarMasuRun` remained 99.019270%. Sibling resemblance without target
  movement did not justify retaining the change.
- Direct-index initialization in `mbDiceInit` triggered complete loop
  unrolling, producing target/source `0x74/0xD0` at 0%. The candidate was
  discarded; the original pointer-loop residue remains.
- `CoinInit`'s six sign-extension differences belong to the visible
  `mbObjCreate` return prototype. Body casts would disguise shared-header
  ownership and were not added.
- `StarPauseHook` and the closest Effect residues remain signature/header or
  ambiguous condition-shape questions. No compensating casts or condition
  rewrites were admitted.

## Object and container evidence

Objdiff CLI `3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/opening.c` | `0x2B14/0x2CD8` | 87.929634% | 22/22 | 18 |
| `board/player.c` | `0xBEB0/0xBDC0` | 96.373980% | 165/165 | 139 |
| `board/coin.c` | `0x4550/0x3F7C` | 81.022545% | 52/52 | 21 |
| `board/star.c` | `0x5C00/0x5A08` | 93.608350% | 90/90 | 66 |
| `board/last5.c` (Wave 78 correction) | `0x2458/0x650` | 17.356405% | 2/10 | 1 |

No previously exact function regressed. The final serialized `rtk ninja -j1`
build and the independent `dtk shasum -q -c config/GP6E01/build.sha1` command
both report `137 files OK`; `cmp` confirms original and rebuilt `main.dol` are
byte-identical. No REL changed, and the final build leaves
`config/GP6E01/symbols.txt` unchanged.
