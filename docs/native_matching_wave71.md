# Native matching wave 71: Board creation and effect ownership

Wave 71 recovers eight byte-exact Board functions totaling `0x938` target
text bytes across `board/coin.c`, `board/dice.c`, `board/player.c`, and
`board/star.c`. Coin gains three lifecycle functions, Dice closes three
creation helpers, Player gains the second Biri-Q particle hook, and Star
recovers its core object family with an exact rotation owner. All four owners
remain `NonMatching`; none of their fallback-linked bytes are counted as
decompiled owners.

## Authorities and method

The MP6 target objects, named symbols, instructions, relocations, and retained
data are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` authenticates the shared Coin,
Dice/Sai, and Player effect families. Exact same-game consumers establish the
two corrected interfaces: Matching `board/guide.c` consumes
`mbObjMotionIDGet` as an `int`, and its exact particle constructors retain an
`int` local across `mbParticleCreate`, matching the two MP6 Dice targets.

The Biri-Q hook is recovered directly from the MP6 CFG, call relocations,
particle offsets, and data-update loop. Its source establishes the real
`MBPARTICLE` union member at `+0x1C` and completes the `PLAYERBIRIQWORK`
fields through `+0x0A`. No storage padding, opaque byte body, register-forcing
construct, or assembly was added.

## Exact closures

Coin adds three exact functions and `0x258` target text bytes:

- `mbCoinClose` (`0xD0`);
- `CoinClose` (`0x144`);
- `mbCoinEffObjCreate` (`0x44`).

Their source also restores the target-proven `0x10` `COINEFFDATA` layout,
the 64-entry effect bank, model owners, and model-process pointer. The
Matching-MP5 `!= NULL` spelling reproduces `CoinClose`'s pointer comparison;
the shorter null comparison was measured and rejected.

Dice adds three exact functions and `0x44C` target text bytes:

- `DiceObjCreate` (`0x360`);
- `DiceInEffCreate` (`0x80`);
- `DiceInDotEffCreate` (`0x6C`).

The object constructor closes after correcting `mbObjMotionIDGet` to the
same-game-authenticated `int` ABI. Both object-system functions remain exact
after that correction. The two particle constructors close with the
same-game-authenticated `int modelId` lifetime, which retains the target
sign-extension after `mbParticleCreate`.

Player adds exact `BiriQEffect2Hook` (`0x118`). The target initializes the
secondary particle bank, copies the primary particle data through the real
`hookData` member, forces white RGB, and clamps the scaled alpha to 255. The
recovered source reproduces every target instruction and relocation under the
required data-value comparison.

Star adds exact `StarObjRotate` (`0x17C`) and represents five formerly absent
functions totaling `0xCF4` target text bytes. `StarObjCreate`,
`StarObjOMExec`, `StarObjEffHook`, and `StarMatHook` recover the real `0x60`
`STARWORK`, object/model/material setup, five-mode update path, transform and
sign-model ownership, particle spawn/fade loop, and global-light material
hook. The independently recovered `MBPARTICLE::hookData` field supplies the
real `+0x1C` owner link used by both the Player and Star targets. Target and
callee stores establish the nameless `int *` output parameters of
`mbObjStarTevStageSet`; no guessed parameter names were introduced.

`StarObjRotate` closes after the target's sole residual identifies the signed
halfword at `STARWORK + 0x20` as `time`, not the earlier reconstructed
`rotY` field. `StarMatHook` remains exact-size at 99.603170%, with only 25
stack-slot operands divergent. One target-derived declaration-order trial
regressed it to 74.603170% and was fully reverted rather than retained.

## Recovered WIP shape and rejected probes

`mbDiceSNpcNumCreate` improves from 81.564514% to 81.935486%. Hoisting the
temporary object position ahead of the output offset restores the target
stack-slot order, and moving the color read ahead of the aggregate copy
restores the target lifetime. Its vector copy still lowers as six integer
loads/stores rather than the target four paired-single instructions, so the
function remains real matching work and is not counted as exact.

The Matching-MP5 direct-array spelling for `mbDiceInit` was compiled once.
It expanded the function from the near-exact `0x7C` form to `0xD0` and scored
0%, so it was fully reverted. The existing six-cursor source remains at
`0x7C` against target `0x74`; no allocator or declaration-order sweep was
performed.

The near-exact whole-owner closeouts in `board/audio.c`, `board/board.c`, and
`board/masu.c` were also re-audited before this batch. Their remaining
differences are allocator/stack-slot coloring without a sibling-authenticated
source lever. No speculative edit was retained.

## Object evidence

All changed objects compile with the pinned MWCC configuration. Objdiff CLI
`3.7.2`, using `functionRelocDiffs=data_value`, reports:

| Owner | Target/source `.text` | Data-value score | Paired | Exact |
| --- | ---: | ---: | ---: | ---: |
| `board/coin.c` | `0x4550/0x1294` | 20.446123% | 27/52 | 9 |
| `board/dice.c` | `0x6390/0x6040` | 92.523540% | 68/68 | 50 |
| `board/player.c` | `0xBEB0/0x81E4` | 66.687890% | 151/165 | 131 |
| `board/star.c` | `0x5C00/0x2254` | 35.109547% | 72/90 | 61 |
| `board/object.c` | `0x2838/0x2838` | 100.000000% | 80/80 | 80 |

The eight new functions are individually 100% under the required comparison.
No complete fallback owner is promoted in this wave. The final serialized
`rtk ninja` and explicit `dtk shasum -q -c config/GP6E01/build.sha1` both
report `137 files OK`; `cmp` confirms the rebuilt and original `main.dol` are
byte-identical. The final build leaves `config/GP6E01/symbols.txt` unchanged.
