# Native matching wave 63: Player movement and collision core

Wave 63 advances `board/player.c` from 129 to 150 of 165 target functions
represented. Strict-exact coverage rises from 105 to 117 functions and the
data-value gate rises from 116 to 130. Source `.text` grows from `0x484C` to
`0x80CC` by recovering real movement, walk-number, rotation, collision, swap,
and position-reset code. The owner remains `NonMatching`; none of its
fallback-linked bytes are counted as decompiled source.

The required caller/callee ABI correction in `board/coin.c` also makes
`mbCoinObjMotSet` strict-exact and raises Coin from 5 to 6 exact functions.
Coin remains `NonMatching`.

## Sources and method

The MP6 target object, named symbols, relocations, data bytes, and aligned
instructions are authoritative. Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb`, particularly
`src/game/board/player.c`, authenticates the shared movement, walk-number,
collision, eye-material, swap, and position-fix source families. MP6
instructions establish every MP6-only flag, field, branch, constant, and
aggregate-copy difference.

The MWCC source oracle was used before changing compiler shape. In particular,
`StackFrame.c` and `Coloring.c` explain how the target walk-number aggregate
copy's paired-single temporaries enlarge the saved FPR/GPR sets and frame;
`CInline.c` confirms that an inline-depth lever is not applicable to the
standalone update symbol. No dummy locals, padding objects, byte blobs,
register-forcing constructs, or assembly were added.

## Recovered ownership

The target-proven `MBPLAYERWORK` remains `0x50` bytes. This wave replaces
opaque storage with signed halfwords at `0x06`, `0x08`, and `0x0C`, restores
the `posFixObj` pointer and `HuVecF` position-fix state, and preserves unknown
field names where no source authority exists. The four movement-space flags
use Matching-MP5 names with MP6 target-proven values `1`, `2`, `4`, and `8`.

Recovered Player paths include:

- normal, jump, and climb movement creation/update, including target motion,
  timing, direction, remaining-time, and completion behavior;
- rotation creation/update and the real Player work ownership;
- walk-number model creation, projection, digit display, pause, and teardown;
- all nine previously absent collision-engine owners, including all-player,
  space, circle, additive, ball, corner, update, and resnap paths;
- player swap, collision-order reset, eye-material darkening, position reset,
  and position reset for all players;
- star, rank, team, effect, and coin paths whose prior compiler shape still
  diverged.

`PLAYERCOLWORK` is now the target `0x10` layout. Matching MP5 and MP6 bit tests
independently prove `motStartF` in bit 7 and the reinitialization/kill flag in
bit 6. The unusual state-2 fallthrough in `PlayerColOMExec` is retained because
the target and sibling control flow both require it.

The Coin interface now accepts an `int motNo` for create/create2/motion-set.
MP6 callers pass the raw second argument and the Coin target bodies consume it
without signed-byte extension. This closes `mbCoinObjMotSet` at `0x58/0x58`;
the two larger creation bodies remain real Coin work.

## Exact closures

Twelve Player functions newly pass the strict object gate:

- `PlayerRotateOMExec` and `PlayerColInit`;
- `mbPlayerEffectSet`, `mbPlayerStarGet`, `mbPlayerGrpStarGet`,
  `mbPlayerStarAdd`, `mbPlayerRankGet`, and `mbPlayerTeamRankGet`;
- `mbPlayerCoinAdd`, `mbPlayerTeamFindOpp`, `mbPlayerColOrderReset`, and
  `mbPlayerSwap`.

`mbPlayerRotateStart` (`0x21C`) and `mbPlayerEyeMatDarkSet` (`0x218`) are also
100% under `functionRelocDiffs=data_value`; their strict scores are
99.851850% and 99.888060% because compiler-local data-symbol identities still
differ. Reusing the authenticated collision interpolation lifetime makes
`PlayerColOMExec` exact under the same data-value gate at `0x520/0x520`; its
strict score is 99.923780%. No previously strict-exact Player function
regresses.

The target-authentic eye table belongs before the still-incomplete Player data
closure. That correct order exposes the existing `mbPlayerTagNameMesGet`
table-relocation difference under the data-value mode, so the net data-value
gain is 14 functions even though 15 functions newly reach 100%. Its opcode
shape and strict score remain unchanged.

## New paired residuals

The remaining newly represented functions retain recovered behavior but do
not pass the strict object gate:

| Function | Target/source | Strict | Data-value |
| --- | ---: | ---: | ---: |
| `mbPlayerMoveMain` | `0x568/0x568` | 99.193640% | 99.251440% |
| `PlayerMoveOMExec` | `0x394/0x394` | 94.550220% | 94.550220% |
| `mbMoveNumCreateColor` | `0x27C/0x27C` | 99.899370% | 99.899370% |
| `MoveNumOMExec` | `0x480/0x460` | 94.197914% | 94.267365% |
| `PlayerColCornerSnap` | `0x15C/0x15C` | 99.540230% | 99.655174% |
| `mbPlayerPosReset` | `0x178/0x178` | 99.574470% | 99.680855% |
| `mbPlayerPosResetAll` | `0x228/0x228` | 99.710144% | 99.782610% |
| `mbev_PlayerColMasuAllSet` | `0x2E4/0x2CC` | 89.589190% | 89.589190% |
| `mbev_PlayerColMasu` | `0x284/0x278` | 93.521736% | 93.521736% |
| `mbev_PlayerColCircleAdd` | `0x2D8/0x2C8` | 93.752750% | 93.752750% |
| `mbev_PlayerColMasuAdd` | `0x26C/0x23C` | 85.032260% | 85.032260% |
| `mbev_PlayerColBall` | `0x2CC/0x2B0` | 87.525140% | 87.692730% |
| `PlayerColCornerSet` | `0x2B0/0x268` | 85.017440% | 85.017440% |
| `PlayerColOMExec` | `0x520/0x520` | 99.923780% | 100.000000% |
| `mbev_PlayerColSet` | `0x1DC/0x1BC` | 93.067230% | 93.151260% |

The aggregate `pos = posNorm` assignment in `MoveNumOMExec` recovers the
target paired-single copy and raises its data-value score from 92.631940% to
94.267365%. The residuals above were left as C work rather than imitated with
storage or instruction tricks.

## Object evidence and boundary

The final serialized object build and pinned objdiff passes report:

- Player: 150/165 paired, 117 strict-exact, 130 data-value-exact;
- Player target/source `.text` `0xBEB0/0x80CC`, at 66.051620% strict and
  66.114300% data-value;
- Player `.bss 0x140` is exact; the semantic `.sdata` bytes are exact at
  target/source raw sizes `0x68/0x65`;
- Coin: 24/52 paired, 6 strict/data-value exact, target/source `.text`
  `0x4550/0x103C`, at 17.055681% strict and 17.064697% data-value;
- both source objects compile with the pinned MWCC configuration, and
  `git diff --check` passes.

Fifteen Player targets remain unpaired, led by `mbPlayerInit`, `PlayerTurn`,
`DiceRun`, the older process-based `PlayerMove`, metal/Biri-Q creation and
update, and `MasuCoinExec`. The complete Player and Coin objects still diverge,
so `configure.py` is unchanged. No Matching flip occurred; consequently this
is an object-only WIP slice and no full-DOL checksum/container gate is claimed.
