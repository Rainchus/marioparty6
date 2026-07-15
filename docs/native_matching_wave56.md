# Native matching wave 56: EXI promotion and Board owner recovery

Wave 56 promotes the complete retained MP6 `dolphin/exi/EXIBios.c` owner and
materializes substantial target-backed C in `board/player.c`, `board/star.c`,
`board/scroll.c`, and `dolphin/mic/m2s.c`. The three Board owners and M2S still
diverge, remain `NonMatching`, and continue to link their extracted fallback
objects. Only EXIBios is promoted.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target on the left and source on the right. Strict
  mapped/exact counts use default relocation identity; data-value scores use
  `functionRelocDiffs=data_value` where stated.
- Compiler: the project-pinned compiler and per-owner flags from
  `configure.py`.
- Primary evidence: MP6 instructions, control flow, effective relocations,
  callers, field offsets, initialized bytes, BSS ordering, and retained symbol
  boundaries.
- Corroborating source: authenticated Matching sibling source is used only
  where the MP6 object independently retains the same behavior and layout.

No assembly, byte blobs, register forcing, guessed fields, or fallback-backed
Matching claim is introduced.

## `dolphin/exi/EXIBios.c`: complete retained owner

Mario Kart: Double Dash commit
[`8a91a6b4`](https://github.com/doldecomp/mkdd/blob/8a91a6b4a32834dfb689fbdcf1dea8796be9c75f/libs/dolphin/exi/EXIBios.c)
configures this source owner `Matching`. Its authentic source carries
`#pragma scheduling off` around the implementation and resets scheduling at
the end; the file's relevant source revision is
[`62132446`](https://github.com/doldecomp/mkdd/commit/621324464e21b77a2d01eb8d85cfe58cf365b83d).
Restoring only that authenticated scheduling contract closes all 23 retained
MP6 functions.

Strict objdiff pairs 23/23 retained functions exactly. Target/source `.text`
are `0x19DC/0x20F0`; target/source total relocations are `187/219`. The six
source-only functions (`CompleteTransfer`, `EXIProbeReset`, `__EXIAttach`,
`EXISelectSD`, `EXIGetType`, and `EXIGetTypeString`) and their source-only data
are linker-stripped. The final linked owner range and full DOL therefore come
from the recovered C despite the larger unlinked source object. This is a
clean-C promotion, not an assembly exception.

## `board/player.c`: real player-work ownership

`MBPLAYERWORK` is recovered as a target-proven `0x50` structure and replaces
consumer-local anonymous overlays. Its four-entry `playerWork` bank is
source/target-exact `.bss 0x140`. `branch.c` now names the `masuNext` field,
and `opening.c` consumes the shared `moveF` and `playerNo` bitfields through
the owner header.

The source now represents 86 retained functions, 66 strictly exact, up from
11 mapped and 10 exact. Recovered families include work/hook ownership,
object/model/team helpers, layer/camera/culling, matrix and transform wrappers,
motion wrappers, coin/star/capsule state, voices, display, and space-corner
accessors. Target/source `.text` are `0xBEB0/0x33E8` at 26.040234%. Twenty
represented functions remain divergent at 78.666664%-99.545456%, and the
large movement/render/state-machine closure remains absent. The owner is not
promoted.

The shared-header integration recompiles `opening.c` and `branch.c` without
redundant private declarations. Under the configured data-value comparison,
opening retains 22 mapped/15 exact functions and branch retains 17/16; neither
owner is promoted.

## `board/star.c`: star and Ztar lifecycle

The recovered owner now includes the target-addressed `STARWORK` fields, the
999-slot star and Ztar object banks, the four-player display bank, and the
process/animation/hook roots. Source `.bss 0x1F48` is target-exact and source
`.sbss 0x34` exactly covers the target's named `0x34` prefix of `.sbss 0x38`.

Mapped/exact coverage rises from 17/17 to 67/55. The additions recover
initialization and cleanup, transform accessors, kill/display/chest wrappers,
star-space processes and destructors, grow/shrink/effect helpers, all-player
display helpers, and the corresponding Ztar lifecycle subset. Target/source
`.text` are `0x5C00/0x1564` at 21.559782%. Twelve represented functions still
retain compiler-shape or missing-closure differences, and the core object
creation/update/render paths remain absent. The owner stays fallback-linked.

## `board/scroll.c`: scroll and map-view lifecycle

The source now owns scroll initialization/cleanup, scroll event control,
collision-model cleanup, star-space lookup, map-view animation lifecycle,
bounded player-collision draining, map-sprite insertion, camera state, and
typed hooks. The target/source `.sbss` owners are both byte-exact at `0x28`.

Coverage rises from 9 mapped/7 exact to 15/11. Target/source `.text` are
`0x3E4C/0x608` at 9.658892%. The four strict divergers are
`mbev_Scroll`, `ScrollMain`, `MapViewCreate`, and `mbMapCameraSet`, all at
99.493670%-99.807690%; data-label equivalence makes the latter two exact under
the data-value comparison. Collision construction, map rendering, and their
execution closures remain absent, so the owner is not promoted.

## `dolphin/mic/m2s.c`: TU ordering and buffer advance

The target proves the BSS order and boundaries: `__M2SBlock` at `0x0`,
`__M2STmpHistory` at `0x1A0`, and `__M2SDebug` at `0x250`. Source `.bss` is
`0x25C` versus target `0x260`; the only target-only bytes are the named
four-byte tail. The source now retains that real ownership by placing
`__M2SCalibration` before the public function sequence instead of relying on
an artificial storage order.

Compound narrow shifts recover the target's signed-sample update form.
`M2SAdvanceBuffer` caches the debug index and source/destination pointers,
uses the target countdown copy, and stores the index once after the copy. It
improves from 64.012660% (`0x148`) to 74.050640% (`0x140`) against target
`0x13C`. Whole target/source `.text` are `0xD50/0xD04`; the data-value score is
79.922540%, with 1/14 strict-exact functions and target/source relocations
`143/149`. A `defer_codegen` experiment moved calibration to the wrong point
and regressed BSS order, so it was removed rather than retained.

## Gate

The modified owners and the affected `opening.c`, `branch.c`, `last5.c`, and
`status.c` consumers compile under their configured flags. The final
integrated build/container gate is:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- progress: 9.17% code/31.33% data overall and 47.07% code/74.48% data in the
  DOL, with 301/895 and 292/396 Matching owners respectively

The final build leaves `config/GP6E01/symbols.txt` unchanged in both the
staged and unstaged diffs.
