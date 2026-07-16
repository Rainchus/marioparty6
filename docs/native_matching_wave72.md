# Native matching wave 72: Board lifecycle and event flows

Wave 72 represents another `0x1D04` target text bytes in recovered C across
`board/coin.c`, `board/player.c`, and `board/star.c`. It also closes eight
functions totaling `0x6B4` target text bytes under objdiff's required
`functionRelocDiffs=data_value` comparison. `board/wipe.c` reaches 31 of 33
exact functions. All four incomplete owners remain `NonMatching`; their
fallback-linked bytes are not counted as decompiled owners.

## Authorities and method

The MP6 target objects, instructions, relocations, symbols, and retained data
are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` supplies the authenticated shared
Board framework. Each port was reconciled against the MP6 target rather than
accepted from sibling parity: MP6 establishes the differing field offsets,
flags, constants, branch shapes, direct data IDs, and object lifetimes.

No inline assembly, byte body, opaque storage substitute, register-forcing
construct, or invented symbol was added. One target-led `board/wipe.c`
declaration-lifetime probe regressed `mbWipeSpecialInit` and was fully
reverted. No allocator sweep was attempted after that negative result.

## Coin lifecycle and effects

Seven formerly absent functions now represent `0xF84` target text bytes:

- `mbCoinInit` (`0xE0`);
- `CoinInit` (`0x1FC`);
- `CoinDraw` (`0x35C`);
- `CoinMain` (`0x1B0`);
- `mbCoinEffCreate` (`0x4A0`);
- `CoinEffCreate` (`0x150`);
- `CoinEffHook` (`0x1AC`).

The recovered source establishes the real `0x160` `MBCOINOBJDATA`, the
`0x1144` model/effect bank, and both adjacent 16-entry effect-bank tables.
The target deliberately indexes 32 entries from the first table; retaining
the adjacent arrays reproduces that original source/data ownership without
inventing a combined declaration. Coin's `.rodata`, `.data`, `.bss`,
`.sdata`, and `.sbss` sections now compare at 100%. The new functions are
substantial real C coverage but are not exact yet.

## Player Biri-Q setup

`mbPlayerBiriQSet` is exact (`0x200`). The target proves the two recovered
`GW_PLAYER` bits, the public setter ABI, and the diagnostic string. The
setter's particle path also establishes the typed model temporary and its
conversion lifetime.

`BiriQEffectCreate` represents another `0x1A8` target bytes at 94.905660%.
Its recovered source uses the already target-proven `MBPARTICLE::hookData`
owner and the callee-proven `mbObjBiriQColorSet(MBMODELID, BOOL, float,
GXColor)` ABI. The target's radius expression is retained as
`21.0f * (0.006666667f * radius)` rather than algebraically rewritten.

## Star and Ztar flow

Five formerly absent functions now represent `0x9D8` target text bytes:

- `mbStarMasuNextSet` (`0x168`);
- `mbStarGetExec` (`0x11C`);
- `ZtarObjCreate` (`0x254`);
- `ZtarObjOMExec` (`0x384`);
- `ZtarObjRotate` (`0x17C`, exact).

The Ztar object path uses the real 999-slot owner, direct
`DATANUM(DATA_capsulechar1, 0x20)`, real HSF/material data, and
`MBPARTICLE::hookData`. The MP6 target corrects two sibling assumptions: the
rotation completion path clears `rotateF` (`0x40`), not `scaleF`, and the
shrink path retains `pos.y += -4.0f`. The empty shrink-idle completion case is
also retained because it is present in the target CFG. Existing
`StarObjOMExec` improves to 98.365030%.

## Wipe lifecycle closure

The target's callee-saved base-pointer and free-argument lifetimes close six
previously divergent functions:

- `mbWipeSpecialClose` (`0xA4`);
- `mbWipeSpecialKill` (`0x74`);
- `mbWipeSpecialCreate` (`0xC4`);
- `mbWipeSpecialFadeInCreate` (`0xD4`);
- `mbWipeSpecialCheck` (`0x3C`);
- `mbWipeSpecialWait` (`0x4C`).

The exact wait loop calls the definition-order auto-inlined check helper,
which reproduces the target pointer load inside the loop condition. The free
paths retain local aliases only where the target preserves those values in
callee-saved registers, and clear `work` at the target-proven branch join.
Only `mbWipeSpecialInit` (92.226090%) and
`mbWipeSpecialFadeOutCreate` (95.088890%) remain divergent.

## Object evidence

All changed objects compile with the pinned MWCC configuration. Objdiff CLI
`3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/coin.c` | `0x4550/0x2204` | 41.045536% | 34/52 | 9 |
| `board/player.c` | `0xBEB0/0x8578` | 68.561040% | 153/165 | 132 |
| `board/star.c` | `0x5C00/0x2C14` | 45.017155% | 77/90 | 62 |
| `board/wipe.c` | `0x3664/0x3650` | 99.679690% | 33/33 | 31 |

The final serialized full build and explicit DTK checksum both report
`137 files OK`; `cmp` confirms the rebuilt and original `main.dol` are
byte-identical. The final build leaves `config/GP6E01/symbols.txt` unchanged.
