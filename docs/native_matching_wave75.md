# Native matching wave 75: Board state initialization

Wave 75 closes four represented Board functions totaling `0x67C` target text
bytes. It restores the player-handicap accessor used by board save setup, the
typed special-wipe texture allocation, and both remaining represented Scroll
register-lifetime residues. No owner is promoted: Board retains three code
residues, Wipe retains one code residue plus a constant-pool difference, and
Scroll still lacks 16 target functions. Fallback-linked bytes are not counted
as decompiled source.

## Authorities and recovery boundary

The MP6 target objects, instructions, relocations, symbols, data consumers,
and control-flow graphs are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Board
source family, including the player-handicap inline accessor and Scroll's
function-scope save/restore state. Sibling source was retained only where the
MP6 target independently proved the same contract.

The local `mwcc-decomp` checkout at
`094b96ca1df4a035b5f93c351f773306c0241f3f` was used to interpret the two
inverse register-color cycles. Its allocator assigns the lowest available
volatile register from the interference graph; for same-width locals with the
same lifetime, declaration-list order is therefore a direct source-shape
lever. No inline assembly, byte body, register-forcing declaration, volatile
match trick, opaque buffer, or compiler sweep was used.

## Board save initialization

MP5's Matching `include/game/board/player.h` authenticates
`mbPlayerHandicapGet`, and its board main owner authenticates the call in save
initialization. Restoring the inline header body also restores its compiler
visibility and packed return lifetime. `mbSaveInit` becomes exact at
`0x280/0x280`; the other 31 previously exact Board functions do not regress.

The shared player header now owns the player prototypes that `board.c` had
duplicated locally. Board remains `NonMatching`: `mbObjectSetup`, `mbMain`, and
`mbNextTime` still diverge.

## Typed special-wipe allocation

GX and Hu3D consumers prove that `WIPE_SPECIAL_DATA::texData` points to a
320x240 RGB565 buffer, so its recovered type is `u16 *`. The target allocation
sequence separately retains the byte count, untyped allocation result, and
typed texture pointer. Expressing those real values in their target-backed
declaration order makes `mbWipeSpecialInit` exact at `0x1CC/0x1CC` without
changing any previously exact function.

`mbWipeSpecialFadeOutCreate` remains divergent. A proposed two-call wrapper
was rejected from the target CFG: the tutorial-exit path deliberately skips
`mbWipeSpecialWait`, whereas that wrapper would wait unconditionally. An
unused saved register is not enough authority to invent a local or a call.

## Scroll lifetime closure

Both represented Scroll residues already had exact target/source sizes,
control flow, and relocations. Each difference was a complete inverse register
cycle between two same-width locals with overlapping lifetimes:

- `mbev_Scroll` (`0xF4`) saves and restores pause-disable state and the camera
  stack number. Restoring declaration-list order removes all four inverse
  `r28`/`r29` operands.
- `ScrollMain` (`0x13C`) retains the star-space and help-window IDs across the
  scroll execution. Restoring their declaration-list order removes all eight
  inverse `r30`/`r31` operands.

Both functions are now 100%. All 15 represented Scroll functions are exact
under the required data-value relocation comparison; 16 target functions
remain absent, so the owner stays fallback-linked.

## Bounded rejected probes

Three source-shape probes were compiled and rejected rather than retained on
score alone:

- Moving `mbMusBoardFadeOut`'s untyped allocation temporary ahead of its typed
  work pointer was byte-neutral: the same 12 stack-slot operands remained and
  `board/audio.c` stayed at 48/49 exact functions.
- Moving `mbObjectSetup`'s authentic `nightF` declaration into its narrower
  source scope was byte-neutral: the same 20 register operands remained.
- Moving `mbMasuPlayerColorSet`'s `dayF` declaration ahead of `type` regressed
  the function from two operand differences to seven, so it was reverted.

These probes do not appear in the retained source.

## Object and container evidence

Objdiff CLI `3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact | Remaining code residue |
| --- | ---: | ---: | ---: | ---: | --- |
| `board/board.c` | `0x1CFC/0x1CFC` | 99.919136% | 35/35 | 32 | `mbObjectSetup`, `mbMain`, `mbNextTime` |
| `board/wipe.c` | `0x3664/0x365C` | 99.936516% | 33/33 | 32 | `mbWipeSpecialFadeOutCreate` |
| `board/scroll.c` | `0x3E4C/0x608` | 9.681465% | 15/31 | 15 | 16 absent functions |

Board retains all 618 target/source text relocations. Wipe retains all 639
text relocations and exact `.data 0xA0`/`.bss 0x38`; its `.sdata2` remains
target/source `0xA8/0xB8` at 82.954544%. Scroll retains exact `.sbss 0x28`.

The final serialized full build and explicit DTK checksum both report
`137 files OK`; `cmp` confirms the rebuilt and original `main.dol` are
byte-identical. No REL changed. The final build leaves
`config/GP6E01/symbols.txt` unchanged.
