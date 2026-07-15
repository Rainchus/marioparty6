# Native matching wave 65: Dice engine and number rendering

Wave 65 advances `board/dice.c` from 15 to 43 of 68 target functions
represented and from one to 35 byte-exact functions under the required
`functionRelocDiffs=data_value` object comparison. Source `.text` grows from
`0x254` to `0x1A5C`; target/source text is now `0x6390/0x1A5C` at
25.429379%. The 35 exact functions account for `0x1328` target text bytes.
The complete owner still diverges, so it remains `NonMatching` and none of
its fallback-linked bytes are counted as decompiled source.

## Authorities and compiler shape

The MP6 target object, symbols, relocations, and data are authoritative.
Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`
marks `src/game/board/sai.c` Matching and authenticates the shared process,
dice-object, number-object, mesh-bend, and lifecycle source families. The MP7
target at commit `f5ea780d07dab8188d3c02f7e9052360c591dc1b`
independently corroborates the evolved number-rendering family: the MP6 and
MP7 number update, shrink, mesh-copy, and mesh-bend bodies are identical once
relocation-bearing words are excluded. MP6 instructions and data establish
the MP6-only fade state, number color, constants, hooks, and branches.

The MWCC source oracle was consulted before source tuning. `CodeGenOptPPC.c`
and `TargetOptimizer-ppc-mach.c` establish that scheduling and peephole
optimization are independent controls; `IroUnrollLoop.c` explains why the
six-cursor initialization loop is not flattened. Target-wide instruction
fingerprints prove this owner used:

```text
-O4,p -schedule off -opt nopeephole
```

With only `-O4,p`, leaf functions had the right unrolling and sizes but the
wrong scheduling. Disabling scheduling closed epilogues and instruction
ordering; disabling peepholes closed update-form loads, record-form compares,
and folded global addresses across the owner. These flags are configured only
for `board/dice.c`.

## Recovered ownership

The source now carries the target-backed retained tables and globals, plus
the real work layouts:

- `DICE_WORK` is `0x64`, with signed 11-entry value storage, three tutorial
  values, three results, position/color/object/effect state, render buffers,
  animations, and timers. The target-proven but semantically unresolved word
  at `+0x4C` remains neutrally named `_unk4C`.
- `DICE_OBJ_WORK` is `0x10`; its flags occupy one byte, followed by implicit
  alignment, lock/time/value halfwords, and the sound handle.
- `DICE_NUM_WORK` is `0x0C`; its byte-backed flags, value, MP6 color byte,
  bend mode, and timers reproduce every proven consumer offset.
- `DICE_FADE_WORK` is eight bytes and `DICE_SNPC_NUM_WORK` is two bytes. The
  SNpc third flag remains `_unk2` because no authenticated source names its
  semantics.
- `DICE_NUM_VTX` recovers the position-plus-weight vertex copy used by the
  exact mesh deformation functions.

`diceObjFileTbl`, `numberFileTbl`, the hit/pad hook banks, number-object bank,
position tables, Kuribo/Patapata result tables, and the process/fade/result
storage now pair with their target owners. The target symbol file repeats
`dicePadBtnHook` at both `+0x14` and `+0x28`; target stores and Matching MP5
source prove that `+0x28` is `diceMotHook`. The metadata typo was documented,
not copied into source or rewritten in `symbols.txt`.

## Exact closures

The following 35 functions are byte-exact under the required data-value
relocation comparison:

- core entry paths: `DiceProcExec`, `mbDiceChanceTradeExec`, and
  `mbDiceChanceCharExec`;
- lifecycle and input: `DiceObjKillSet`, `mbDiceFadeSet`, `mbDiceKill`,
  `mbDiceClose`, `DiceKill`, `DicePlayerPadBtn`, and `DiceReadyWait`;
- public hooks and accessors: `mbDiceKillCheck`, `mbDiceKillCheckAll`,
  `mbDiceResultGet`, all three hook setters, `mbDiceObjHit`, `mbDiceTypeGet`,
  both tutorial accessors, `mbDiceValueNoGet`, `mbDiceMaxGet`, and
  `mbDiceValueMaxGet`;
- number rendering: `DiceNumObjOMExec`, `mbDiceNumObjKill`,
  `DiceNumObjShrinkOMExec`, `mbDiceNumStopCheck`, `mbDiceNumKill`,
  `mbDiceNumShrinkSet`, `mbDiceStub`, `DiceNumObjMdlCopy`, and
  `DiceNumObjMdlBend`;
- special-NPC leaves: `mbDiceSNpcNumKill`, `mbDiceSNpcNumDispSet`, and
  `mbDiceSNpcNumSet`.

The exact `DiceNumObjOMExec` recovers the MP6-specific two-digit spacing,
vertical arc, fade threshold, squared alpha, static white `GXColor`, and
by-value color ABI. `DiceNumObjShrinkOMExec`, `DiceNumObjMdlCopy`, and
`DiceNumObjMdlBend` retain the authenticated sibling source shape without
padding, casts that alter storage, or assembly.

## Deliberate residuals

Eight represented functions remain real C work:

| Function | Target/source | Data-value score | Evidence-backed boundary |
| --- | ---: | ---: | --- |
| `mbDiceInit` | `0x74/0x7C` | 92.758620% | six-cursor loop is recovered; two redundant moves remain |
| `mbDiceProcExec` | `0x168/0xC8` | 49.622223% | wrapper behavior remains incomplete |
| `mbDiceExec` | `0x2A8/0x168` | 48.658825% | wrapper behavior remains incomplete |
| `mbDicePlayerExec` | `0x134/0xAC` | 37.441560% | wrapper behavior remains incomplete |
| `DiceNumObjBendOMExec` | `0x3C8/0x388` | 87.293390% | optional third-model hook-data owner is not authenticated |
| three SNpc vector leaves | `0x14/0x1C` each | 0% | target uses paired-single loads/stores |

The ordinary-C and `HuCopyVecF` shapes for the SNpc leaves were both compiled:
one emits integer aggregate copies and the other scalar float copies, while
the target uses paired-single operations. No authenticated sibling or header
provides the original intrinsic spelling, so no intrinsic guess, cast trick,
or inline assembly was admitted.

Twenty-five target functions are still unrepresented, including the main
process/destructor, help window, dice object/fade/hit state machines, number
creation/reset, equal-roll event/effects, and SNpc creation/update. They remain
the next work in this owner rather than empty placeholders.

## Object evidence

The final object-only validation for this WIP slice is:

```text
ninja build/GP6E01/src/board/dice.o
objdiff-cli diff -p . -u main/board/dice \
  -c functionRelocDiffs=data_value
```

The compile passes. Objdiff reports 43/68 functions paired, 35 exact,
target/source `.text 0x6390/0x1A5C`, `.rodata 0xE8/0x94` at 78.723404%,
`.data 0x220/0x14D` at 75.940704%, and target/source `.bss 0x40/0x3C` with
all represented BSS bytes exact. The sole `configure.py` entry remains
`NonMatching`. This slice changes no linked owner, so no full-DOL gate or
Matching promotion is claimed.
