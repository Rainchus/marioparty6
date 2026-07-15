# Native matching wave 66: Dice object and event flow

Wave 66 advances `board/dice.c` from 43 to all 68 target functions represented
and from 35 to 47 byte-exact functions under the required
`functionRelocDiffs=data_value` comparison. Source `.text` grows from `0x1A5C`
to `0x603C`; target/source text is now `0x6390/0x603C` at 92.468925%.
The 47 exact functions account for `0x337C` target text bytes. The complete
owner still diverges, so it remains `NonMatching` and none of its
fallback-linked bytes are counted as decompiled source.

## Authorities and recovered ownership

The MP6 target object, symbols, relocations, and data remain authoritative.
Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`
continues to authenticate the shared `src/game/board/sai.c` process, object,
number, and effect families. The MP7 target at commit
`f5ea780d07dab8188d3c02f7e9052360c591dc1b` corroborates the evolved event
and number-rendering families. MP6 instructions, relocations, retained data,
and same-game consumers establish every MP6-specific branch, field, table,
hook, message, effect, and lifecycle operation retained here.

This slice recovers the main Dice process and destructor, help-window setup,
the complete dice-object update state machine, player input dispatch, hit
resolution, number reset, object effects, equal-roll behavior, both inbound
particle families, and the complete special-NPC number-display path. The
retained MP6 tables now include the equal-roll animation bank, both inbound
scale/color families, the inbound animation bank, and the eight-byte coin
string buffer. `DiceNumPosSet` is restored as the definition-order inline used
twice by `mbDiceNumObjCreate`; it produces no standalone source symbol.

Declaration-order changes were accepted only when they reproduced an
authenticated body and the target's allocation graph. They are consistent
with the coloring order implemented by the pinned MWCC source oracle. No
inline assembly, opaque byte body, guessed intrinsic, or padding operation
was added.

## New exact closures

Twelve additional functions are byte-exact in this wave and add `0x2054` exact
target text bytes:

- process and UI: `DiceProcMain` (`0x25C`), `DiceProcDestroy` (`0x9C`), and
  `DiceHelpWinCreate` (`0x148`);
- object and input flow: `DiceObjOMExec` (`0x698`), `DiceExec` (`0x1E8`), and
  `DiceObjHit` (`0x2D8`);
- number/effect flow: `DiceNumObjReset` (`0xE8`), `DiceObjEffKill` (`0x88`),
  `DiceObjEffPuffDraw` (`0x374`), and `DiceObjEffTriDraw` (`0x48C`);
- compiler-shape closures: `DiceFadeOMExec` (`0x19C`) and
  `mbDiceNumObjCreate` (`0x5B0`).

All 35 exact Wave 65 functions remain exact. In particular, the recovered
object update implements its kill, motion, fade, face, and result paths, and
the two draw callbacks reproduce the target's particle lifetime, matrix,
color, display-list, and counter behavior without source-level tricks. The
fade closure comes from the target-proven reciprocal-multiply and explicit
`360 * time` expression order. The number constructor closes after restoring
the target-proven alpha-before-color aggregate ABI order.

## Represented residuals

Twenty-one represented functions remain real C matching work:

| Function | Target/source | Data-value score | Evidence-backed boundary |
| --- | ---: | ---: | --- |
| `mbDiceInit` | `0x74/0x7C` | 92.758620% | recovered six-cursor initialization; two target move artifacts remain |
| `mbDiceProcExec` | `0x168/0xC8` | 49.622223% | wrapper control flow remains incomplete |
| `mbDiceExec` | `0x2A8/0x168` | 48.658825% | wrapper control flow remains incomplete |
| `mbDicePlayerExec` | `0x134/0xAC` | 37.441560% | wrapper control flow remains incomplete |
| `DiceObjCreate` | `0x360/0x364` | 99.513885% | shared `mbObjMotionIDGet` prototype emits one target-absent `extsh` |
| `DiceNumObjBendOMExec` | `0x3C8/0x388` | 87.293390% | optional third-model hook-data path remains incomplete |
| `mbDiceSNpcNumCreate` | `0xF8/0x100` | 81.564514% | collection/cleanup behavior is represented; paired-vector copy lowering differs |
| `ev_DiceZorome` | `0x364/0x37C` | 90.387100% | target behavior is represented; register/control shape still diverges |
| `ev_DiceZoromeCoin` | `0x334/0x2D4` | 75.853660% | 64-coin behavior is represented; loop/source shape still diverges |
| `DiceZoromeEffHook` | `0x570/0x550` | 85.295975% | target particle behavior is represented; update shape still diverges |
| `DiceInEffCreate` | `0x80/0x7C` | 96.562500% | shared particle-create return prototype suppresses the target `extsh` |
| `DiceInDotEffCreate` | `0x6C/0x68` | 95.925930% | shared particle-create return prototype suppresses the target `extsh` |
| `DiceHitEffSet` | `0x24/0x24` | 86.111115% | global-address node shape differs |
| `DiceInEffHook` | `0x60C/0x5F4` | 94.643410% | complete target behavior is represented; register/expression shape remains |
| `DiceInDotEffHook` | `0x6C4/0x660` | 82.990760% | complete target behavior is represented; register/expression shape remains |
| `DiceObjEffCreate` | `0x368/0x360` | 98.967890% | target retains two move pairs not reproduced by the authenticated shape |
| `mbDiceSNpcNumObjCreate` | `0x150/0x160` | 88.928570% | model/camera behavior is represented; aggregate-vector lowering differs |
| `DiceSNpcNumUpdate` | `0x360/0x36C` | 95.532410% | display behavior is represented; aggregate-vector lowering and coloring remain |
| three SNpc vector leaves | `0x14/0x1C` each | 12.000000% | target uses paired-single copies; ordinary aggregate copies lower as integer moves |

The Zorome/equal-roll, inbound-effect, and SNpc routines are source-recovered
behavior, not matching claims. The MP6 target proves their calls, state
transitions, loop bounds, data references, camera math, digit placement, and
cleanup; the MP7 binary family independently authenticates the inbound/SNpc
source shape. Their emitted bodies still differ and remain fallback-linked.
No target function is now absent from the source owner.

## Object evidence

The final object-only validation for this WIP slice is:

```text
ninja build/GP6E01/src/board/dice.o
objdiff-cli diff -p . -u main/board/dice \
  -c functionRelocDiffs=data_value
```

The compile passes. Objdiff reports 68/68 target functions paired, 47 exact,
target/source `.text 0x6390/0x603C` at 92.468925%, `.rodata 0xE8/0x1E4` at
64.210526%, `.data 0x220/0x214` at 98.884760%, `.sdata 0x18/0x1B` at
66.666664%, `.sdata2 0x168/0x16C` at 95.027626%, and target/source
`.bss 0x40/0x3C` with all represented BSS bytes exact. The sole
`configure.py` entry remains `NonMatching`. This slice changes no linked
owner, so no full-DOL gate or Matching promotion is claimed.
